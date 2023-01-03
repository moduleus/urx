/*!
 * Copyright Moduleus
 * \file uff_writer.h
 * \brief
 */

#ifndef UFF_WRITER_H
#define UFF_WRITER_H

// UFF
#include "uff_dataset.h"
#include "uff_object.h"

#include <H5Cpp.h>
#include <optional>

namespace uff
{
    
/**
 * @brief The UFF Writer class
 */
class Writer : public uff::Object
{
    UFF_TYPE_MACRO(Writer, uff::Object);

public:

    Writer() {  }

    void printSelf(std::ostream& os, std::string indent) const override;
    
    uff::Dataset& dataset() { return m_dataset; }
    void setDataset(const uff::Dataset& dataset) { m_dataset = dataset; }
    
    /* Set/Get the filename of the UFF file. The 'fileName' must contain the file extension. */
    std::string fileName() const { return m_fileName; }
    void setFileName(const std::string& fileName) {  m_fileName = fileName; }

    /**
     * @brief Write the content of the Dataset in a file.
     * setDataset() and setFileName() must have been called before calling this method.
     * @return true if the writing is successful, false otherwise.
     */
    bool writeToFile();
    
private:
    void writeAcquisition(H5::Group& group, const uff::Acquisition& acquisition);
    void writeVersion(H5::Group& group, const uff::Version& version);

    // Group
    void writeGroupLink(H5::Group& group, const std::shared_ptr<GroupLink>& groupLink);

    void writeGroupData(H5::Group& group, const std::shared_ptr<GroupData>& groupData);

    void writeSuperGroup(H5::Group& group, const std::shared_ptr<uff::SuperGroup>& superGroup);
    void writeGroup(H5::Group& group, const std::shared_ptr<uff::Group>& groupUff);
    void writeIGroup(H5::Group& group, const std::shared_ptr<uff::IGroup>& igroup);

    // Probe
    void writeProbe(H5::Group& group, const std::shared_ptr<uff::Probe>& probe);

    void writeElement(H5::Group& group, const uff::Element& element);

    void writeLinearArray(H5::Group& group, const std::shared_ptr<uff::LinearArray>& linearArray);
    void writeMatrixArray(H5::Group& group, const std::shared_ptr<uff::MatrixArray>& matrixArray);
    void writeRcaArray(H5::Group& group, const std::shared_ptr<uff::RcaArray>& rcaArray);

    // Sequence
    void writeSequence(H5::Group& group, const Sequence& sequence);

    void writeTimedEvent(H5::Group& group, const uff::TimedEvent& timedEvent);

    void writeEvent(H5::Group& group, const std::shared_ptr<uff::Event>& event);

    // Receive Setup
    void writeReceiveSetup(H5::Group& group, const uff::ReceiveSetup& receiveSetup);

    // Transmit setup
    void writeTransmitSetup(H5::Group& group, const uff::TransmitSetup& transmitSetup);

    void writeWave(H5::Group& group, const std::shared_ptr<uff::Wave>& wave);
    void writeAperture(H5::Group& group, const uff::Aperture& aperture);
    void writeExcitation(H5::Group& group, const std::shared_ptr<Excitation>& excitation);

    // Transform
    void writeTransform(H5::Group& group, const uff::Transform& transform);
    void writeTranslation(H5::Group& group, const uff::Translation& translation);
    void writeRotation(H5::Group& group, const uff::Rotation& rotation);
    
    // _____________ Write Low level types ______________________________
    // String
    H5::DataSet writeStringDataset(H5::Group& group, const std::string& name, const std::string& value);
    H5::DataSet writeOptionalStringDataset(H5::Group& group, const std::string& name, const std::optional<std::string>& value);

    // Double
    H5::DataSet writeDoubleDataset(H5::Group& group, const std::string& name, double value);
    H5::DataSet writeOptionalDoubleDataset(H5::Group& group, const std::string& name, const std::optional<double>& value);

    // Integer
    H5::DataSet writeIntegerArrayDataset(H5::Group& group, const std::string& name, const std::vector<int>& values, const std::vector<size_t>& dimensions);
    H5::DataSet writeIntegerDataset(H5::Group& group, const std::string& name, int value);
    
    // Float
    H5::DataSet writeFloatArrayDataset(H5::Group& group, const std::string& name, const std::vector<float>& values, const std::vector<size_t>& dimensions);

    // int16_t
    H5::DataSet writeInt16ArrayDataset(H5::Group& group, const std::string& name, const std::vector<int16_t>& values, const std::vector<size_t>& dimensions);

    // _________________ Tools ___________________________
    template<typename T>
    std::string getIdFromPointer(const std::vector<std::shared_ptr<T>>& vec, std::weak_ptr<T> wptr);

    template<typename T>
    void writeArray(H5::Group& group, const std::vector<T>& array);

private:
    std::string m_fileName;
    uff::Dataset m_dataset;
};

} // namespace uff

#endif // UFF_WRITER_H
