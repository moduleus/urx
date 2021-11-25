/*!
 * Copyright Moduleus
 * \file uff_reader_v0_0_1.cpp
 * \brief
 */

#include "uff_reader_v0_2.h"

#include <type_traits>
#include <limits>
#include <assert.h>

namespace uff
{

    void ReaderV0_2::printSelf(std::ostream& os, std::string indent) const
    {
        superclass::printSelf(os, indent);
    }

    bool ReaderV0_2::updateMetadata()
    {
        m_dataset = std::make_shared<uff::Dataset>();

        try
        {
            H5::Exception::dontPrint();

            H5::H5File file(m_fileName, H5F_ACC_RDONLY);

            // Version
            H5::Group version(file.openGroup("version"));
            readVersion(version);

            assert(m_dataset->version().major() == 0);
            assert(m_dataset->version().minor() == 2);

            // Tranform version
            m_dataset->setVersion(uff::Version());

            // Channel Data
            H5::Group acquisition(file.openGroup("channel_data"));
            readAcquisition(acquisition);

            file.close();
            return true;
        }  // end of try block
        // catch failure caused by the H5File operations
        catch (H5::Exception error)
        {
            error.printErrorStack();
            std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
        }

        return false;
    }

    uff::Aperture ReaderV0_2::readAperture(const H5::Group& group)
    {
        uff::Aperture aperture;

        // origin
        aperture.setOrigin(readTransform(group.openGroup("origin")));

        // "window"
        aperture.setWindow(readOptionalStringDataset(group, "window"));

        // "f_number"
        aperture.setFNumber(readOptionalDoubleDataset(group, "f_number"));

        // "fixed_size"
        aperture.setFixedSize(readOptionalDoubleDataset(group, "fixed_size"));

        return aperture;
    }

    void ReaderV0_2::readAcquisition(const H5::Group& group)
    {
        uff::Acquisition& acquisition = m_dataset->acquisition();
        acquisition.setAuthors(readStringDataset(group, "authors"));
        acquisition.setDescription(readStringDataset(group, "description"));
        acquisition.setLocalTime(readStringDataset(group, "local_time"));
        acquisition.setCountryCode(readStringDataset(group, "country_code"));
        acquisition.setSystem(readStringDataset(group, "system"));
        acquisition.setSoundSpeed(readDoubleDataset(group, "sound_speed"));

        // Fields who doesn't exist in uff v0.0.1
        acquisition.setTimeOffset(0);

        // Create Group infos
        std::shared_ptr<uff::GroupData> groupData = std::make_shared<uff::GroupData>();
        std::shared_ptr<uff::Group> uffGroup = std::make_shared<uff::Group>();
        acquisition.addGroupData(groupData);
        acquisition.addGroup(uffGroup);
        acquisition.setInitialGroup(uffGroup);
        groupData->setGroup(uffGroup);

        // Create GroupData
        std::vector<size_t> dataDims;
        std::vector<float> dataFloat;
        readArrayDataset(group, "data", dataFloat, dataDims);
        if (dataDims.size() != 4)
        {
            std::cerr << "Uff::ReaderV0_2 : Dataset dimension != 4" << std::endl;
            return;
        }
        // Copy data to group Data (casting to int16_t)
        groupData->data().resize(dataFloat.size());
        for (uint32_t i = 0; i < dataFloat.size(); i++) { groupData->data()[i] = (int16_t)dataFloat[i]; }

        // Extract acquisition data parameters
        const uint32_t nbFrames = dataDims[0];
        const uint32_t nbEvents = dataDims[1];
        const uint32_t nbChannels = dataDims[2];
        const uint32_t nbSamples = dataDims[3];

        // Create unique group
        uffGroup->setDescription("");
        uffGroup->setTimeOffset(0);
        uffGroup->setRepetitionCount(nbFrames);
        uffGroup->setRepetitionRate(readDoubleDataset(group, "repetition_rate"));

        // Probes
        H5::Group probes(group.openGroup("probes"));
        acquisition.setProbes(readArray<std::shared_ptr<uff::Probe>>(probes));

        // Unique waves
        H5::Group waves(group.openGroup("unique_waves"));
        acquisition.setUniqueWaves(readArray<std::shared_ptr<uff::Wave>>(waves));

        // Unique events
        H5::Group uniqueEvents(group.openGroup("unique_events"));
        acquisition.setUniqueEvents(readArray<std::shared_ptr<uff::Event>>(uniqueEvents));
        for (auto& uniqueEvent : acquisition.uniqueEvents()) { uniqueEvent->receiveSetup().setNumberOfSamples(nbSamples); }

        // Create sequence
        uff:Sequence sequence;
        sequence.setTimeOffset(0);
        H5::Group timedEvents(group.openGroup("sequence"));
        sequence.setTimedEvents(readArray<TimedEvent>(timedEvents));

        uffGroup->setSequence(sequence);
    }

