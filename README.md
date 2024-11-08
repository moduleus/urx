# URX

## Description

The Ultrasound Raw data eXchange file format is a structure of data to store ultrasound acquisition.

You may use it with various languages (C++, Python and MATLAB).

[Get more informations in the Wiki](./wiki) about class diagram.

## C++

### CMake

#### Available files

All CMake files will be installed in `INSTALL_PATH/lib/cmake/Urx-XXXYYY/` with:
  - XXX: the version of the Urx library,
  - YYY: empty if library has been built in shared mode, "_static" if library is static.

#### Configuration

First, you need to get Urx package with `find_package`.

```cmake
find_package(Urx REQUIRED)
```

You will have access to targets:

  - `Urx::Urx`: header-only library of the urx struct.
  - `Urx::UrxUtils`: library with helper to manipulate urx struct, including read / write in hdf5 file format.

#### Usage

```cmake
add_executable(UrxTestIo ${SRCS_FILES})
target_link_libraries(
  UrxTestIo PRIVATE Urx::Urx Urx::UrxUtils ${HDF5_CXX_LIBRARIES})
```

`Urx::UrxUtils` is only needed if you use helpers. If you only need the urx file format, only `Urx::Urx` is needed.

`HDF5_CXX_LIBRARIES` is only needed if the library has been built with HDF5 support.

You may found a complete example in `test/urx` or in `test/io` folders.

### C++

#### Available files

All files will be installed in :

  - `INSTALL_PATH/include/Urx-XXXYYY/` (headers),
  - `INSTALL_PATH/lib` (static libraries),
  - `INSTALL_PATH/bin` (executables and libraries for Windows only).

#### Usage

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

## MATLAB

### Available files

All MATLAB files will be installed in :

    - `INSTALL_PATH/share/Urx-XXXYYY/matlab` (scripts),
    - `INSTALL_PATH/lib` (static libraries),
    - `INSTALL_PATH/bin` (shared libraries for Windows only).

### Configuration

If you are using Urx from MATLAB toolkit, you just have to enable Unicode if you need it (`feature('DefaultCharacterSet','UTF-8');`).

If you built Urx from source, before using `Urx` in MATLAB, you first need to:

  - allow utf-8 caracters,
  - load the urx library.

```MATLAB
addpath([pwd 'INSTALL_PATH/share/Urx-XXXYYY/matlab']);
feature('DefaultCharacterSet','UTF-8');
urx.LibBinding.getInstance([path to libUrxMatlabBinding.so], [path to INSTALL_PATH/include/Urx-XXXYYY/urx/matlab/bindings.h], {[path to INSTALL_PATH/include/Urx-XXXYYY]}, {});
```

⚠ When invoking `urx.LibBinding.getInstance` the first time, environment variable `HDF5_DISABLE_VERSION_CHECK` is set. MATLAB have it's own version of HDF5 library. Under Linux it will conflict with the one from Urx.

If you try to use `dataset.saveToFile(...)` or `urx.Dataset.loadFromFile(...)` without setting `HDF5_DISABLE_VERSION_CHECK`, MATLAB will crash with error: `Headers are 1.8.23, library is 1.8.12`.

<details>
  <summary>Example of full error message</summary>
  
  ```
Warning! ***HDF5 library version mismatched error***
The HDF5 header files used to compile this application do not match
the version used by the HDF5 library to which this application is linked.
Data corruption or segmentation faults may occur if the application continues.
This can happen when an application was compiled by one version of HDF5 but
linked with a different version of static or shared HDF5 library.
You should recompile the application or check your shared library related
settings such as 'LD_LIBRARY_PATH'.
'HDF5_DISABLE_VERSION_CHECK' environment variable is set to 1, application will
continue at your own risk.
Headers are 1.8.23, library is 1.8.12
	    SUMMARY OF THE HDF5 CONFIGURATION
	    =================================
General Information:
-------------------
		   HDF5 Version: 1.8.12
		  Configured on: Mon May 10 15:29:10 EDT 2021
		  Configured by: batserve@bat1062601glnxa64 at The MathWorks, Inc., Natick, MA 01760
		 Configure mode: production
		    Host system: x86_64-pc-linux-gnu
	      Uname information: Linux bat1062601glnxa64 4.19.0-13-amd64 #1 SMP Debian 4.19.160-2 (2020-11-28) x86_64 GNU/Linux
		       Byte sex: little-endian
		      Libraries: shared
	     Installation point: /mathworks/devel/bat/B3p3/build/3p/install/7215999/glnxa64/hdf5-1.8
Compiling Options:
------------------
               Compilation Mode: production
                     C Compiler: /mathworks/hub/3rdparty/internal/3349624/glnxa64/gcc-6.3.0/bin/gcc ( gcc (MW GCC 6.3.0-GLIBC2.12-gold) 6.3.0)
                         CFLAGS: -O2 -pipe -pthread -fdebug-prefix-map=/mathworks/devel/bat/B3p3/build= -fPIC 
                      H5_CFLAGS:   -ansi -pedantic -Wall -W -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline -O -fomit-frame-pointer -finline-functions
                      AM_CFLAGS: 
                       CPPFLAGS: 
                    H5_CPPFLAGS: -D_POSIX_C_SOURCE=199506L   -DNDEBUG -UH5_DEBUG_API
                    AM_CPPFLAGS: -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_BSD_SOURCE 
               Shared C Library: yes
               Static C Library: no
  Statically Linked Executables: no
                        LDFLAGS: -Wl,--as-needed -fuse-ld=gold -Wl,--disable-new-dtags -Wl,--hash-style=both -Wl,--version-script=/mathworks/devel/bat/B3p3/build/3p/sources/hdf5-1.8//hdf5_linux_exported_symbol.map
                     H5_LDFLAGS: 
                     AM_LDFLAGS: 
 	 	Extra libraries:  -lz -ldl -lm 
 		       Archiver: ar
 		 	 Ranlib: ranlib
 	      Debugged Packages: 
		    API Tracing: no
Languages:
----------
                        Fortran: no
                            C++: no
Features:
---------
                  Parallel HDF5: no
             High Level library: yes
                   Threadsafety: no
            Default API Mapping: v18
 With Deprecated Public Symbols: yes
         I/O filters (external): deflate(zlib)
         I/O filters (internal): shuffle,fletcher32,nbit,scaleoffset
                            MPE: no
                     Direct VFD: no
                        dmalloc: no
Clear file buffers before write: yes
           Using memory checker: no
         Function Stack Tracing: no
                           GPFS: no
      Strict File Format Checks: no
   Optimization Instrumentation: no
       Large File Support (LFS): yes
```
</details>

