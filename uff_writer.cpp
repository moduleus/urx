/*!
 * Copyright Moduleus
 * \file uff_writer.cpp
 * \brief
 */

#include "uff_writer.h"

namespace uff
{

void Writer::printSelf(std::ostream& os, std::string indent) const
{
    superclass::printSelf(os, indent);
    os << indent << "HDF5 Version: " << "TODO" << std::endl
        << indent << "UFF Version: " << "TODO" << std::endl
        << indent << "FileName: " << this->m_fileName << std::endl
        << indent << "Dataset: " << this->m_dataset << std::endl;
}

bool Writer::writeToFile()
{
    try
    {
        H5::Exception::dontPrint();

        H5::H5File file(m_fileName, H5F_ACC_TRUNC);

        // Version
        H5::Group version(file.createGroup("version"));
        writeVersion(version, m_dataset.version());

        // Channel Data
        H5::Group channelData(file.createGroup("channel_data"));
        writeAcquisition(channelData, m_dataset.acquisition());

        file.close();

        return true;
    }  // end of try block
    // catch failure caused by the H5File operations
    catch (H5::FileIException error)
    {
        error.printErrorStack();
        std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
        return false;
    }
    // catch failure caused by the DataSet operations
    catch (H5::DataSetIException error)
    {
        error.printErrorStack();
        std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
        return false;
    }
    // catch failure caused by the DataSpace operations
    catch (H5::DataSpaceIException error)
    {
        error.printErrorStack();
        std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
        return false;
    }
    // catch failure caused by the DataSpace operations
    catch (H5::DataTypeIException error)
    {
        error.printErrorStack();
        std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
        return false;
    }
    // catch failure caused by the Group operations
    catch (H5::GroupIException error)
    {
        error.printErrorStack();
        std::cerr << __FILE__ << __LINE__ << error.getDetailMsg();
        return false;
    }
}

void Writer::writeAcquisition(H5::Group& group, const uff::Acquisition& acquisition)
{
    // acquisition.authors
    writeStringDataset(group, "authors", acquisition.authors());

    // acquisition.description
    writeStringDataset(group, "description", acquisition.description());

    // acquisition.local_time
    writeStringDataset(group, "local_time", acquisition.localTime());

    // acquisition.country_code
    writeStringDataset(group, "country_code", acquisition.countryCode());

    // acquisition.system
    writeStringDataset(group, "system", acquisition.system());

    // acquisition.sound_speed
    writeDoubleDataset(group, "sound_speed", acquisition.soundSpeed());

    // acquisition.time_offset
    writeDoubleDataset(group, "time_offset", acquisition.timeOffset());

    // acquisition.initial_group
    std::string intialGroupId = getIdFromPointer<uff::IGroup>(acquisition.groups(), acquisition.initialGroup());
    writeStringDataset(group, "initial_group", intialGroupId);

    // Group links
    H5::Group groupLinks(group.createGroup("group_links"));
    writeGroupLinkArray(groupLinks, acquisition.groupLinks());

    // Groups
    H5::Group groups(group.createGroup("groups"));
    writeGroupArray(groups, acquisition.groups());

    // Probes
    H5::Group probes(group.createGroup("probes"));
    writeProbeArray(probes, acquisition.probes());

    // Unique events
    H5::Group uniqueEvents(group.createGroup("unique_events"));
    writeEventArray(uniqueEvents, acquisition.uniqueEvents());

    // Unique waves
    H5::Group waves(group.createGroup("unique_waves"));
    writeWaveArray(waves, acquisition.uniqueWaves());

    // Unique Excitations
    H5::Group uniqueExcitations(group.createGroup("unique_excitations"));
    writeExcitationArray(uniqueExcitations, acquisition.uniqueExcitations());

    // Group Data
    H5::Group groupData(group.createGroup("group_data"));
    writeGroupDataArray(groupData, acquisition.groupData());
}

void Writer::writeGroupLinkArray(H5::Group& group, const std::vector<std::shared_ptr<GroupLink>>& groupLinks)
{
    char buf[9];
    snprintf(buf, sizeof buf, "%08d", 0);
    for (int i = 0; i < groupLinks.size(); i++)
    {
        snprintf(buf, sizeof buf, "%08d", i + 1);
        std::string groupDataId = buf;
        H5::Group hdf5Group = group.createGroup(groupDataId);
        writeGroupLink(hdf5Group, groupLinks[i]);
    }
}

void Writer::writeGroupLink(H5::Group& group, const std::shared_ptr<GroupLink>& groupLink)
{
    // source
    std::string idGroupSource = getIdFromPointer<IGroup>(m_dataset.acquisition().groups(), groupLink->source());
    writeStringDataset(group, "source", idGroupSource);

    // destination
    std::string idGroupDestination = getIdFromPointer<IGroup>(m_dataset.acquisition().groups(), groupLink->destination());
    writeStringDataset(group, "destination", idGroupDestination);
}

void Writer::writeGroupDataArray(H5::Group& group, const std::vector<std::shared_ptr<GroupData>>& groupData)
{
    char buf[9];
    snprintf(buf, sizeof buf, "%08d", 0);
    for (int i = 0; i < groupData.size(); i++)
    {
        snprintf(buf, sizeof buf, "%08d", i + 1);
        std::string id = buf;
        H5::Group hdf5Group = group.createGroup(id);
        writeGroupData(hdf5Group, groupData[i]);
    }
}

void Writer::writeGroupData(H5::Group& group, const std::shared_ptr<GroupData>& groupData)
{
    // group
    std::string idGroup = getIdFromPointer<IGroup>(m_dataset.acquisition().groups(), groupData->group());
    writeStringDataset(group, "group", idGroup);

    // data
    todo
}

void Writer::writeGroupArray(H5::Group& group, const std::vector<std::shared_ptr<IGroup>>& igroup)
{
    char buf[9];
    snprintf(buf, sizeof buf, "%08d", 0);
    for (int i = 0; i < igroup.size(); i++)
    {
        snprintf(buf, sizeof buf, "%08d", i + 1);
        std::string id = buf;
        H5::Group hdf5Group = group.createGroup(id);

        if (Group* group = dynamic_cast<Group*>(igroup[i].get()))                       { writeGroup(hdf5Group, *group); }
        else if (SuperGroup* superGroup = dynamic_cast<SuperGroup*>(igroup[i].get()))   { writeSuperGroup(hdf5Group, *superGroup); }
        else { assert(false); }
    }
}

void Writer::writeElement(H5::Group& group, const uff::Element& element)
{
    writeOptionalDoubleDataset(group, "x", element.x());
    writeOptionalDoubleDataset(group, "y", element.y());
    writeOptionalDoubleDataset(group, "z", element.z());
}

void Writer::writeElementArray(H5::Group& group, const std::vector<uff::Element>& elements)
{
    char buf[9];
    snprintf(buf, sizeof buf, "%08d", 0);
    for (int i = 0; i < elements.size(); i++)
    {
        snprintf(buf, sizeof buf, "%08d", i + 1);
        std::string element_id = buf;
        H5::Group element = group.createGroup(element_id);
        writeElement(element, elements[i]);
    }
}

void Writer::writeEvent(H5::Group& group, const std::shared_ptr<uff::Event>& ev)
{
    // "transmit_setup"
    H5::Group transmitSetup = group.createGroup("transmit_setup");
    writeTransmitSetup(transmitSetup, ev->transmitSetup());

    // "receive_setup"
    H5::Group receiveSetup = group.createGroup("receive_setup");
    writeReceiveSetup(receiveSetup, ev->receiveSetup());
}

void Writer::writeEventArray(H5::Group& group, const std::vector<std::shared_ptr<uff::Event>>& events)
{
    char buf[9];
    snprintf(buf, sizeof buf, "%08d", 0);
    for (int i = 0; i < events.size(); i++)
    {
        snprintf(buf, sizeof buf, "%08d", i + 1);
        std::string event_id = buf;
        H5::Group ev = group.createGroup(event_id);
        writeEvent(ev, events[i]);
    }
}

void Writer::writeLinearArray(H5::Group& group, const std::shared_ptr<uff::LinearArray>& linearArray)
{
    // Write "number_elements"
    writeIntegerDataset(group, "number_elements", linearArray->numberElements());

    // Write "pitch"
    writeOptionalDoubleDataset(group, "pitch", linearArray->pitch());

    // Write "element_width"
    writeOptionalDoubleDataset(group, "element_width", linearArray->elementWidth());

    // Write "element_height"
    writeOptionalDoubleDataset(group, "element_height", linearArray->elementHeight());
}

void Writer::writeMatrixArray(H5::Group& group, const std::shared_ptr<uff::MatrixArray>& matrixArray)
{
    // Write "number_elements_x"
    writeIntegerDataset(group, "number_elements_x", matrixArray->numberElementsX());

    // Write "number_elements_y"
    writeIntegerDataset(group, "number_elements_y", matrixArray->numberElementsY());

    // Write "pitch_x"
    writeOptionalDoubleDataset(group, "pitch_x", matrixArray->pitchX());

    // Write "pitch_y"
    writeOptionalDoubleDataset(group, "pitch_y", matrixArray->pitchY());

    // Write "element_width"
    writeOptionalDoubleDataset(group, "element_width", matrixArray->elementWidth());

    // Write "element_height"
    writeOptionalDoubleDataset(group, "element_height", matrixArray->elementHeight());
}

void Writer::writeRcaArray(H5::Group& group, const std::shared_ptr<uff::RcaArray>& rcaArray)
{
    // Write "number_elements_x"
    writeIntegerDataset(group, "number_elements_x", rcaArray->numberElementsX());

    // Write "number_elements_y"
    writeIntegerDataset(group, "number_elements_y", rcaArray->numberElementsY());

    // Write "pitch_x"
    writeOptionalDoubleDataset(group, "pitch_x", rcaArray->pitchX());

    // Write "pitch_y"
    writeOptionalDoubleDataset(group, "pitch_y", rcaArray->pitchY());

    // Write "element_width_x"
    writeOptionalDoubleDataset(group, "element_width_x", rcaArray->elementWidthX());

    // Write "element_width_y"
    writeOptionalDoubleDataset(group, "element_width_y", rcaArray->elementWidthY());

    // Write "element_height_x"
    writeOptionalDoubleDataset(group, "element_height_x", rcaArray->elementHeightX());

    // Write "element_height_y"
    writeOptionalDoubleDataset(group, "element_height_y", rcaArray->elementHeightY());
}

void Writer::writeProbe(H5::Group& group, const std::shared_ptr<uff::Probe>& probe)
{
    // write "transform"
    H5::Group transform = group.createGroup("transform");
    writeTransform(transform, probe->transform());

    // write "elements"
    H5::Group elements = group.createGroup("elements");
    writeElementArray(elements, probe->elements());

    // write "focal_length" (optional)
    writeOptionalDoubleDataset(group, "focal_length", probe->focalLength());

    // write "element_geometry" (optional)
    // write "element_impulse_response" (optional)

    // MatrixArray ?
    std::shared_ptr<uff::MatrixArray> matrixArray = std::dynamic_pointer_cast<uff::MatrixArray>(probe);
    if (matrixArray.get() != nullptr)
    {
        writeStringDataset(group, "probe_type", "MatrixArray");
        writeMatrixArray(group, matrixArray);
        return;
    }

    // LinearArray ?
    std::shared_ptr<uff::LinearArray> linearArray = std::dynamic_pointer_cast<uff::LinearArray>(probe);
    if (linearArray.get() != nullptr)
    {
        writeStringDataset(group, "probe_type", "LinearArray");
        writeLinearArray(group, linearArray);
        return;
    }    

    // RcaArray ?
    std::shared_ptr<uff::RcaArray> rcaArray = std::dynamic_pointer_cast<uff::RcaArray>(probe);
    if (rcaArray.get() != nullptr)
    {
        writeStringDataset(group, "probe_type", "RcaArray");
        writeRcaArray(group, rcaArray);
        return;
    }
}

void Writer::writeProbeArray(H5::Group& group, const std::vector<std::shared_ptr<uff::Probe>>& probes)
{
    char buf[9];
    snprintf(buf, sizeof buf, "%08d", 0);
    for (int i = 0; i < probes.size(); i++)
    {
        snprintf(buf, sizeof buf, "%08d", i + 1);
        std::string probe_id = buf;
        H5::Group probe = group.createGroup(probe_id);
        writeProbe(probe, probes[i]);
    }
}

void Writer::writeReceiveSetup(H5::Group& group, const uff::ReceiveSetup& receiveSetup)
{
    // "probe"
    std::string probeId = getIdFromPointer<uff::Probe>(m_dataset.acquisition().probes(), receiveSetup.probe());
    writeStringDataset(group, "probe_id", probeId);

    // "time_offset"
    writeDoubleDataset(group, "time_offset", receiveSetup.timeOffset());

    // "sampling_frequency"
    writeOptionalDoubleDataset(group, "sampling_frequency", receiveSetup.samplingFrequency());

    // "sampling_type"
    writeIntegerDataset(group, "sampling_type", receiveSetup.samplingType());

    // "channel_mapping"
    writeIntegerArrayDataset(group, "channel_mapping", receiveSetup.channelMapping(), {});

    // "tgc_profile"
    writeFloatArrayDataset(group, "tgc_profile", receiveSetup.tgcProfile(), {});

    // "tgc_sampling_frequency"
    writeOptionalDoubleDataset(group, "tgc_sampling_frequency", receiveSetup.tgcSamplingFrequency());

    // "modulation_frequency"
    writeOptionalDoubleDataset(group, "modulation_frequency", receiveSetup.modulationFrequency());
}

void Writer::writeRotation(H5::Group& group, const uff::Rotation& rotation)
{
    writeDoubleDataset(group, "x", rotation.x());
    writeDoubleDataset(group, "y", rotation.y());
    writeDoubleDataset(group, "z", rotation.z());
}

void Writer::writeTimedEvent(H5::Group& group, const uff::TimedEvent& timedEvent)
{
    // "event"
    std::string eventId = getIdFromPointer<uff::Event>(m_dataset.acquisition().uniqueEvents(), timedEvent.evenement());
    writeStringDataset(group, "event_id", eventId);

    // "time_offset"
    writeDoubleDataset(group, "time_offset", timedEvent.timeOffset());
}

void Writer::writeTimedEventArray(H5::Group& group, const std::vector<uff::TimedEvent>& timedEvents)
{
    char buf[9];
    snprintf(buf, sizeof buf, "%08d", 0);
    for (int i = 0; i < timedEvents.size(); i++)
    {
        snprintf(buf, sizeof buf, "%08d", i + 1);
        std::string timedEvent_id = buf;
        H5::Group timedEvent = group.createGroup(timedEvent_id);
        writeTimedEvent(timedEvent, timedEvents[i]);
    }
}

void Writer::writeTransform(H5::Group& group, const uff::Transform& transform)
{
    // "rotation"
    H5::Group rotation = group.createGroup("rotation");
    writeRotation(rotation, transform.rotation());

    // "translation"
    H5::Group translation = group.createGroup("translation");
    writeTranslation(translation, transform.translation());
}

void Writer::writeTranslation(H5::Group& group, const uff::Translation& translation)
{
    writeDoubleDataset(group, "x", translation.x());
    writeDoubleDataset(group, "y", translation.y());
    writeDoubleDataset(group, "z", translation.z());
}

void Writer::writeTransmitSetup(H5::Group& group, const uff::TransmitSetup& transmitSetup)
{
    // "probe"
    const std::string probeId = getIdFromPointer<uff::Probe>(m_dataset.acquisition().probes(), transmitSetup.probe());
    writeStringDataset(group, "probe_id", probeId);

    // "time_offset"
    writeDoubleDataset(group, "time_offset", transmitSetup.timeOffset());

    // "wave"
    const std::string waveId = getIdFromPointer<uff::Wave>(m_dataset.acquisition().uniqueWaves(), transmitSetup.wave());
    writeStringDataset(group, "wave_id", probeId);
}

void Writer::writeVersion(H5::Group& group, const uff::Version& version)
{
    writeIntegerDataset(group, "major", version.major());
    writeIntegerDataset(group, "minor", version.minor());
    writeIntegerDataset(group, "patch", version.patch());
}