    double ReaderV0_2::readDoubleDataset(const H5::Group& group, const std::string& name)
    {
        H5::StrType datatype(H5::PredType::NATIVE_DOUBLE);
        H5::DataSet dataset = group.openDataSet(name);
        double value;
        dataset.read(&value, datatype);
        dataset.close();
        return value;
    }

    std::optional<double> ReaderV0_2::readOptionalDoubleDataset(const H5::Group& group, const std::string& name)
    {
        H5::StrType datatype(H5::PredType::NATIVE_DOUBLE);
        H5::DataSet dataset = group.openDataSet(name);
        double value;
        dataset.read(&value, datatype);
        dataset.close();
        std::optional<double> result = std::nullopt;
        if (!std::isnan(value))
            result = value;
        return result;
    }

    uff::Element ReaderV0_2::readElement(const H5::Group& group)
    {
        uff::Element element;

        element.setX(readOptionalDoubleDataset(group, "x"));
        element.setY(readOptionalDoubleDataset(group, "y"));
        element.setZ(readOptionalDoubleDataset(group, "z"));

        return element;
    }

    std::shared_ptr<uff::Event> ReaderV0_2::readEvent(const H5::Group& group)
    {
        auto event = std::make_shared<uff::Event>();

        // "receive_setup"
        event->setReceiveSetup(readReceiveSetup(group.openGroup("receive_setup")));

        // "transmit_setup"
        event->setTransmitSetup(readTransmitSetup(group.openGroup("transmit_setup")));

        return event;
    }

    std::weak_ptr<uff::Excitation> ReaderV0_2::readExcitation(const H5::Group& group)
    {
        std::shared_ptr<uff::Excitation> excitation = std::make_shared<uff::Excitation>();

        // "pulse_shape"
        excitation->setPulseShape(readOptionalStringDataset(group, "pulse_shape"));

        // "transmit_frequency"
        excitation->setTransmitFrequency(readOptionalDoubleDataset(group, "transmit_frequency"));

        // "sampling_frequency"
        excitation->setSamplingFrequency(readOptionalDoubleDataset(group, "sampling_frequency"));

        // TODO: Waveform
        
        // Add unique excitation to acquisition only if it doesn't exist
        bool found = false;
        for (size_t iExcitation = 0; iExcitation < m_dataset->acquisition().uniqueExcitations().size() && !found; iExcitation++) 
        { 
            found = *excitation.get() == *m_dataset->acquisition().uniqueExcitations()[iExcitation].get();

            if (found) excitation = m_dataset->acquisition().uniqueExcitations()[iExcitation];
        }
        if(!found)m_dataset->acquisition().addUniqueExcitation(excitation);

        return excitation;
    }

    void ReaderV0_2::readIntegerArrayDataset(const H5::Group& group, const std::string& name,
        std::vector<int>& values, std::vector<size_t>& dimensions)
    {
        H5::DataSet dataset = group.openDataSet(name);
        // TODO: check if type is correct : dataset.getTypeClass()
        H5::StrType datatype(H5::PredType::NATIVE_INT);

        // find dataset dimensions
        H5::DataSpace dataspace = dataset.getSpace();
        int ndims = dataspace.getSimpleExtentNdims();
        dimensions.resize(ndims);
        dataspace.getSimpleExtentDims((hsize_t*)dimensions.data()); // NOTE: Will bug on architectures whose (sizeof(hsize_t != sizeof(long long unsigned int))) cf. the static_assert upper

        size_t numel = 1;
        for (auto sz : dimensions) { numel *= sz; }

        // reserve space in the output buffer
        values.resize(numel);

        // read data
        dataset.read(values.data(), datatype);
    }

