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
python -m pip install . -vvv --config-settings="--global-option=CMAKE_TOOLCHAIN_FILE=D:\\repos\\urx\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake cmake_build_type=RelWithDebInfo" --no-clean
```

If you do minor changes to the C++ code, you may open project in `urx/build/temp.win-amd64-cpython-310/Release_pyurx`, select `RelWithDebInfo`, build, and run `cp ./urx/build/temp.win-amd64-cpython-310/Release_pyurx/python/RelWithDebInfo/bindings.cp310-win_amd64.pyd ./urx/.venv/Lib/site-packages/pyurx/bindings.cp310-win_amd64.pyd`.

  * Launch tests

```bash
python -m unittest discover test/python
```