    void Writer::writeWave(H5::Group& group, const std::shared_ptr<uff::Wave>& wave)
    {
        // write "origin"
        H5::Group origin = group.createGroup("origin");
        writeTransform(origin, wave->origin());

        // write "wave_type"
        writeIntegerDataset(group, "wave_type", (int)wave->waveType());

        // write "aperture"
        H5::Group aperture = group.createGroup("aperture");
        writeAperture(aperture, wave->aperture());

        // "channel_mapping"
        writeIntegerArrayDataset(group, "channel_mapping", wave->channelMapping(), { wave->channelMapping().size() });

        // write "excitation"
        const std::string excitationId = getIdFromPointer<uff::Excitation>(m_dataset.acquisition().uniqueExcitations(), wave->excitation());
        writeStringDataset(group, "excitation_id", excitationId);
    }

    void Writer::writeWaveArray(H5::Group& group, const std::vector<std::shared_ptr<uff::Wave>>& waves)
    {
        char buf[9];
        snprintf(buf, sizeof buf, "%08d", 0);
        for (int i = 0; i < waves.size(); i++)
        {
            snprintf(buf, sizeof buf, "%08d", i + 1);
            std::string wave_id = buf;
            H5::Group wave = group.createGroup(wave_id);
            writeWave(wave, waves[i]);
        }
    }

