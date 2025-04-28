## Stream

Stream class allow to read and write a dataset and to manipulate GroupData without having all of them loaded in memory.

### Write

Before streaming a group data into a file, the full dataset must be written.

```cpp
#include <urx/utils/io/stream.h>

std::shared_ptr<urx::Dataset> dataset = std::make_shared<urx::Dataset>();

std::string filename = "stream.urx";

// A stream need a dataset and a filename to store data in HDF5 file.
Stream stream(filename, dataset);

stream.saveToFile();
```

Then, you can stream to a new group data.

Urx v1 only support contiguous dataset. You need to explicitly disable Chunk (by default).

```cpp
stream.setChunkGroupData(false);
```

But if you want efficiency stream and use chunk dataset, you need to explicitly enable chunk.

```cpp
stream.setChunkGroupData(true);
```

Finally, write 

```cpp
std::shared_ptr<urx::RawDataVector<std::complex<double>>> raw_data_double=...;

// Create un new GroupData by setting the group and group_timestamp.
urx::utils::io::GroupDataStream group_data =
    stream.createGroupData(dataset->acquisition.groups.front(), urx::DoubleNan(1.));
// For each sequence, append the raw data, sequence_timestamps and event_timestamps.
group_data.append(raw_data_double, 1.2, {2.3, 3.4, 4.5});
// Modify data for the next sequence.
// ...
group_data.append(raw_data_double, 12., {23., 34., 45.});
```

### Read

Before streaming a group data into memory, the full dataset (except raw data) must be read.

```cpp
#include <urx/utils/io/stream.h>

std::shared_ptr<urx::Dataset> dataset_loaded = std::make_shared<urx::Dataset>();
Stream stream(filename, dataset_loaded);
// Set it to STREAM to not load RawData at load time.
// Set it to FULL to load all RawData at load time.
stream.setRawDataLoadPolicy(urx::utils::io::RawDataLoadPolicy::STREAM);
// Load everything except RawData fields.
stream.loadFromFile();
```

Know you can read RawData.

```cpp
// 4 can be computed with GroupDataReader::offset(1, 0, 0, 0).
std::shared_ptr<urx::RawData> buffer_double =
    std::make_shared<urx::RawDataVector<std::complex<double>>>(4);

// Read data from the 0th group data (first argument) at sequence position 0 (fourth argument) and
// for 1 sequence (fifth argument) and write it in buffer_double at the sequence position 0
// (third argument).
stream.readRawData(0, buffer_double, 0, 0, 1);
useRawData(buffer_double);
// Read the second sequence in the same buffer.
stream.readRawData(0, buffer_double, 0, 1, 1);
```
