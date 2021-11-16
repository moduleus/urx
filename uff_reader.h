/*!
 * Copyright Moduleus
 * \file uff_reader.h
 * \brief
 */

#ifndef UFF_READER_H
#define UFF_READER_H

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
        Reader() {}
    
        void printSelf(std::ostream& os, std::string indent) const override;

        std::shared_ptr<uff::Dataset> dataset() { return m_dataset; }

        // Set/Get the filename of the UFF file. The 'fileName' must contain the file extension. 
        std::string fileName() const { return m_fileName; }
        void setFileName(const std::string& fileName) { m_fileName = fileName; }

        void updateMetadata();

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

        // Int
        void readIntegerArrayDataset(const H5::Group& group, const std::string& name, std::vector<int>& values, std::vector<size_t>& dimensions);
        int readIntegerDataset(const H5::Group& group, const std::string& name);

        // std::string
        std::string readStringDataset(const H5::Group& group, const std::string& name);
        std::optional<std::string> readOptionalStringDataset(const H5::Group& group, const std::string& name);

        // float Vector
        void readFloatArrayDataset(const H5::Group& group, const std::string& name,
            std::vector<float>& values, std::vector<size_t>& dimensions);
        // int16_t vector
        void readInt16ArrayDataset(const H5::Group& group, const std::string& name,
            std::vector<int16_t>& values, std::vector<size_t>& dimensions);
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
    inline std::vector<T> Reader::readArray(const H5::Group& group)
    {
        std::vector<T> vector;

        char buf[9];
        int id = 1;
        snprintf(buf, sizeof buf, "%08d", id);
        while (H5Lexists(group.getLocId(), buf, H5P_DEFAULT))
        {
            H5::Group hdf5Group = group.openGroup(buf);

            if      (std::is_same_v<T, std::shared_ptr<uff::GroupLink>>())   { vector.push_back(readGroupLink(hdf5Group)); }
            else if (std::is_same_v<T, std::shared_ptr<uff::IGroup>>())      { vector.push_back(readIGroup(hdf5Group)); }
            else if (std::is_same_v<T, std::shared_ptr<uff::Probe>>())       { vector.push_back(readProbe(hdf5Group)); }
            else if (std::is_same_v<T, std::shared_ptr<uff::Event>>())       { vector.push_back(readEvent(hdf5Group)); }
            else if (std::is_same_v<T, std::shared_ptr<uff::Wave>>())        { vector.push_back(readWave(hdf5Group)); }
            else if (std::is_same_v<T, std::shared_ptr<uff::Excitation>>())  { vector.push_back(readExcitation(hdf5Group)); }
            else if (std::is_same_v<T, std::shared_ptr<uff::GroupData>>())   { vector.push_back(readGroupData(hdf5Group)); }
            else if (std::is_same_v<T, Element>())                           { vector.push_back(readElement(hdf5Group)); }
            else { assert(false); }

            id++;
            snprintf(buf, sizeof buf, "%08d", id);
        }

        return vector;
    }

} // namespace uff

#endif // UFF_READER_H
