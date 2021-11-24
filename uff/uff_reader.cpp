/*!
 * Copyright Moduleus
 * \file uff_reader.cpp
 * \brief
 */

#include "uff_reader.h"

#include <type_traits>
#include <limits>

namespace uff
{

    void Reader::printSelf(std::ostream& os, std::string indent) const { superclass::printSelf(os, indent); }

    void Reader::updateMetadata()
    {
        m_dataset = std::make_shared<uff::Dataset>();
        try
        {
            H5::Exception::dontPrint();
            H5::H5File file(m_fileName, H5F_ACC_RDONLY);

            // Version
            H5::Group version(file.openGroup("version"));
            readVersion(version);

            // Channel Data
            H5::Group acquisition(file.openGroup("acquisition"));
            readAcquisition(acquisition);

            file.close();
        }  // end of try block
        // catch failure caused by the H5File operations
        catch (H5::FileIException error)
        {
            error.printErrorStack();
            std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
            return;
        }
        // catch failure caused by the DataSet operations
        catch (H5::DataSetIException error)
        {
            error.printErrorStack();
            std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
            return;
        }
        // catch failure caused by the DataSpace operations
        catch (H5::DataSpaceIException error)
        {
            error.printErrorStack();
            std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
            return;
        }
        // catch failure caused by the DataSpace operations
        catch (H5::DataTypeIException error)
        {
            error.printErrorStack();
            std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
            return;
        }
        // catch failure caused by the Group operations
        catch (H5::GroupIException error)
        {
            error.printErrorStack();
            std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
            return;
        }
    }

    void Reader::readAcquisition(const H5::Group& group)
    {
        uff::Acquisition& acquisition = m_dataset->acquisition();
        acquisition.setAuthors(readStringDataset(group, "authors"));
        acquisition.setDescription(readStringDataset(group, "description"));
        acquisition.setLocalTime(readStringDataset(group, "local_time"));
        acquisition.setCountryCode(readStringDataset(group, "country_code"));
        acquisition.setSystem(readStringDataset(group, "system"));
        acquisition.setSoundSpeed(readDoubleDataset(group, "sound_speed"));
        acquisition.setTimeOffset(readDoubleDataset(group, "time_offset"));

        // Group Links
        H5::Group groupLinks(group.openGroup("group_links"));
        acquisition.setGroupLink(readArray<std::shared_ptr<GroupLink>>(groupLinks));

        // Groups
        H5::Group groups(group.openGroup("groups"));
        acquisition.setGroups(readArray<std::shared_ptr<IGroup>>(groups));

        // Probes
        H5::Group probes(group.openGroup("probes"));
        acquisition.setProbes(readArray<std::shared_ptr<Probe>>(probes));

        // Unique events
        H5::Group uniqueEvents(group.openGroup("unique_events"));
        acquisition.setUniqueEvents(readArray<std::shared_ptr<Event>>(uniqueEvents));

        // Unique waves
        H5::Group waves(group.openGroup("unique_waves"));
        acquisition.setUniqueWaves(readArray<std::shared_ptr<Wave>>(waves));

        // Excitations
        H5::Group excitation(group.openGroup("excitations"));
        acquisition.setUniqueExcitations(readArray<std::shared_ptr<Excitation>>(excitation));

        // Group data
        H5::Group groupData(group.openGroup("group_data"));
        acquisition.setGroupData(readArray<std::shared_ptr<GroupData>>(groupData));

        // Initial Group
        int initialGroupId = std::stoi(readStringDataset(group, "initial_group"));
        acquisition.setInitialGroup(acquisition.groups()[(size_t)initialGroupId - 1]);
    }

    void Reader::readVersion(const H5::Group& group)
    {
        int major = readIntegerDataset(group, "major");
        int minor = readIntegerDataset(group, "minor");
        int patch = readIntegerDataset(group, "patch");
        uff::Version version(major, minor, patch);
        m_dataset->setVersion(version);
    }

    std::shared_ptr<uff::GroupLink> Reader::readGroupLink(const H5::Group& group)
    {
        std::shared_ptr<uff::GroupLink> groupLink = std::make_shared<uff::GroupLink>();

        // Source 
        int idSource = stoi(readStringDataset(group, "source_id"));
        groupLink->setSource(m_dataset->acquisition().groups()[idSource - 1]);

        // Destination
        int idDestination = stoi(readStringDataset(group, "source_id"));
        groupLink->setDestination(m_dataset->acquisition().groups()[idDestination - 1]);

        return groupLink;
    }

