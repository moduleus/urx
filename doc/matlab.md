## Install MinGW C compiler

If you want to use a free compiler, follow the next steps:

  - Download `MinGW-w64` from [https://www.mingw-w64.org/downloads](https://www.mingw-w64.org/downloads). You may use `LLVM-MinGW` at Github [https://github.com/mstorsjo/llvm-mingw/releases](https://github.com/mstorsjo/llvm-mingw/releases) and get the latest release from the file `llvm-mingw-YYYYMMDD-msvcrt-x86_64.zip`.
  - Extract it and install it on `c:\llvm-mingw-YYYYMMDD-msvcrt-x86_64` (path `c:\llvm-mingw-YYYYMMDD-msvcrt-x86_64\bin` must exist) or anywhere but don't use path with space inside.
  - In MATLAB, run:
```MATLAB
setenv('MW_MINGW64_LOC', 'C:/llvm-mingw-YYYYMMDD-msvcrt-x86_64')
mex -setup c -v
mex -setup c++ -v
```
Check you have a message `Found installed compiler 'MinGW64 Compiler (C++)'.`.

## Installed files

All MATLAB files will be installed in `C:\Users\CURRENT_USER\AppData\Roaming\MathWorks\MATLAB Add-Ons\Toolboxes\urx` :

  - `+urx` (all MATLAB script),
  - `include` (C++ header, needed to get symbols in C++ library),
  - `UrxMatlabBinding.dll` (main C++ library).

## Troubleshoot

If you have an error like `MATLAB:mex:NoCompilerFound_link_Win64` / `No supported compiler was found` / `Output from preprocessor is:'cl' is not recognized as an internal or external command, operable program or batch file.`, run `mex -setup c -v` and `mex -setup c++ -v`.

Updating Visual Studio may break mex configuration.

<details>
  <summary>Example of full error message</summary>

```
... Looking for compiler 'Intel Parallel Studio XE 2015 for C++ with Microsoft Visual Studio 2015' ...
... Looking for environment variable 'ICPP_COMPILER15' ...No.
Did not find installed compiler 'Intel Parallel Studio XE 2015 for C++ with Microsoft Visual Studio 2015'.
... Looking for compiler 'Intel Parallel Studio XE 2016 for C++ with Microsoft Visual Studio 2015' ...
... Looking for environment variable 'ICPP_COMPILER16' ...No.
Did not find installed compiler 'Intel Parallel Studio XE 2016 for C++ with Microsoft Visual Studio 2015'.
... Looking for compiler 'Intel Parallel Studio XE 2017 for C++ with Microsoft Visual Studio 2015' ...
... Looking for environment variable 'ICPP_COMPILER17' ...No.
Did not find installed compiler 'Intel Parallel Studio XE 2017 for C++ with Microsoft Visual Studio 2015'.
... Looking for compiler 'Intel Parallel Studio XE 2017 for C++ with Microsoft Visual Studio 2017' ...
... Looking for environment variable 'ICPP_COMPILER17' ...No.
Did not find installed compiler 'Intel Parallel Studio XE 2017 for C++ with Microsoft Visual Studio 2017'.
... Looking for compiler 'Intel Parallel Studio XE 2018 for C++ with Microsoft Visual Studio 2015' ...
... Looking for environment variable 'ICPP_COMPILER18' ...No.
Did not find installed compiler 'Intel Parallel Studio XE 2018 for C++ with Microsoft Visual Studio 2015'.
... Looking for compiler 'Intel Parallel Studio XE 2018 for C++ with Microsoft Visual Studio 2017' ...
... Looking for environment variable 'ICPP_COMPILER18' ...No.
Did not find installed compiler 'Intel Parallel Studio XE 2018 for C++ with Microsoft Visual Studio 2017'.
... Looking for compiler 'MinGW64 Compiler (C++)' ...
... Looking for environment variable 'MW_MINGW64_LOC' ...No.
Did not find installed compiler 'MinGW64 Compiler (C++)'.
... Looking for compiler 'MinGW64 Compiler with Windows 10 SDK or later (C++)' ...
... Looking for environment variable 'MW_MINGW64_LOC' ...No.
Did not find installed compiler 'MinGW64 Compiler with Windows 10 SDK or later (C++)'.
... Looking for compiler 'Microsoft Visual C++ 2013' ...
... Looking for registry setting 'HKLM\SOFTWARE\Microsoft\VisualStudio\SxS\VS7' 12.0 ...No.
... Looking for registry setting 'HKCU\SOFTWARE\Microsoft\VisualStudio\SxS\VS7' 12.0 ...No.
... Looking for registry setting 'HKLM\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7' 12.0 ...No.
... Looking for registry setting 'HKCU\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7' 12.0 ...No.
Did not find installed compiler 'Microsoft Visual C++ 2013'.
... Looking for compiler 'Microsoft Visual C++ 2015' ...
... Looking for registry setting 'HKLM\SOFTWARE\Microsoft\VisualStudio\SxS\VC7' 14.0 ...No.
... Looking for registry setting 'HKCU\SOFTWARE\Microsoft\VisualStudio\SxS\VC7' 14.0 ...No.
... Looking for registry setting 'HKLM\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VC7' 14.0 ...No.
... Looking for registry setting 'HKCU\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VC7' 14.0 ...No.
Did not find installed compiler 'Microsoft Visual C++ 2015'.
... Looking for compiler 'Microsoft Visual C++ 2017' ...
... Looking for registry setting 'HKLM\SOFTWARE\Microsoft\VisualStudio\SxS\VS7' 15.0 ...No.
... Looking for registry setting 'HKCU\SOFTWARE\Microsoft\VisualStudio\SxS\VS7' 15.0 ...No.
... Looking for registry setting 'HKLM\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7' 15.0 ...No.
... Looking for registry setting 'HKCU\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7' 15.0 ...No.
Did not find installed compiler 'Microsoft Visual C++ 2017'.
... Looking for compiler 'MinGW64 Compiler (C++)' ...
... Looking for environment variable 'MW_MINGW64_LOC' ...No.
Did not find installed compiler 'MinGW64 Compiler (C++)'.
Error using mex
No supported compiler was found. You can install the freely
available MinGW-w64 C/C++ compiler; see Install MinGW-w64
Compiler. For more options, visit
https://www.mathworks.com/support/compilers.
```

</details>

## Log file

If you want to enable logging, set `URX_DEBUG` environment variable to:

  - `COUT` to display message to the standard output or,
  - the filename where you want to store log messages.

## Use URX outside of MATLAB toolbox

If you built Urx from source, before using `Urx` in MATLAB, you first need to:

  - allow utf-8 caracters,
  - load the urx library.

```MATLAB
addpath([pwd 'INSTALL_PATH/share/Urx-XXXYYY/matlab']);
feature('DefaultCharacterSet','UTF-8');
urx.LibBinding.getInstance([path to libUrxMatlabBinding.so], [path to INSTALL_PATH/include/Urx-XXXYYY/urx/matlab/bindings.h], {[path to INSTALL_PATH/include/Urx-XXXYYY]}, {});
```

⚠ When invoking `urx.LibBinding.getInstance` the first time, environment variable `HDF5_DISABLE_VERSION_CHECK` is set. MATLAB have it's own version of HDF5 library. Under Linux it will conflict with the one from Urx.

If you try to use `urx.saveToFile(...)` or `urx.loadFromFile(...)` without setting `HDF5_DISABLE_VERSION_CHECK` to `1` (warning only) or `2` (disable warning message), MATLAB will crash with error: `Headers are 1.8.23, library is 1.8.12`.

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

Why is there a problem? Under Linux, the same symbol from two differents libraries are merged. So you can't know which one will be used. By setting `HDF5_DISABLE_VERSION_CHECK` to `1`, you are disabling version check. Under Windows, the operating system keeps symbols from different libraries separate.