    void Writer::writeAperture(H5::Group& group, const uff::Aperture& aperture)
    {
        // "origin"
        H5::Group origin = group.createGroup("origin");
        writeTransform(origin, aperture.origin());

        // "window"
        writeOptionalStringDataset(group, "window", aperture.window());

        // "f_number"
        writeOptionalDoubleDataset(group, "f_number", aperture.fNumber());

        // "fixed_size"
        writeOptionalDoubleDataset(group, "fixed_size", aperture.fixedSize());

        // "maximum_size" TODO
        // "minimum_size" TODO
    }

    void Writer::writeExcitation(H5::Group& group, const std::shared_ptr<Excitation>& excitation)
    {
        // "pulse_shape"
        writeOptionalStringDataset(group, "pulse_shape", excitation->pulseShape());

        // "transmit_frequency"
        writeOptionalDoubleDataset(group, "transmit_frequency", excitation->transmitFrequency());

        // "waveform"
        writeFloatArrayDataset(group, "waveform", excitation->waveform(), {});

        // "sampling_frequency"
        writeOptionalDoubleDataset(group, "sampling_frequency", excitation->samplingFrequency());
    }

    void Writer::writeExcitationArray(H5::Group& group, const std::vector<std::shared_ptr<Excitation>>& excitations)
    {
        char buf[9];
        snprintf(buf, sizeof buf, "%08d", 0);
        for (int i = 0; i < excitations.size(); i++)
        {
            snprintf(buf, sizeof buf, "%08d", i + 1);
            std::string id = buf;
            H5::Group hdf5Group = group.createGroup(id);
            writeExcitation(hdf5Group, excitations[i]);
        }
    }

