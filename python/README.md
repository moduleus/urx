# Build

```bash
python -m venv .venv
```

```bash
source ./.venv/bin/activate
.\.venv\Scripts\Activate.ps1
```

```bash
python -m pip install -r requirements-gitlab.txt
python -m pip install . -vvv --config-settings="--global-option=CMAKE_TOOLCHAIN_FILE=D:\\repos\\urx\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake cmake_build_type=RelWithDebInfo" --no-clean
```

```bash
python -m unittest discover test/python
```
