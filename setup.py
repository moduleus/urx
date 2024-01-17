import inspect
import os
import sys

import importlib.metadata as imp
import cmake_build_extension
import setuptools

from setuptools import setup, find_packages
from pathlib import Path


CMAKE_TOOLCHAIN_FILE = next(
    (arg for arg in sys.argv if arg.startswith("CMAKE_TOOLCHAIN_FILE")), None)
if CMAKE_TOOLCHAIN_FILE != None:
    sys.argv.remove(CMAKE_TOOLCHAIN_FILE)
    if sys.platform == 'win32':
        TRIPLET = "x64-windows-static-md"
    else:
        TRIPLET = "x64-linux"
else:
    raise Exception(
        'Missing CMAKE_TOOLCHAIN_FILE for VCPKG in --global-option')

init_py = inspect.cleandoc(
    f"""
    import cmake_build_extension

    with cmake_build_extension.build_extension_env():
        from .bindings import *
    """
)

setuptools.setup(
    ext_modules=[
        cmake_build_extension.CMakeExtension(
            name="pyurx",
            install_prefix="pyurx",
            write_top_level_init=init_py,
            source_dir=str(Path(__file__).parent.absolute()),
            cmake_configure_options=[
                "-DWITH_PYTHON:BOOL=ON",
                "-DWITH_HDF5:BOOL=OFF",
                "-DBUILD_SHARED_LIBS:BOOL=OFF",
                "-DCALL_FROM_SETUP_PY:BOOL=ON",
                "-DBUILD_TESTING:BOOL=OFF",
                f"-DVCPKG_TARGET_TRIPLET={TRIPLET}",
                f"-DVCPKG_HOST_TRIPLET={TRIPLET}",
                f"-D{CMAKE_TOOLCHAIN_FILE}",
                "-DVCPKG_MANIFEST_MODE:BOOL=ON",
                f"-DVCPKG_MANIFEST_DIR={str(Path(__file__).parent.absolute())}",
                f"-DPython3_EXECUTABLE={sys.executable}"
            ],
            # Disable Ninja build generator
            cmake_generator=None
        )
    ],
    cmdclass=dict(
        build_ext=cmake_build_extension.BuildExtension
    ),
    version="0.0.1"
)