    // ___________________________ Write Low level types ___________________________________________________________________________________________________

    H5::DataSet Writer::writeStringDataset(H5::Group& group, const std::string& name, const std::string& value)
    {
        H5::StrType vlst(0, H5T_VARIABLE);
        H5::DataSpace ds_space(H5S_SCALAR);
        H5::DataSet dataset = group.createDataSet(name.c_str(), vlst, ds_space);
        dataset.write(value, vlst);
        return dataset;
    }

    H5::DataSet Writer::writeOptionalStringDataset(H5::Group& group, const std::string& name, const std::optional<std::string>& value)
    {
        H5::StrType vlst(0, H5T_VARIABLE);
        H5::DataSpace ds_space(H5S_SCALAR);
        H5::DataSet dataset = group.createDataSet(name.c_str(), vlst, ds_space);
        if (value.has_value())
            dataset.write(value.value(), vlst);
        else
            dataset.write(std::string("undefined"), vlst);
        return dataset;
    }

    H5::DataSet Writer::writeDoubleDataset(H5::Group& group, const std::string& name, double value)
    {
        H5::StrType datatype(H5::PredType::NATIVE_DOUBLE);
        H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
        H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
        dataset.write(&value, datatype, dataspace);
        dataset.close();
        return dataset;
    }