Urx must use the same version (at least major / minor number) than the one from MATLAB (here 1.8).

Why is there a problem? Under Linux, the same symbol from two differents libraries are merged. So you can't know which one will be used. By setting `HDF5_DISABLE_VERSION_CHECK` to 1, you are disabling version check. Under Windows, the operating system keeps symbols from different libraries separate.

### Usage

```MATLAB
dataset = urx.Dataset();

probe1 = urx.Probe();
probe1.description = 'Probe 1';
dataset.acquisition.excitations = [excitation1];

dataset.saveToFile('test.urx');

dataset2 = urx.Dataset.loadFromFile('test.urx');
```

You may found a complete example in file `test/matlab/+urx/+UnitTests/FileFromScratch.m`.

### Important note

Some field are optional or can be empty (see C++ UML diagram). If you want to unassign a value, don't use `[]` but use `empty` method of the class (i.e. `urx.ElementGeometry.empty`).

## Python

### Usage

```python
import ultrasound_rawdata_exchange as urx;

dataset = urx.Dataset()

probe1 = urx.Probe()
probe1.description = 'Probe 1'
dataset.acquisition.excitations = [excitation1]

urx.saveToFile('test.urx', dataset)

dataset2 = urx.loadFromFile('test.urx')
```

You may found a basic usage in folder `test/python`.

## Build it with CMake

See `.gitlab-ci.yml` file for various examples.

### Windows

Under Windows, you need to explicitly set Visual Studio generator.

`cmake.exe -S root_path -B CI -G "Visual Studio 17 2022" -A x64`

### Linux

`cmake -S . -B CI`

### Common features

  * Install path

Set `-DCMAKE_INSTALL_PREFIX=` variable to define the installation path.

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

## Build it with pip wheel

### Using vcpkg

You may generate wheel archive by launching `pip wheel .` from the root directory of the project.

The `setup.py` file support the following options:

  - `CMAKE_TOOLCHAIN_FILE` to set vcpkg path (repository cloned if option is not given),
  - `cmake_build_type` to set CMAKE_BUILD_TYPE in CMake (Release by default),
  - `vcpkg_triplet` to set vcpkg triplet (guessed if omited),

You can also set CMake variable with `-DVARIABLE=VALUE`.

Example:

```sh
python.exe -m pip install .
--config-settings="--global-option=CMAKE_TOOLCHAIN_FILE=vcpkg_path/scripts/buildsystems/vcpkg.cmake"
--config-settings="--global-option=cmake_build_type=RelWithDebInfo"
--config-settings="--global-option=vcpkg_triplet=x64-wsmrep"
--config-settings="--global-option=-DBUILD_SHARED_LIBS=OFF"
--config-settings="--global-option=-DWITH_HDF5=ON"
-v
```

See `build:windows-python` and `build:ubuntu-python` in `.gitlab-ci.yml` file for real examples.

### Without vcpkg

If you want to disable vcpkg and use you own 3rd party library (that you already built), you can set `DISABLE_VCPKG` to `ON` and set all `XXX_DIR` CMake variable (path of the CMake files of the 3rd party).

See CMake target `UrxPythonWheel` in `python/CMakeLists.txt` file for real example.
