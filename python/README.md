# Build

  * Create the virtual environment

```bash
python -m venv .venv
```

```bash
source ./.venv/bin/activate
.\.venv\Scripts\Activate.ps1
```

  * Build for dev

```bash
python -m pip install -r requirements-gitlab.txt
python -m pip install . -v --no-clean \
  --config-settings="--global-option=CMAKE_TOOLCHAIN_FILE=D:\\repos\\urx\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" \
  --config-settings="--global-option=cmake_build_type=RelWithDebInfo" \
  --config-settings="--global-option=vcpkg_triplet=x64-wsmrep" \
  --config-settings="--global-option=build_shared_libs=OFF" \
  --config-settings="--global-option=-DWITH_HDF5=ON"
```

If you do minor changes to the C++ code, you may open project in `urx/build/temp.win-amd64-cpython-310/Release_urx`, select `RelWithDebInfo`, build, and run `cp ./urx/build/temp.win-amd64-cpython-310/Release_urx/python/RelWithDebInfo/bindings.cp310-win_amd64.pyd ./urx/.venv/Lib/site-packages/urx/bindings.cp310-win_amd64.pyd`.

  * Launch tests

```bash
python -m unittest discover test/python
```