    H5::DataSet Writer::writeOptionalDoubleDataset(H5::Group& group, const std::string& name, std::optional<double> value)
    {
        H5::StrType datatype(H5::PredType::NATIVE_DOUBLE);
        H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
        H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
        if (value.has_value())
            dataset.write(&value.value(), datatype, dataspace);
        else
        {
            constexpr double nan = std::numeric_limits<double>::quiet_NaN();
            dataset.write(&nan, datatype, dataspace);
        }
        dataset.close();
        return dataset;
    }

    H5::DataSet Writer::writeIntegerArrayDataset(H5::Group& group, const std::string& name, const std::vector<int>& values, const std::vector<size_t>& dimensions)
    {
        assert(dimensions.size() <= 4);

        hsize_t dims[4];
        int ndims = (int)dimensions.size();
        if (ndims == 0)    // unspecified dimension: write 1D array
        {
            ndims = 1;
            dims[0] = values.size();
        }
        else
        {
            size_t numel = dimensions[0];
            dims[0] = dimensions[0];
            for (int i = 1; i < dimensions.size(); i++)
            {
                numel *= dimensions[i];
                dims[i] = dimensions[i];
            }

            // check if prod(dimensions) == values.length()
            assert(values.size() == numel);
        }

        H5::DataSpace dataspace = H5::DataSpace(ndims, dims);
        H5::DataSet dataset = group.createDataSet(name, H5::PredType::NATIVE_INT, dataspace);
        dataset.write(values.data(), H5::PredType::NATIVE_INT);
        dataset.close();
        return dataset;
    }