    int ReaderV0_2::readIntegerDataset(const H5::Group& group, const std::string& name)
    {
        H5::StrType datatype(H5::PredType::NATIVE_INT);
        H5::DataSet dataset = group.openDataSet(name);
        int value;
        dataset.read(&value, datatype);
        dataset.close();
        return value;
    }

    std::shared_ptr<uff::LinearArray> ReaderV0_2::readLinearArray(const H5::Group& group)
    {
        auto linearArray = std::make_shared<uff::LinearArray>(readIntegerDataset(group, "number_elements"));

        // Read "pitch"
        linearArray->setPitch(readOptionalDoubleDataset(group, "pitch"));

        // Read "element_width"
        linearArray->setElementWidth(readOptionalDoubleDataset(group, "element_width"));

        // Read "element_height"
        linearArray->setElementHeight(readOptionalDoubleDataset(group, "element_height"));

        return linearArray;
    }

    std::shared_ptr<uff::MatrixArray> ReaderV0_2::readMatrixArray(const H5::Group& group)
    {
        auto matrixArray = std::make_shared<uff::MatrixArray>();

        // Read "number_elements"
        matrixArray->setNumberElementsX(readIntegerDataset(group, "number_elements_x"));
        matrixArray->setNumberElementsY(readIntegerDataset(group, "number_elements_y"));

        // Read "pitch"
        matrixArray->setPitchX(readOptionalDoubleDataset(group, "pitch_x"));
        matrixArray->setPitchY(readOptionalDoubleDataset(group, "pitch_y"));

        // Read "element_width"
        matrixArray->setElementWidth(readOptionalDoubleDataset(group, "element_width"));

        // Read "element_height"
        matrixArray->setElementHeight(readOptionalDoubleDataset(group, "element_height"));

        return matrixArray;
    }

    std::shared_ptr<RcaArray> ReaderV0_2::readRcaArray(const H5::Group& group)
    {
        auto rcaArray = std::make_shared<uff::RcaArray>(readIntegerDataset(group, "number_elements_x"), readIntegerDataset(group, "number_elements_y"));

        // Read "pitch"
        rcaArray->setPitchX(readOptionalDoubleDataset(group, "pitch_x"));
        rcaArray->setPitchY(readOptionalDoubleDataset(group, "pitch_y"));

        // Read "element_width"
        rcaArray->setElementWidthX(readOptionalDoubleDataset(group, "element_width_x"));
        rcaArray->setElementWidthY(readOptionalDoubleDataset(group, "element_width_y"));

        // Read "element_height"
        rcaArray->setElementHeightX(readOptionalDoubleDataset(group, "element_height_x"));
        rcaArray->setElementHeightY(readOptionalDoubleDataset(group, "element_height_y"));

        return rcaArray;
    }

    std::shared_ptr<uff::Probe> ReaderV0_2::readProbe(const H5::Group& group)
    {
        auto probe = std::make_shared<uff::Probe>();

        // transform
        H5::Group transform = group.openGroup("transform");
        probe->setTransform(readTransform(transform));

        // focal length
        probe->setFocalLength(readOptionalDoubleDataset(group, "focal_length"));

        // elements
        H5::Group elements = group.openGroup("elements");
        probe->setElements(readArray<uff::Element>(elements));

        // element geometries (TODO)

        // impulse responses (TODO)

        // probe_type (optional)
        if (H5Lexists(group.getLocId(), "probe_type", H5P_DEFAULT))
        {
            std::string probeType = readStringDataset(group, "probe_type");
            if (probeType == "LinearArray")
            {
                auto linearArray = readLinearArray(group);
                linearArray->setTransform(probe->transform());
                linearArray->setFocalLength(probe->focalLength());
                linearArray->setElements(probe->elements());
                linearArray->setElementGeometries(probe->elementGeometries());
                linearArray->setImpulseResponses(probe->impulseResponses());
                return linearArray;
            }
            else if (probeType == "MatrixArray")
            {
                auto matrixArray = readMatrixArray(group);
                matrixArray->setTransform(probe->transform());
                matrixArray->setFocalLength(probe->focalLength());
                matrixArray->setElements(probe->elements());
                matrixArray->setElementGeometries(probe->elementGeometries());
                matrixArray->setImpulseResponses(probe->impulseResponses());
                return matrixArray;
            }
            else if (probeType == "RcaArray")
            {
                auto rcaArray = readRcaArray(group);
                rcaArray->setTransform(probe->transform());
                rcaArray->setFocalLength(probe->focalLength());
                rcaArray->setElements(probe->elements());
                rcaArray->setElementGeometries(probe->elementGeometries());
                rcaArray->setImpulseResponses(probe->impulseResponses());
                return rcaArray;
            }
            else
            {
                std::cerr << getClassNameInternal() << ": Ignoring unknown probe_type:" << probeType << std::endl;
                return probe;
            }
        }

        return probe;
    }

