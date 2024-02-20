import inspect
import os
import sys

import importlib.metadata as imp
import cmake_build_extension
import setuptools

from setuptools import setup, find_packages
from pathlib import Path


CMAKE_TOOLCHAIN_FILE_arg = next(
    (arg for arg in sys.argv if arg.startswith("CMAKE_TOOLCHAIN_FILE")), None)
if CMAKE_TOOLCHAIN_FILE_arg != None:
    sys.argv.remove(CMAKE_TOOLCHAIN_FILE_arg)
    if sys.platform == 'win32':
        TRIPLET = "x64-windows-static-md"
    else:
        TRIPLET = "x64-linux"
else:
    raise Exception(
        'Missing CMAKE_TOOLCHAIN_FILE for VCPKG in --global-option')

cmake_build_type_arg = next(
    (arg for arg in sys.argv if arg.startswith("cmake_build_type=")), None)
if cmake_build_type_arg != None:
    sys.argv.remove(cmake_build_type_arg)
    cmake_build_type_arg = cmake_build_type_arg[len("cmake_build_type="):]
else:
    cmake_build_type_arg = "Release"

setuptools.setup(
    name="pyurx",
    ext_modules=[
        cmake_build_extension.CMakeExtension(
            name="pyurx",
            install_prefix="pyurx",
            source_dir=str(Path(__file__).parent.absolute()),
            cmake_build_type=f"{cmake_build_type_arg}",
            cmake_configure_options=[
                "-DWITH_PYTHON:BOOL=ON",
                "-DWITH_HDF5:BOOL=ON",
                "-DBUILD_SHARED_LIBS:BOOL=OFF",
                "-DCALL_FROM_SETUP_PY:BOOL=ON",
                "-DBUILD_TESTING:BOOL=OFF",
                f"-DVCPKG_TARGET_TRIPLET={TRIPLET}",
                f"-DVCPKG_HOST_TRIPLET={TRIPLET}",
                f"-D{CMAKE_TOOLCHAIN_FILE_arg}",
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
    version="0.0.1",
    packages=["pyurx"],
    package_dir={
        "pyurx": "python/pyurx",
    }
)
