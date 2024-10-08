import sys

import cmake_build_extension
import setuptools
import toml
import subprocess
import os

from pathlib import Path

import sysconfig

pyproject = toml.load("pyproject.toml")
name_project = pyproject["project"]["name"]
name_project_underscore = name_project.replace("-", "_")

cmake_build_type_arg = next((arg for arg in sys.argv if arg.startswith("cmake_build_type=")), None)
if cmake_build_type_arg != None:
    sys.argv.remove(cmake_build_type_arg)
    cmake_build_type_arg = cmake_build_type_arg[len("cmake_build_type=") :]
else:
    cmake_build_type_arg = "Release"

CMAKE_TOOLCHAIN_FILE_arg = next(
    (arg for arg in sys.argv if arg.startswith("CMAKE_TOOLCHAIN_FILE")), None
)
if CMAKE_TOOLCHAIN_FILE_arg != None:
    sys.argv.remove(CMAKE_TOOLCHAIN_FILE_arg)

DISABLE_VCPKG_arg = next((arg for arg in sys.argv if arg.startswith("DISABLE_VCPKG")), None)
if DISABLE_VCPKG_arg != None:
    sys.argv.remove(DISABLE_VCPKG_arg)

vcpkg_triplet_arg = next((arg for arg in sys.argv if arg.startswith("vcpkg_triplet=")), None)
if vcpkg_triplet_arg != None:
    sys.argv.remove(vcpkg_triplet_arg)
    vcpkg_triplet_arg = vcpkg_triplet_arg[len("vcpkg_triplet=") :]

if DISABLE_VCPKG_arg is None and CMAKE_TOOLCHAIN_FILE_arg is None:
    if not (Path(__file__).parent.absolute() / "vcpkg").is_dir():
        subprocess.run(
            [
                "git",
                "clone",
                "https://github.com/microsoft/vcpkg",
                str(Path(__file__).parent.absolute() / "vcpkg"),
            ]
        )
        if os.name == "nt":
            subprocess.run(
                [str(Path(__file__).parent.absolute() / "vcpkg" / "bootstrap-vcpkg.bat")]
            )
        else:
            subprocess.run([str(Path(__file__).parent.absolute() / "vcpkg" / "bootstrap-vcpkg.sh")])

    CMAKE_TOOLCHAIN_FILE_arg = "CMAKE_TOOLCHAIN_FILE=" + str(
        Path(__file__).parent.absolute() / "vcpkg" / "scripts" / "buildsystems" / "vcpkg.cmake"
    )

build_shared_libs_arg = next(
    (arg for arg in sys.argv if arg.startswith("-DBUILD_SHARED_LIBS=")), None
)
if build_shared_libs_arg != None:
    sys.argv.remove(build_shared_libs_arg)
    build_shared_libs_arg = build_shared_libs_arg[len("-DBUILD_SHARED_LIBS=") :]
else:
    build_shared_libs_arg = next(
        (arg for arg in sys.argv if arg.startswith("-DBUILD_SHARED_LIBS:BOOL=")), None
    )
    if build_shared_libs_arg != None:
        sys.argv.remove(build_shared_libs_arg)
        build_shared_libs_arg = build_shared_libs_arg[len("-DBUILD_SHARED_LIBS:BOOL=") :]
    else:
        build_shared_libs_arg = "OFF"

cmake_configure_options = []

if vcpkg_triplet_arg != None:
    VCPKG_TRIPLET = vcpkg_triplet_arg
else:
    if sys.maxsize > 2**32:
        if (
            os.environ.get("PYTHON_ARCH") is not None
            and os.environ.get("PYTHON_ARCH").upper() == "ARM64"
        ) or sysconfig.get_platform().endswith("arm64"):
            VCPKG_TRIPLET = "arm64-"
        else:
            VCPKG_TRIPLET = "x64-"
    else:
        VCPKG_TRIPLET = "x86-"

    if sys.platform == "win32":
        if VCPKG_TRIPLET == "arm64-":
            cmake_configure_options += ["-A", "arm64"]
        elif VCPKG_TRIPLET == "x64-":
            cmake_configure_options += ["-A", "x64"]
        else:
            cmake_configure_options += ["-A", "Win32"]

        VCPKG_TRIPLET += "w"

        if build_shared_libs_arg == "OFF":
            # static-md
            VCPKG_TRIPLET += "sm"
        else:
            # dynamic
            VCPKG_TRIPLET += "d"

        if cmake_build_type_arg != "Debug":
            # Release
            VCPKG_TRIPLET += "r"

        # Environment
        VCPKG_TRIPLET += "e"

        if build_shared_libs_arg == "OFF":
            # python dynamic
            VCPKG_TRIPLET += "p"

    else:
        VCPKG_TRIPLET += "l"

        if build_shared_libs_arg == "OFF":
            # static
            VCPKG_TRIPLET += "s"
        else:
            # dynamic
            VCPKG_TRIPLET += "d"

        if cmake_build_type_arg != "Debug":
            # Release
            VCPKG_TRIPLET += "r"

        if build_shared_libs_arg == "OFF":
            # python dynamic
            VCPKG_TRIPLET += "p"

hdf5_arg = next((arg for arg in sys.argv if arg.startswith("-DWITH_HDF5")), None)
if hdf5_arg != None:
    sys.argv.remove(hdf5_arg)
else:
    hdf5_arg = "-DWITH_HDF5=ON"

cmake_configure_options += [
    "-DWITH_PYTHON:BOOL=ON",
    hdf5_arg,
    f"-DBUILD_SHARED_LIBS:BOOL={build_shared_libs_arg}",
    "-DCALL_FROM_SETUP_PY:BOOL=ON",
    "-DBUILD_TESTING:BOOL=ON",
    f"-DPython3_EXECUTABLE={sys.executable}",
]

if CMAKE_TOOLCHAIN_FILE_arg is not None:
    cmake_configure_options += [
        f"-DVCPKG_TARGET_TRIPLET={VCPKG_TRIPLET}",
        f"-DVCPKG_HOST_TRIPLET={VCPKG_TRIPLET}",
        f"-DVCPKG_OVERLAY_TRIPLETS={str(Path(__file__).parent.absolute())}/vcpkg-registry/triplets",
        f"-D{CMAKE_TOOLCHAIN_FILE_arg}",
        "-DVCPKG_MANIFEST_MODE:BOOL=ON",
        f"-DVCPKG_MANIFEST_DIR={str(Path(__file__).parent.absolute())}",
    ]

d_arg = next((arg for arg in sys.argv if arg.startswith("-D")), None)
while d_arg:
    sys.argv.remove(d_arg)
    cmake_configure_options += [d_arg]
    d_arg = next((arg for arg in sys.argv if arg.startswith("-D")), None)

setuptools.setup(
    ext_modules=[
        cmake_build_extension.CMakeExtension(
            name=f"{name_project}",
            install_prefix=f"{name_project_underscore}",
            source_dir=str(Path(__file__).parent.absolute()),
            cmake_build_type=f"{cmake_build_type_arg}",
            cmake_configure_options=cmake_configure_options,
            # Disable Ninja build generator
            cmake_generator=None,
        )
    ],
    cmdclass=dict(build_ext=cmake_build_extension.BuildExtension),
    packages=[f"{name_project_underscore}"],
    package_dir={
        f"{name_project_underscore}": f"python/{name_project_underscore}",
    },
)