    uff::ReceiveSetup ReaderV0_2::readReceiveSetup(const H5::Group& group)
    {
        uff::ReceiveSetup receiveSetup;

        // "probe"
        int probeId = std::stoi(readStringDataset(group, "probe_id"));
        receiveSetup.setProbe(m_dataset->acquisition().probes()[(size_t)probeId - 1]);

        // "time_offset"
        receiveSetup.setTimeOffset(readDoubleDataset(group, "time_offset"));

        // "sampling_frequency"
        receiveSetup.setSamplingFrequency(readDoubleDataset(group, "sampling_frequency"));

        // "sampling_type"
        int st = readIntegerDataset(group, "sampling_type");
        switch (st)
        {
        case (int)uff::ReceiveSetup::SAMPLING_TYPE::DIRECT_RF:
            receiveSetup.setSamplingType(uff::ReceiveSetup::SAMPLING_TYPE::DIRECT_RF);
            break;
        case (int)uff::ReceiveSetup::SAMPLING_TYPE::IQ:
            receiveSetup.setSamplingType(uff::ReceiveSetup::SAMPLING_TYPE::IQ);
            break;
        case (int)uff::ReceiveSetup::SAMPLING_TYPE::QUADRATURE_4X_F0:
            receiveSetup.setSamplingType(uff::ReceiveSetup::SAMPLING_TYPE::QUADRATURE_4X_F0);
            break;
        case (int)uff::ReceiveSetup::SAMPLING_TYPE::QUADRATURE_2X_F0:
            receiveSetup.setSamplingType(uff::ReceiveSetup::SAMPLING_TYPE::QUADRATURE_2X_F0);
            break;
        default:
            std::cerr << "Unknow sampling type:" << st;
        }

        // channel_mapping [optional]
        if (H5Lexists(group.getLocId(), "channel_mapping", H5P_DEFAULT))
        {
            std::vector<int> channelMapping;
            std::vector<size_t> dimensions;
            readIntegerArrayDataset(group, "channel_mapping", channelMapping, dimensions);
            receiveSetup.setChannelMapping(channelMapping);
        }

        // "tgc_profile" [optional]
        if (H5Lexists(group.getLocId(), "tgc_profile", H5P_DEFAULT))
        {
            std::vector<float> tgcProfile;
            std::vector<size_t> dimensions;
            readArrayDataset(group, "tgc_profile", tgcProfile, dimensions);
            receiveSetup.setTgcProfile(tgcProfile);
        }

        // "tgc_sampling_frequency" [optional]
        if (H5Lexists(group.getLocId(), "tgc_sampling_frequency", H5P_DEFAULT))
        {
            receiveSetup.setTgcSamplingFrequency(readOptionalDoubleDataset(group, "tgc_sampling_frequency"));
        }

        // "modulation_frequency" [optional]
        if (H5Lexists(group.getLocId(), "tgc_sampling_frequency", H5P_DEFAULT))
        {
            receiveSetup.setModulationFrequency(readOptionalDoubleDataset(group, "modulation_frequency"));
        }

        return receiveSetup;
    }

    uff::Rotation ReaderV0_2::readRotation(const H5::Group& group)
    {
        uff::Rotation rotation;

        rotation.setX(readDoubleDataset(group, "x"));
        rotation.setY(readDoubleDataset(group, "y"));
        rotation.setZ(readDoubleDataset(group, "z"));

        return rotation;
    }

