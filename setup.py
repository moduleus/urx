import inspect
import os
import sys

import importlib.metadata as imp
import cmake_build_extension
import setuptools

from setuptools import setup, find_packages
from pathlib import Path


def find_package_filepath(package, filename):
    found_paths = [file.locate()
                   for file in imp.files(package) if file.name == filename]
    if not found_paths:
        raise Exception(f"No '{filename}' found in '{package}'.")
    return found_paths[0]


pybind11_dir_path = find_package_filepath(
    "pybind11", "pybind11Config.cmake").parent


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
                "-DBUILD_SHARED_LIBS:BOOL=OFF",
                "-DCALL_FROM_SETUP_PY:BOOL=ON",
                "-DBUILD_TESTING:BOOL=OFF",
                f"-Dpybind11_DIR={pybind11_dir_path}",
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
