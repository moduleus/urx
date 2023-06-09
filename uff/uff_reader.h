/*!
 * Copyright Moduleus
 * \file uff_reader.h
 * \brief
 */

#ifndef UFF_READER_H
#define UFF_READER_H

#ifdef WITH_HDF5

// UFF
#include "uff_dataset.h"
#include "uff_object.h"

#include <H5Cpp.h>
#include <optional>

namespace uff
{
    
    /**
     * @brief The UFF reader class
     * TODO: setFileName(), update(), 
     * TODO: only read metadata first, then read RF channel data next and only on-demand
     */
    class Reader : public uff::Object
    {
        UFF_TYPE_MACRO(Reader, uff::Object);

    public:
        Reader(const std::string& fileName = "") { m_fileName = fileName; }
    
        void printSelf(std::ostream& os, std::string indent) const override;

        std::shared_ptr<uff::Dataset> dataset() { return m_dataset; }
         
        // Set/Get the filename of the UFF file. The 'fileName' must contain the file extension. 
        std::string fileName() const { return m_fileName; }
        void setFileName(const std::string& fileName) { m_fileName = fileName; }

        bool updateMetadata();

    protected:
        // ********************* Read Uff classes ***************************************************************
        // Acquistion
        void readAcquisition(const H5::Group& group);

        // Version
        void readVersion(const H5::Group& group);

        // Group link
        std::shared_ptr<uff::GroupLink> readGroupLink(const H5::Group& group);

        // Groups
        std::shared_ptr<uff::IGroup> readIGroup(const H5::Group& group);
        std::shared_ptr<uff::SuperGroup> readSuperGroup(const H5::Group& group);
        std::shared_ptr<uff::Group> readGroup(const H5::Group& group);

        // Group Data
        std::shared_ptr<uff::GroupData> readGroupData(const H5::Group& group);

        // Element
        uff::Element readElement(const H5::Group& group);

        // Event
        std::shared_ptr<uff::Event> readEvent(const H5::Group& group);

        // Excitation
        std::shared_ptr<uff::Excitation> readExcitation(const H5::Group& group);

        // Probe
        std::shared_ptr<uff::LinearArray> readLinearArray(const H5::Group& group);
        std::shared_ptr<uff::MatrixArray> readMatrixArray(const H5::Group& group);
        std::shared_ptr<uff::RcaArray> readRcaArray(const H5::Group& group);

        std::shared_ptr<uff::Probe> readProbe(const H5::Group& group);

        // Sequence
        uff::Sequence readSequence(const H5::Group& group);

        // Timed event
        uff::TimedEvent readTimedEvent(const H5::Group& group);

        // Receive Setup
        uff::ReceiveSetup readReceiveSetup(const H5::Group& group);

        // Transform
        uff::Transform readTransform(const H5::Group& group);
        uff::Rotation readRotation(const H5::Group& group);
        uff::Translation readTranslation(const H5::Group& group);

        // Transmit Setup
        uff::TransmitSetup readTransmitSetup(const H5::Group& group);

        // Wave
        std::shared_ptr<uff::Wave> readWave(const H5::Group& group);

        // Aperture
        uff::Aperture readAperture(const H5::Group& group);


        // ********************* Read basic types *******************************************************
        // Double
        double readDoubleDataset(const H5::Group& group, const std::string& name);
        std::optional<double> readOptionalDoubleDataset(const H5::Group& group, const std::string& name);
        
        // Float
        float readFloatDataset(const H5::Group& group, const std::string& name);
        std::optional<float> readOptionalFloatDataset(const H5::Group& group, const std::string& name);

        // Int
        void readIntegerArrayDataset(const H5::Group& group, const std::string& name, std::vector<int>& values, std::vector<size_t>& dimensions);
        int readIntegerDataset(const H5::Group& group, const std::string& name);

        // std::string
        std::string readStringDataset(const H5::Group& group, const std::string& name);
        std::optional<std::string> readOptionalStringDataset(const H5::Group& group, const std::string& name);

        // float Vector
        template<typename T>
        void readArrayDataset(const H5::Group& group, const std::string& name,
            std::vector<T>& values, std::vector<size_t>& dimensions);

        // Uff class vector
        template<typename T>
        std::vector<T> readArray(const H5::Group& group);
    
    private:
        // name of the file to read
        std::string m_fileName;

        // dataset
        std::shared_ptr<uff::Dataset> m_dataset;
    };

    template<typename T>
    inline void Reader::readArrayDataset(const H5::Group& group, const std::string& name, std::vector<T>& values, std::vector<size_t>& dimensions)
    {
        const H5::DataSet dataset = group.openDataSet(name);
        const H5::StrType datatype = dataset.getStrType();

        // find dataset dimensions
        const H5::DataSpace dataspace = dataset.getSpace();
        const int ndims = dataspace.getSimpleExtentNdims();
        dimensions.resize(ndims);
        dataspace.getSimpleExtentDims((unsigned long long*)dimensions.data()); // Poor casting
        size_t numel = 1;
        for (auto sz : dimensions) { numel *= sz; }

        // reserve space in the output buffer
        values.resize(numel);

        // read data
        dataset.read((void*)values.data(), datatype);
    }

    template<typename T>
    inline std::vector<T> Reader::readArray(const H5::Group& group)
    {
        std::vector<T> vector;

        char buf[9];
        int id = 1;
        snprintf(buf, sizeof buf, "%08d", id);
        while (H5Lexists(group.getLocId(), buf, H5P_DEFAULT))
        {
            H5::Group hdf5Group = group.openGroup(buf);

            T objectRead;

            if constexpr (std::is_same<T, std::shared_ptr<uff::GroupLink>>::value)   { objectRead = readGroupLink(hdf5Group); }
            else if constexpr (std::is_same<T, std::shared_ptr<uff::IGroup>>::value)      { objectRead = readIGroup(hdf5Group); }
            else if constexpr (std::is_same<T, std::shared_ptr<uff::Probe>>::value)       { objectRead = readProbe(hdf5Group); }
            else if constexpr (std::is_same<T, std::shared_ptr<uff::Event>>::value)       { objectRead = readEvent(hdf5Group); }
            else if constexpr (std::is_same<T, std::shared_ptr<uff::Wave>>::value)        { objectRead = readWave(hdf5Group); }
            else if constexpr (std::is_same<T, std::shared_ptr<uff::Excitation>>::value)  { objectRead = readExcitation(hdf5Group); }
            else if constexpr (std::is_same<T, std::shared_ptr<uff::GroupData>>::value)   { objectRead = readGroupData(hdf5Group); }
            else if constexpr (std::is_same<T, uff::Element>::value)                      { objectRead = readElement(hdf5Group); }
            else if constexpr (std::is_same<T, uff::TimedEvent>::value)                   { objectRead = readTimedEvent(hdf5Group); }
            else { assert(false); }

            vector.push_back(objectRead);

            id++;
            snprintf(buf, sizeof buf, "%08d", id);
        }

        return vector;
    }

} // namespace uff

#endif // WITH_HDF5

#endif // UFF_READER_H