    H5::DataSet Writer::writeIntegerDataset(H5::Group& group, const std::string& name, int value)
    {
        H5::StrType datatype(H5::PredType::NATIVE_INT);
        H5::DataSpace dataspace = H5::DataSpace(H5S_SCALAR);
        H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
        dataset.write(&value, datatype, dataspace);
        dataset.close();
        return dataset;
    }

    H5::DataSet Writer::writeFloatArrayDataset(H5::Group& group, const std::string& name,
        const std::vector<float>& values, const std::vector<size_t>& dimensions)
    {
        assert(dimensions.size() <= 4);

        hsize_t dims[4];
        int ndims = (int)dimensions.size();
        if (ndims == 0)    // unspecified dimension: write 1D array
        {
            ndims = 1;
            dims[0] = values.size();
        }
        else
        {
            size_t numel = dimensions[0];
            dims[0] = dimensions[0];
            for (int i = 1; i < dimensions.size(); i++)
            {
                numel *= dimensions[i];
                dims[i] = dimensions[i];
            }

            // check if prod(dimensions) == values.length()
            assert(values.size() == numel);
        }

        H5::DataSpace dataspace = H5::DataSpace(ndims, dims);
        H5::DataSet dataset = group.createDataSet(name, H5::PredType::NATIVE_FLOAT, dataspace);
        dataset.write(values.data(), H5::PredType::NATIVE_FLOAT);
        dataset.close();
        return dataset;
    }

    H5::DataSet Writer::writeInt16ArrayDataset(H5::Group& group, const std::string& name,
        const std::vector<int16_t>& values, const std::vector<size_t>& dimensions)
    {
        assert(dimensions.size() <= 4);

        hsize_t dims[4];
        int ndims = (int)dimensions.size();
        if (ndims == 0)    // unspecified dimension: write 1D array
        {
            ndims = 1;
            dims[0] = values.size();
        }
        else
        {
            size_t numel = dimensions[0];
            dims[0] = dimensions[0];
            for (int i = 1; i < dimensions.size(); i++)
            {
                numel *= dimensions[i];
                dims[i] = dimensions[i];
            }

            // check if prod(dimensions) == values.length()
            assert(values.size() == numel);
        }

        H5::DataSpace dataspace = H5::DataSpace(ndims, dims);
        H5::DataSet dataset = group.createDataSet(name, H5::PredType::NATIVE_INT16, dataspace);
        dataset.write(values.data(), H5::PredType::NATIVE_INT16);
        dataset.close();
        return dataset;
    }

    /**
     * Return the string-ID (format: "00000123") of the pointer wptr.
     * The string-ID is the position of the pointer in the vector 'vec'. First pointer's string-ID is "00000001".
     * If 'wptr' does not point to an object stored in 'vec' then return "????????"
     */
    template<typename T>
    std::string Writer::getIdFromPointer(const std::vector<std::shared_ptr<T>>& vec, std::weak_ptr<T> wptr)
    {
        if (auto p1 = wptr.lock())
        {
            int cnt = 1;
            for (auto p2 : vec)
            {
                if (p1 == p2)
                {
                    char buf[9];
                    snprintf(buf, sizeof buf, "%08d", cnt);
                    return std::string(buf);
                }
                cnt++;
            }
        }

        return "????????";
    }

} // namespace uff