    std::string ReaderV0_2::readStringDataset(const H5::Group& group, const std::string& name)
    {
        H5::StrType datatype(0, H5T_VARIABLE);
        H5::DataSpace dataspace(H5S_SCALAR);
        H5::DataSet dataset = group.openDataSet(name);
        std::string buffer;
        dataset.read(buffer, datatype, dataspace);
        return buffer;
    }

    std::optional<std::string> ReaderV0_2::readOptionalStringDataset(const H5::Group& group, const std::string& name)
    {
        H5::StrType datatype(0, H5T_VARIABLE);
        H5::DataSpace dataspace(H5S_SCALAR);
        H5::DataSet dataset = group.openDataSet(name);
        std::string buffer;
        dataset.read(buffer, datatype, dataspace);
        std::optional<std::string> result = std::nullopt;
        if (buffer != "undefined")
            result = buffer;
        return result;
    }

    uff::TimedEvent ReaderV0_2::readTimedEvent(const H5::Group& group)
    {
        uff::TimedEvent timedEvent;

        // "event"
        int eventId = std::stoi(readStringDataset(group, "event_id"));
        timedEvent.setEvent(m_dataset->acquisition().uniqueEvents()[(size_t)eventId - 1]);

        // "time_offset"
        timedEvent.setTimeOffset(readDoubleDataset(group, "time_offset"));

        return timedEvent;
    }

    uff::Transform ReaderV0_2::readTransform(const H5::Group& group)
    {
        // rotation
        uff::Rotation rotation = readRotation(group.openGroup("rotation"));

        // translation
        uff::Translation translation = readTranslation(group.openGroup("translation"));

        return uff::Transform(rotation, translation);
    }

    uff::Translation ReaderV0_2::readTranslation(const H5::Group& group)
    {
        uff::Translation translation;

        translation.setX(readDoubleDataset(group, "x"));
        translation.setY(readDoubleDataset(group, "y"));
        translation.setZ(readDoubleDataset(group, "z"));

        return translation;
    }
    
    uff::TransmitSetup ReaderV0_2::readTransmitSetup(const H5::Group& group)
    {
        uff::TransmitSetup transmitSetup;

        // "probe"
        int probeId = std::stoi(readStringDataset(group, "probe_id"));
        transmitSetup.setProbe(m_dataset->acquisition().probes()[(size_t)probeId - 1]);

        // "transmit_wave"
        H5::Group transmitWave(group.openGroup("transmit_wave"));

        // Wave ID (found in transmit wave)
        int waveId = std::stoi(readStringDataset(transmitWave, "wave_id"));
        transmitSetup.setWave(m_dataset->acquisition().uniqueWaves()[(size_t)waveId - 1]);

        // Time offset is in transmit wave class
        transmitSetup.setTimeOffset(readDoubleDataset(transmitWave, "time_offset"));

        // channel_mapping [optional]
        if (H5Lexists(group.getLocId(), "channel_mapping", H5P_DEFAULT))
        {
            std::vector<int> channelMapping;
            std::vector<size_t> dimensions;
            readIntegerArrayDataset(group, "channel_mapping", channelMapping, dimensions);
            transmitSetup.wave().lock()->setChannelMapping(channelMapping);
        }

        return transmitSetup;
    }

    void ReaderV0_2::readVersion(const H5::Group& group)
    {
        int major = readIntegerDataset(group, "major");
        int minor = readIntegerDataset(group, "minor");
        int patch = readIntegerDataset(group, "patch");
        uff::Version version(major, minor, patch);
        m_dataset->setVersion(version);
    }

    std::shared_ptr<uff::Wave> ReaderV0_2::readWave(const H5::Group& group)
    {
        auto wave = std::make_shared<uff::Wave>();

        // write "origin"
        wave->setOrigin(readTransform(group.openGroup("origin")));

        // write "wave_type"
        const int waveType = readIntegerDataset(group, "wave_type");
        wave->setWaveType((uff::WaveType)waveType);
        if ((int)wave->waveType() != waveType) { std::cerr << "Unknow sampling type:" << waveType; }

        // write "aperture"
        wave->setAperture(readAperture(group.openGroup("aperture")));

        // write "excitation"
        wave->setExcitation(readExcitation(group.openGroup("excitation")));

        return wave;
    }

} // namespace uff
