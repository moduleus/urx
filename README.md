# URX

## Description

The Ultrasound Raw data eXchange file format is a structure of data to store ultrasound acquisition.

You may use it with various languages (C++, Python and MATLAB).

TODO BIG IMAGE OF THE URX PLANTUML FROM WIKI.

[Get more informations in the Wiki](http://google.fr).

## Basic usage / examples

### C++

#### CMake

##### Available files

All CMake files will be installed in `INSTALL/lib/cmake/Urx-XXXYYY/` with:
  - XXX: the version of the Urx library,
  - YYY: empty if library has been built in shared mode, "_static" if library is static.

##### Configuration

First, you need to get Urx package with `find_package` or by setting `Urx_DIR`.

```cmake
find_package(Urx REQUIRED)
```

You will have access to targets:

  - `Urx::Urx`: header-only library of the urx struct.
  - `Urx::UrxUtils`: library with helper to manipulate urx struct, including read / write in hdf5 file format.

##### Usage

```cmake
add_executable(UrxTestIo ${SRCS_FILES})
target_link_libraries(
  UrxTestIo PRIVATE Urx::Urx Urx::UrxUtils ${HDF5_CXX_LIBRARIES})
```

`Urx::UrxUtils` is only needed if you use helpers. If you only need the urx file format, only `Urx::Urx` is needed.

`HDF5_CXX_LIBRARIES` is only needed if the library has been built with HDF5 support.

You may found a complete example in `test/urx` or in `test/io` folders.

#### C++

##### Available files

All files will be installed in :

    - `INSTALL/include/Urx-XXXYYY/` (headers),
    - `install/lib` (static libraries),
    - `install/bin` (executables and libraries for Windows only).

##### Usage

All dynamic memory allocation should be stored in a `shared_ptr`.

```cpp
#include <memory>

#include <urx/dataset.h>

auto dataset = std::make_shared<urx::Dataset>();

auto probe = std::make_shared<urx::Probe>();
probe->description = "Probe 1";
dataset->acquisition.probes.push_back(probe);

urx::utils::io::writer::saveToFile("file.urx", *dataset);

auto dataset_loaded = urx::utils::io::reader::loadFromFile("file.urx");
```

You may found a complete example in file `urx/utils/io/test/io.h` in function `generateFakeDataset`.

### MATLAB

#### Available files

All MATLAB files will be installed in :

    - `INSTALL/share/Urx-XXXYYY/matlab` (scripts),
    - `install/lib` (static libraries),
    - `install/bin` (shared libraries for Windows only).

#### Configuration

Before using `Urx` in MATLAB, you first need to:

  - allow utf-8 caracters,
  - load the urx library.

```MATLAB
addpath([pwd 'INSTALL/share/Urx-XXXYYY/matlab']);
setenv("HDF5_DISABLE_VERSION_CHECK", "1");
feature('DefaultCharacterSet','UTF-8');
urx.LibBinding.getInstance([path to libUrxMatlabBinding.so], [path to INSTALL/include/Urx-XXXYYY/urx/matlab/bindings.h], {[path to INSTALL/include/Urx-XXXYYY]}, {});
```

Setting `HDF5_DISABLE_VERSION_CHECK` is tricky. MATLAB have it's own version of HDF5. Because we also use HDF5, we need to use the same version (at least major / minor numver). Under Linux, the same symbol from two differents libraries are merged. So you can't know which one will be used. By setting `HDF5_DISABLE_VERSION_CHECK` to 1, you are disabling version check. You must be absolutly sure that the major / minor version of HDF5 are the same between MATLAB and Urx.

#### Usage

```MATLAB
dataset = urx.Dataset();

probe1 = urx.Probe();
probe1.description = 'Probe 1';
dataset.acquisition.excitations = [excitation1];

dataset.saveToFile('test.urx');

dataset2 = urx.Dataset.loadFromFile('test.urx');
```

You may found a complete example in file `test/matlab/+urx/+UnitTests/FileFromScratch.m`.

#### Important note

Some field are optional or can be empty. If you want to unassign a value, don't use `[]` but use `urx.ElementGeometry.empty`.

### Python

#### Usage

```python
import pyurx as urx;

dataset = urx.Dataset()

probe1 = urx.Probe()
probe1.description = 'Probe 1'
dataset.acquisition.excitations = [excitation1]

urx.saveToFile('test.urx', dataset)

dataset2 = urx.loadFromFile('test.urx')
```

You may found a basic usage in folder `test/python`.

## Build it

See `.gitlab-ci.yml` file for various examples.

### With CMake

#### Windows

Under Windows, you need to explicitly set Visual Studio generator.

`cmake.exe -S root_path -B CI -G "Visual Studio 17 2022" -A x64`

#### Linux

`cmake -S . -B CI`

#### Common features

  * Debug or Release

If you use a single build type generator (like Makefile in Linux), you need to set `CMAKE_BUILD_TYPE` to `Debug` or `Release`.

  * Static or dynamic libraries

Set `BUILD_SHARED_LIBS` to `OFF` or `ON`.

Note that you should set `BUILD_SHARED_LIBS` to `OFF` if you want to build MATLAB or Python buiding.

Be sure to use a vcpkg triplet with the same shared / static link option.

  * Use external 3rd party with vcpkg

`vcpkg` is a package manager. You first need to clone it with `git clone https://github.com/microsoft/vcpkg.git`.

Also, be sure that submodule `vcpkg-registry` is initialized with `git submodule update --init --recursive`.

Then pass the following arguments:

Windows:
```cmake
-DCMAKE_TOOLCHAIN_FILE="vcpkg_path/scripts/buildsystems/vcpkg.cmake"
-DVCPKG_TARGET_TRIPLET=x64-wsmep -DVCPKG_HOST_TRIPLET=x64-wsmep
-DVCPKG_OVERLAY_TRIPLETS=vcpkg-registry/triplets"
```

Linux with static libraries (make sure that BUILD_SHARED_LIBS is set to OFF).
```cmake
-DCMAKE_TOOLCHAIN_FILE="vcpkg_path/scripts/buildsystems/vcpkg.cmake"
-DVCPKG_TARGET_TRIPLET=x64-linux -DVCPKG_HOST_TRIPLET=x64-linux
-DVCPKG_OVERLAY_TRIPLETS=vcpkg-registry/triplets
```

We provide various triplet with following syntax:

  - `w` for Windows,
  - `l` for Linux,
  - `d` for Linux dynamic (will be static otherwise),
  - `sm` for Windows static with /MD option (will be dynamic otherwise). See [VCPKG_CRT_LINKAGE](https://learn.microsoft.com/en-us/vcpkg/users/triplets#vcpkg_crt_linkage),
  - `e` to pass environment variable for Windows. See [VCPKG_ENV_PASSTHROUGH](https://learn.microsoft.com/en-us/vcpkg/users/triplets#vcpkg_env_passthrough),
  - `p` to pass Python3_EXECUTABLE to vcpkg via `VCPKG_Python3_EXECUTABLE` environment variable.

  * Enable HDF5

This feature is needed to read / write urx structure to file.

Set `WITH_HDF5` to `OFF` or `ON`.

  * Enable Python binding

Set `WITH_PYTHON` to `OFF` or `ON`.

You can also set `WITH_PYTHON_WHL` to `ON` if you want to generate wheel package without using `pip wheel`.

  * Enable MATLAB binding

Set `WITH_MATLAB` to `OFF` or `ON`.

You don't need to have MATLAB installed on your computer to build the MATLAB binding.

  * Build with Clang

You need to set lots of CMake variables and use CMake 3.29 if you want to use `CMAKE_LINKER_TYPE` variable.

```cmake
-DCMAKE_C_COMPILER=/usr/bin/clang-17
-DCMAKE_CXX_COMPILER=/usr/bin/clang++-17
-DCMAKE_AR=/usr/bin/llvm-ar-17
-DCMAKE_AS=/usr/bin/llvm-as-17
-DCMAKE_RANLIB=/usr/bin/llvm-ranlib-17
-DCMAKE_LINKER_TYPE=LLD
```

You may found various command to build Urx in file `.gitlab-ci.yml`.

### With pip wheel

You may generate wheel archive by launching `pip wheel .` from the root directory of the project.

The `setup.py` file support the following options:

  - `CMAKE_TOOLCHAIN_FILE` to set vcpkg path (repository cloned if option is not given),
  - `cmake_build_type` to set CMAKE_BUILD_TYPE in CMake (Release by default),
  - `vcpkg_triplet` to set vcpkg triplet (guessed if omited),
  - `build_shared_libs` to build with shared library (OFF is only supported / tested for Python binding),
  - `hdf5` to build with file support (ON by default).

Example:

`python.exe -m pip install . --config-settings="--global-option=CMAKE_TOOLCHAIN_FILE=vcpkg_path\\scripts\\buildsystems\\vcpkg.cmake" --config-settings="--global-option=cmake_build_type=RelWithDebInfo" --config-settings="--global-option=vcpkg_triplet=x64-wsmrep" --config-settings="--global-option=build_shared_libs=OFF" --config-settings="--global-option=hdf5=ON" -v`

See `build:windows-python` and `build:ubuntu-python` in `.gitlab-ci.yml` file for real examples.
