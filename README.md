# URX

## Description

The Ultrasound Raw data eXchange file format is a structure of data to store ultrasound acquisition.

You may use it with various languages (C++, Python and MATLAB).

[Get more informations in the Wiki](./wiki) about class diagram.

## Table of Contents

  * Common installation and usage in [C++](#use-it-with-c), [MATLAB](#use-it-with-matlab), [Python](#use-it-with-python).
  * Build it yourself for C++, MATLAB, Python with [CMake](doc/cpp.md#build).

## Use it with C++

There is no installer for C++ user. You need to [build](doc/cpp.md#build) it yourself. If you don't need to read / write file, urx library is header-only.

See [advanced uses](doc/cpp.md) for more information.

### Configuration

First, you need to get Urx package with `find_package`. If package is not found, set `Urx_DIR=INSTALL_PATH/lib/cmake/Urx-XXXYYY`.

```cmake
find_package(Urx REQUIRED)
```

You will have access to targets:

  - `Urx::Urx`: header-only library of the urx struct.
  - `Urx::UrxUtils`: library with helper to manipulate urx struct, including read / write in hdf5 file format.

### CMake examples

```cmake
find_package(HDF5 REQUIRED NAMES hdf5 COMPONENTS CXX hdf5::hdf5-static)
find_package(Urx REQUIRED)
add_executable(UrxTestIo ${SRCS_FILES})
target_link_libraries(
  UrxTestIo PRIVATE Urx::Urx Urx::UrxUtils hdf5::hdf5-static hdf5::hdf5_cpp-static)
```

`Urx::UrxUtils` is only needed if you use helpers. If you only need the urx file format, only `Urx::Urx` is needed.

`hdf5::hdf5-static hdf5::hdf5_cpp-static` is only needed if the library has been built with HDF5 support. In this case, hdf5 has been built in static mode.

You may found a complete example of `CMakeLists.txt` in `test/urx` or in `test/io` folders.

### C++ examples

All dynamic memory allocation should be stored in a `shared_ptr`.

```cpp
#include <memory>

#include <urx/dataset.h>
#include <urx/probe.h>
#include <urx/utils/io/reader.h>
#include <urx/utils/io/writer.h>

auto dataset = std::make_shared<urx::Dataset>();

auto probe = std::make_shared<urx::Probe>();
probe->description = "Probe 1";
dataset->acquisition.probes.push_back(probe);

urx::utils::io::writer::saveToFile("file.urx", *dataset);

auto dataset_loaded = urx::utils::io::reader::loadFromFile("file.urx");
```

You may found an example for all C++ classes in file `urx/utils/test/dataset_gen.h` in function `generateFakeDataset` and much more in the official wiki.

## Use it with MATLAB

See [advanced uses](doc/matlab.md) for more information.

### MATLAB installer

Urx MATLAB homepage is hosted in [MATLAB File Exchange](https://fr.mathworks.com/matlabcentral/fileexchange/176398-ultrasound-raw-data-exchange). To download the toolbox, select `Download` > `Toolbox`. It compatibles for both Windows and Linux.

### Requirements

MATLAB needs a C compiler to load the C library in Urx toolbox (i.e. Visual Studio or MinGW). To use MinGW, see [doc/matlab.md#install-mingw-c-compiler] paragraph.

### Configuration

If you are using Urx from MATLAB toolbox, you just have to enable Unicode if you need it (`feature('DefaultCharacterSet','UTF-8');`).

### Examples

```MATLAB
dataset = urx.Dataset();

probe1 = urx.Probe();
probe1.description = 'Probe 1';
excitation1 = urx.Excitation();
dataset.acquisition.excitations = [excitation1];

urx.saveToFile('test.urx', dataset);

dataset2 = urx.loadFromFile('test.urx');
```

You may found a complete example in file `test/matlab/+urx/+UnitTests/FileFromScratch.m` and much more in the official wiki.

### Important note

Some field are optional or can be empty (see C++ UML diagram). If you want to unassign a value, don't use `[]` but use `empty` method of the class (i.e. `urx.ElementGeometry.empty`).

## Use it with Python

See [advanced uses](doc/python.md) for more information.

### Python wheel

Urx Python is hosted in [Pypi](https://pypi.org/project/ultrasound-rawdata-exchange/).

From your Python's virtual environment, run `pip install ultrasound_rawdata_exchange`.

### Examples

```python
import ultrasound_rawdata_exchange as urx;

dataset = urx.Dataset()

probe1 = urx.Probe()
probe1.description = 'Probe 1'
excitation1 = urx.Excitation()
dataset.acquisition.excitations = [excitation1]

urx.saveToFile('test.urx', dataset)

dataset2 = urx.loadFromFile('test.urx')
```

You may found a basic example in folder `test/python` and much more in the official wiki.