    std::shared_ptr<uff::IGroup> Reader::readIGroup(const H5::Group& group)
    {
        std::shared_ptr<uff::IGroup> iGroup;

        // If this IGroup has initial_group attribute then it's a super group else just a group.
        if (H5Lexists(group.getLocId(), "initial_group", H5P_DEFAULT)) { iGroup = readSuperGroup(group); }
        else { iGroup = readGroup(group); }

        // description
        iGroup->setDescription(readStringDataset(group, "description"));

        // time_offset
        iGroup->setTimeOffset(readDoubleDataset(group, "time_offset"));

        // repetition_count
        iGroup->setRepetitionCount((uint32_t)readIntegerDataset(group, "repetition_count"));

        return iGroup;
    }

    std::shared_ptr<uff::SuperGroup> Reader::readSuperGroup(const H5::Group& group)
    {
        std::shared_ptr<uff::SuperGroup> superGroup = std::make_shared<uff::SuperGroup>();

        // initial_group
        int idInitialGroup = stoi(readStringDataset(group, "initial_group"));
        std::shared_ptr<IGroup> initialIGroup = m_dataset->acquisition().groups()[(size_t)idInitialGroup - 1];
        std::weak_ptr<Group> initialGroup = std::dynamic_pointer_cast<Group>(initialIGroup);
        superGroup->setInitialGroup(initialGroup);

        return superGroup;
    }

    std::shared_ptr<uff::Group> Reader::readGroup(const H5::Group& group)
    {
        std::shared_ptr<uff::Group> groupUff = std::make_shared<uff::Group>();

        // repetition_rate
        groupUff->setRepetitionRate(readDoubleDataset(group, "repetition_rate"));

        // sequence
        H5::Group sequence(group.openGroup("sequence"));
        groupUff->setSequence(readSequence(sequence));

        return groupUff;
    }

    std::shared_ptr<uff::GroupData> Reader::readGroupData(const H5::Group& group)
    {
        std::shared_ptr<uff::GroupData> groupData = std::make_shared<uff::GroupData>();

        // group
        int idGroup = stoi(readStringDataset(group, "group"));
        std::shared_ptr<IGroup> iGroupConcerned = m_dataset->acquisition().groups()[(size_t)idGroup - 1];
        std::weak_ptr<Group> groupConcerned = std::dynamic_pointer_cast<Group>(iGroupConcerned);
        groupData->setGroup(groupConcerned);

        // data
        std::vector<size_t> dataDims;
        readArrayDataset(group, "data", groupData->data(), dataDims);
        if (dataDims.size() != 4) { std::cerr << "Uff::Reader : Dataset dimension != 4" << std::endl; }

        return groupData;
    }

    uff::Element Reader::readElement(const H5::Group& group)
    {
        uff::Element element;

        element.setX(readOptionalDoubleDataset(group, "x"));
        element.setY(readOptionalDoubleDataset(group, "y"));
        element.setZ(readOptionalDoubleDataset(group, "z"));

        return element;
    }

    std::shared_ptr<uff::Event> Reader::readEvent(const H5::Group& group)
    {
        auto event = std::make_shared<uff::Event>();

        // "receive_setup"
        event->setReceiveSetup(readReceiveSetup(group.openGroup("receive_setup")));

        // "transmit_setup"
        event->setTransmitSetup(readTransmitSetup(group.openGroup("transmit_setup")));

        return event;
    }

