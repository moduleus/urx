/**
 * Copyright MODULEUS 2019
 */
 
// UFF
#include "uff_acquisition.h"
#include "uff_reader.h"
#include "uff_writer.h"

// System
#include <iostream>

#define CHECK_ISO8601(str)                                         \
    if(uff::Acquisition::isIso8601(str) == false)                 \
    {                                                             \
        std::cerr << "Not ISO-8601: " << (str) << std::endl;     \
        return -1;                                                \
    }

#define CHECK_NOT_ISO8601(str)                                  \
    if(uff::Acquisition::isIso8601(str) == true)                 \
    {                                                             \
        std::cerr << "ISO-8601: " << (str) << std::endl;         \
        return -1;                                                \
    }
    
#define CHECK_ISO3166(str)                                         \
    if(uff::Acquisition::isIso3166(str) == false)                 \
    {                                                             \
        std::cerr << "Not ISO-3166: " << (str) << std::endl;     \
        return -1;                                                \
    }

#define CHECK_NOT_ISO3166(str)                              \
    if(uff::Acquisition::isIso3166(str) == true)             \
    {                                                         \
        std::cerr << "ISO-3166: " << (str) << std::endl;     \
        return -1;                                            \
    }

int main(int argc, char* argv[])
{
    // Test ISO-8601
    CHECK_ISO8601("2008-09-12T15:53:00")
        CHECK_ISO8601("2019-06-15T15:53:00")
        CHECK_ISO8601("2008-09-15 05:53:59")
        CHECK_ISO8601("2008-09-15 15:53:00.0")

        CHECK_NOT_ISO8601("")
        CHECK_NOT_ISO8601("2015-01-17T28:23:02Z")
        CHECK_NOT_ISO8601("2009-05-19T14:3924")
        CHECK_NOT_ISO8601("2010-02-18T16:23.33.600")
        CHECK_NOT_ISO8601("2009-05-19T14:3924")
        CHECK_NOT_ISO8601("200-09-15T15:53:00")
        CHECK_NOT_ISO8601("2008-9-15T15:53:00")
        CHECK_NOT_ISO8601("2008-09-5T15:53:00")
        CHECK_NOT_ISO8601("2008-09-15T 5:53:00")
        CHECK_NOT_ISO8601("2008-13-15T15:53:00")
        CHECK_NOT_ISO8601("2008-09-32T15:53:00")
        CHECK_NOT_ISO8601("2008-09-15T25:53:00")

        // Test ISO-3166
        CHECK_ISO3166("FR")
        CHECK_ISO3166("US")
        CHECK_ISO3166("GE")
        CHECK_ISO3166("IT")
        CHECK_ISO3166("XX")

        CHECK_NOT_ISO3166("fr")
        CHECK_NOT_ISO3166("FRr")
        CHECK_NOT_ISO3166("France")
        CHECK_NOT_ISO3166("")


    // Test to read old uff, write the converted one and re read. 
    std::string filename = "C:\\Visual-WS\\uff_cpp\\data\\power_doppler_hd30.uff";
    uff::Reader reader(filename);
    assert(reader.updateMetadata());
    std::shared_ptr<uff::Dataset> oldDatasetConverted = reader.dataset();

    std::string writeFileName = filename.substr(0, filename.rfind('.')) + "_out" + filename.substr(filename.rfind('.'));
    uff::Writer writer;
    writer.setFileName(writeFileName);
    writer.setDataset(*oldDatasetConverted.get());
    assert(writer.writeToFile());

    reader.setFileName(writeFileName);
    assert(reader.updateMetadata());
    std::shared_ptr<uff::Dataset> datasetRead = reader.dataset();

    return 0;
}
