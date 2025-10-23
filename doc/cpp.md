## Build

You can use CMake to build urx for C++ / MATLAB / Python user.

See `.gitlab-ci.yml` file for various examples.

### Build for Windows

Requirements:

  * [git](https://gitforwindows.org/),
  * [cmake v3.28 minimum](https://gitforwindows.org/),
  * [Python v3.8 minimum](https://www.python.org/downloads/),

Under Windows, you need to explicitly set Visual Studio generator.

`cmake.exe -S root_path -B CI -G "Visual Studio 17 2022" -A x64`

Full example with git-bash console:

```sh
git clone https://github.com/moduleus/urx.git
cd urx
git clone https://github.com/microsoft/vcpkg.git
vcpkg/bootstrap-vcpkg.bat
git clone https://github.com/moduleus/vcpkg-registry.git
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake -DBUILD_SHARED_LIBS=OFF -DVCPKG_TARGET_TRIPLET=x64-wsmep -DVCPKG_HOST_TRIPLET=x64-wsmep -DVCPKG_OVERLAY_TRIPLETS=vcpkg-registry/triplets -DWITH_PYTHON:BOOL=ON -DWITH_PYTHON_WHL:BOOL=ON -DWITH_HDF5:BOOL=ON -DWITH_MATLAB:BOOL=ON -DBUILD_TESTING:BOOL=OFF -DCMAKE_INSTALL_PREFIX=install
cmake --build build --config Release --parallel 4
```

### Build for Linux

`cmake -S . -B CI`

Full example:

```sh
git clone https://github.com/moduleus/urx.git
cd urx
git clone https://github.com/microsoft/vcpkg.git
vcpkg/bootstrap-vcpkg.sh
git clone https://github.com/moduleus/vcpkg-registry.git
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DVCPKG_TARGET_TRIPLET=x64-lsrp -DVCPKG_HOST_TRIPLET=x64-lsrp -DVCPKG_OVERLAY_TRIPLETS=vcpkg-registry/triplets -DWITH_PYTHON:BOOL=ON -DWITH_PYTHON_WHL:BOOL=ON -DWITH_HDF5:BOOL=ON -DWITH_MATLAB:BOOL=ON -DBUILD_TESTING:BOOL=OFF -DCMAKE_INSTALL_PREFIX=install
cmake --build build --parallel 4
```

### Common features

  * Install path

Set `-DCMAKE_INSTALL_PREFIX=` variable to define the installation path.

  * Debug or Release

If you use a single build type generator (like Makefile in Linux), you need to set `CMAKE_BUILD_TYPE` to `Debug` or `Release`.

  * Static or dynamic libraries

Set `BUILD_SHARED_LIBS` to `OFF` or `ON`.

Note that you should set `BUILD_SHARED_LIBS` to `OFF` if you want to build MATLAB or Python buiding.

Be sure to use a vcpkg triplet with the same shared / static link option. For example:
  - Windows and static: `x64-wsmep`
  - Windows and shared: `x64-wdrep`
  - Linux and static: `x64-lsrp`
  - Linux and shared: `x64-ldr`

  * Use external 3rd party with vcpkg

`vcpkg` is a package manager. You first need to clone it with `git clone https://github.com/microsoft/vcpkg.git`.

Also, clone `vcpkg-registry` with `git clone https://github.com/moduleus/vcpkg-registry.git`.

Then pass the arguments below. ⚠ If you use `cmake-gui`, be sure to set these variables BEFORE running configure once. Otherwise, you need to reset cache.

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
  - `sm` for Windows static with `/MD` option (will be dynamic otherwise). See [VCPKG_CRT_LINKAGE](https://learn.microsoft.com/en-us/vcpkg/users/triplets#vcpkg_crt_linkage),
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

## CMake installed files

All CMake files will be installed in `INSTALL_PATH/lib/cmake/Urx-XXXYYY/` with:
  - XXX: the version of the Urx library (i.e. `1.0.2`),
  - YYY: empty if library has been built in shared mode, `_static` if library is static.

## C++ installed files

All files will be installed in :

  - `INSTALL_PATH/include/Urx-XXXYYY/` (headers),
  - `INSTALL_PATH/lib` (dynamic libraries for Linux and static libraries for all operating system),
  - `INSTALL_PATH/bin` (dynamic libraries for Windows and executables for all operating system).