    std::shared_ptr<uff::Excitation> Reader::readExcitation(const H5::Group& group)
    {
        std::shared_ptr<uff::Excitation> excitation = std::make_shared<uff::Excitation>();

        // "pulse_shape"
        excitation->setPulseShape(readOptionalStringDataset(group, "pulse_shape"));

        // "transmit_frequency"
        excitation->setTransmitFrequency(readOptionalDoubleDataset(group, "transmit_frequency"));

        // "sampling_frequency"
        excitation->setSamplingFrequency(readOptionalDoubleDataset(group, "sampling_frequency"));

        return excitation;
    }

std::shared_ptr<uff::LinearArray> Reader::readLinearArray(const H5::Group& group)
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

std::shared_ptr<uff::MatrixArray> Reader::readMatrixArray(const H5::Group& group)
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

std::shared_ptr<RcaArray> Reader::readRcaArray(const H5::Group&group)
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

std::shared_ptr<uff::Probe> Reader::readProbe(const H5::Group& group)
{
    auto probe = std::make_shared<uff::Probe>();

    // transform
    H5::Group transform = group.openGroup("transform");
    probe->setTransform(readTransform(transform));

    // focal length
    probe->setFocalLength(readOptionalDoubleDataset(group, "focal_length"));

    // elements
    H5::Group elements = group.openGroup("elements");
    probe->setElements(readArray<Element>(elements));

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
        else if(probeType == "RcaArray")
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

uff::Sequence Reader::readSequence(const H5::Group& group)
{
    uff::Sequence sequence;

    // time_offset
    sequence.setTimeOffset(readDoubleDataset(group, "time_offset"));

    // timed_events
    H5::Group timedEvents(group.openGroup("timed_events"));
    sequence.setTimedEvents(readArray<TimedEvent>(timedEvents));

    return sequence;
}

uff::ReceiveSetup Reader::readReceiveSetup(const H5::Group& group)
{
    uff::ReceiveSetup receiveSetup;

    // "probe"
    int probeId = std::stoi(readStringDataset(group, "probe_id"));
    receiveSetup.setProbe(m_dataset->acquisition().probes()[(size_t)probeId - 1]);

    // "time_offset"
    receiveSetup.setTimeOffset(readDoubleDataset(group, "time_offset"));

    // "sampling_frequency"
    receiveSetup.setSamplingFrequency(readDoubleDataset(group, "sampling_frequency"));

    // "nb_samples"
    receiveSetup.setNumberOfSamples(readIntegerDataset(group, "nb_samples"));

    // "sampling_type"
    int st = readIntegerDataset(group, "sampling_type");
    switch (st)
    {
    case uff::ReceiveSetup::SAMPLING_TYPE::DIRECT_RF:
        receiveSetup.setSamplingType(uff::ReceiveSetup::SAMPLING_TYPE::DIRECT_RF);
        break;
    case uff::ReceiveSetup::SAMPLING_TYPE::IQ:
        receiveSetup.setSamplingType(uff::ReceiveSetup::SAMPLING_TYPE::IQ);
        break;
    case uff::ReceiveSetup::SAMPLING_TYPE::QUADRATURE_4X_F0:
        receiveSetup.setSamplingType(uff::ReceiveSetup::SAMPLING_TYPE::QUADRATURE_4X_F0);
        break;
    case uff::ReceiveSetup::SAMPLING_TYPE::QUADRATURE_2X_F0:
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

uff::Rotation Reader::readRotation(const H5::Group& group)
{
    uff::Rotation rotation;

    rotation.setX(readDoubleDataset(group, "x"));
    rotation.setY(readDoubleDataset(group, "y"));
    rotation.setZ(readDoubleDataset(group, "z"));

    return rotation;
}

uff::TimedEvent Reader::readTimedEvent(const H5::Group& group)
{
    uff::TimedEvent timedEvent;

    // "event"
    int eventId = std::stoi(readStringDataset(group, "event_id"));
    timedEvent.setEvent(m_dataset->acquisition().uniqueEvents()[(size_t)eventId - 1]);

    // "time_offset"
    timedEvent.setTimeOffset(readDoubleDataset(group, "time_offset"));

    return timedEvent;
}

uff::Transform Reader::readTransform(const H5::Group& group)
{
    // rotation
    uff::Rotation rotation = readRotation(group.openGroup("rotation"));

    // translation
    uff::Translation translation = readTranslation(group.openGroup("translation"));

    return uff::Transform(rotation, translation);
}

uff::Translation Reader::readTranslation(const H5::Group& group)
{
    uff::Translation translation;

    translation.setX(readDoubleDataset(group, "x"));
    translation.setY(readDoubleDataset(group, "y"));
    translation.setZ(readDoubleDataset(group, "z"));

    return translation;
}

uff::TransmitSetup Reader::readTransmitSetup(const H5::Group& group)
{
    uff::TransmitSetup transmitSetup;

    // "probe"
    int probeId = std::stoi(readStringDataset(group, "probe_id"));
    transmitSetup.setProbe(m_dataset->acquisition().probes()[(size_t)probeId - 1]);

    // time_offset
    transmitSetup.setTimeOffset(readDoubleDataset(group, "time_offset"));

    // "transmit_wave"
    int waveId = std::stoi(readStringDataset(group, "wave_id"));
    transmitSetup.setWave(m_dataset->acquisition().uniqueWaves()[(size_t)waveId - 1]);

    return transmitSetup;
}

std::shared_ptr<uff::Wave> Reader::readWave(const H5::Group& group)
{
    auto wave = std::make_shared<uff::Wave>();

    // "origin"
    wave->setOrigin(readTransform(group.openGroup("origin")));

    // "wave_type"
    switch (readIntegerDataset(group, "wave_type"))
    {
    case (int)uff::WaveType::CONVERGING_WAVE:
        wave->setWaveType(uff::WaveType::CONVERGING_WAVE);
        break;
    case (int)uff::WaveType::CYLINDRICAL_WAVE:
        wave->setWaveType(uff::WaveType::CYLINDRICAL_WAVE);
        break;
    case (int)uff::WaveType::DIVERGING_WAVE:
        wave->setWaveType(uff::WaveType::DIVERGING_WAVE);
        break;
    case (int)uff::WaveType::PLANE_WAVE:
        wave->setWaveType(uff::WaveType::PLANE_WAVE);
        break;
    default:
        // TODO: unknown wave type ?!?
        break;
    }

    // "aperture"
    wave->setAperture(readAperture(group.openGroup("aperture")));

    // channel_mapping [optional]
    if (H5Lexists(group.getLocId(), "channel_mapping", H5P_DEFAULT))
    {
        std::vector<int> channelMapping;
        std::vector<size_t> dimensions;
        readIntegerArrayDataset(group, "channel_mapping", channelMapping, dimensions);
        wave->setChannelMapping(channelMapping);
    }

    // "excitation"
    int excitationId = stoi(readStringDataset(group, "excitation_id"));
    wave->setExcitation(m_dataset->acquisition().uniqueExcitations()[(size_t)excitationId - 1]);

    return wave;
}

    uff::Aperture Reader::readAperture(const H5::Group& group)
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

    double Reader::readDoubleDataset(const H5::Group& group, const std::string& name)
    {
        H5::StrType datatype(H5::PredType::NATIVE_DOUBLE);
        H5::DataSet dataset = group.openDataSet(name);
        double value;
        dataset.read(&value, datatype);
        dataset.close();
        return value;
    }

    std::optional<double> Reader::readOptionalDoubleDataset(const H5::Group& group, const std::string& name)
    {
        H5::StrType datatype(H5::PredType::NATIVE_DOUBLE);
        H5::DataSet dataset = group.openDataSet(name);
        double value;
        dataset.read(&value, datatype);
        dataset.close();
        std::optional<double> result = std::nullopt;
        if (!std::isnan(value)) { result = value; }
        return result;
    }

    void Reader::readIntegerArrayDataset(const H5::Group& group, const std::string& name,
        std::vector<int>& values, std::vector<size_t>& dimensions)
    {
        H5::DataSet dataset = group.openDataSet(name);
        // TODO: check if type is correct : dataset.getTypeClass()
        H5::StrType datatype(H5::PredType::NATIVE_INT);

        // find dataset dimensions
        H5::DataSpace dataspace = dataset.getSpace();
        int ndims = dataspace.getSimpleExtentNdims();
        //std::cout << "ndims:" << ndims << std::endl;
        dimensions.resize(ndims);
        dataspace.getSimpleExtentDims((hsize_t*)dimensions.data()); // NOTE: Will bug on architectures whose (sizeof(hsize_t != sizeof(long long unsigned int))) cf. the static_assert upper
        size_t numel = 1;
        for (auto sz : dimensions) { numel *= sz; }

        // reserve space in the output buffer
        values.resize(numel);

        // read data
        dataset.read(values.data(), datatype);
    }

    int Reader::readIntegerDataset(const H5::Group& group, const std::string& name)
    {
        H5::StrType datatype(H5::PredType::NATIVE_INT);
        H5::DataSet dataset = group.openDataSet(name);
        int value;
        dataset.read(&value, datatype);
        dataset.close();
        return value;
    }

    std::string Reader::readStringDataset(const H5::Group& group, const std::string& name)
    {
        H5::StrType datatype(0, H5T_VARIABLE);
        H5::DataSpace dataspace(H5S_SCALAR);
        H5::DataSet dataset = group.openDataSet(name);
        std::string buffer;
        dataset.read(buffer, datatype, dataspace);
        return buffer;
    }

    std::optional<std::string> Reader::readOptionalStringDataset(const H5::Group& group, const std::string& name)
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

} // namespace uff
