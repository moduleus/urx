## Build Python module with pip wheel

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
