# Changelog

## 1.3.2

### C++

#### What's new (improvement)

Speedup read of `vector<vector<uint32_t>>` from urx file.

### Python

#### Bug fixes

Fix setting stream.readerOptions / stream.writerOptions :

`stream.writerOptions().clean_unusable_data = True`

## 1.3.1

### C++

#### Bug fixes

Fix PImpl of Stream class (ABI break).

### MATLAB

#### Bug fixes

Fix validate dataset function.

### Python

#### Bug fixes

Restore dropped constructor for GroupDataReader.

## 1.3.0

### What's new (feature)

  * Add clone feature to duplicate entirely a Dataset via a deep copy. The produced dataset doesn't share any memory with the original Dataset that has been cloned.

  * Add check feature for all URX classes via a Validator. The check enables to detect/fix incoherent data in the classes.

  * Dataset could not be saved anymore if the dataset doesn't pass the check by default. However, the WriterOptions class enables to bypass the check through CheckData property.

  * Saving a file in an existing file now merges data.

#### What's new (improvement)

  * Better of errors handling.

### C++

#### What's new (feature)

  * Add an utils fonction `urx::utils::common:getElementIndex` (file `urx/utils/common.h`) to find the index of an urx class instance in a vector: ie. find the index of a probe in acquisition.probes vector. The search can be done using deep value comparison or pointer comparison.

  * Add an `valueComparison` fonction (file `urx/detail/compare.h`) to compare pointer by value rather by comparing adresses.

#### Bug fixes

  * Remove the `shared_ptr` and `weak_ptr` `operator==` overload have introduced weird behavior. Using `valueComparison` instead force to explicit the purpose of comparison by value. The comparison of `shared_ptr` and `weak_ptr` for these classes now have a different behavior, more suitable with the standard C++ code behavior:
    -  ElementGeometry
    -  Excitation
    -  Group
    -  GroupData
    -  ImpulseResponse
    -  Probe
    -  RawData
    -  Wave

### MATLAB

#### What's new (feature)

  * Binding: add Streaming, GroupDataReader.

  * Urx's field that contains vector of uint32 or array of cells of array of uint32 are now index. So value starts at 1.

### Python

#### What's new (feature)

  * Add Streaming binding.

#### What's new (improvement)

  * Better stubs for better auto-completion in IDE.

## 1.2.0

### C++

#### What's new (feature)

  * Add stream support for GroupData. RawData can not be loaded in memory at urx read time and can be write sequence by sequence. See [README](urx/utils/io/README.md).

  * Add various helpers for probe (`updateCurvilinearElementsPositions`, `updateRcaElementsRectGeometry`, `updateMatrixElementsRectGeometry`, `updateLinearElementsRectGeometry`, `updateCurvilinearElementsRectGeometry`, `createStandardRca`, `createStandardLinear`, `createStandardCurviLinear`, `createStandardMatrix`)

#### What's new (improvement)

  * Major refactor of urx reader / writer to merge it in a class.

### MATLAB

#### Bug fixes

  * `RawData_float_complex` and `RawData_float_real` couldn't be used in `GroupData.rawData`.

  * Don't free memory when an allocated object by MATLAB (`shared_ptr`) is assigned in a `weak_ptr` C++ field.

  * Reset property if PostSet throw exception.

  * Exception in PreGet / PostSet shouldn't break urx library and need to restart MATLAB.

  * Vector3D must be double number.

  * Fix append data in matrix (i.e. `probe.elements(end+1) = element`).

  * Invalidate fields if an allocated object by MATLAB (`shared_ptr`) is assigned in a raw C++ field.

  * Avoid warnings in MATLAB 2018.

  * Fix fields of inherited classes when they are initialized by `loadFromFile` function. 

  * Hide private field in `TransmitSetup` and `Wave` classes.

  * Invalidate MATLAB object when C++ reallocate memory

#### What's new (improvement)

  * Merge Windows and Linux toolbox.

### Python

#### Bug fixes

  * Fix memory leak when RawData is allocated on Python side.

## 1.1.0

### C++

#### Bug fixes

Fix default version number in Version class.

### MATLAB

#### What's new

Add support for MATLAB 2018.

Implement `eq` / `==` feature. These fonction compare C pointer and not content of fields.

Implement `isequaln` feature. This fonction compares content of fields. `isequal` do the same than `isequaln` function.

Handle exception for `urx.loadFromFile` / `urx.saveToFile`.

Add a constructor with 3 parameters to `Vector3D`.

#### API break

`urx.Dataset.saveToFile` and `urx.Dataset.loadFromFile` is deprecated. Use `urx.saveToFile` and `urx.loadFromFile` instead.

#### Bug fixes

Improve stability of MATLAB implementation.

### Python

#### What's new

Implement of `__float__`, `__array__` and `__repr__` for `DoubleNan` to ease usage.

Implement of `__array__` for `Vector3D` to ease usage and add a constructor that allow `np.array` of `list` with a size of 3.

Handle exception for `urx.loadFromFile` / `urx.saveToFile` with `ReadFileException` and `WriteFileException`.

Fix support for Python 3.8 to 3.13 and for pypy 3.8 to 3.10.

#### Bug fixes

`GroupData.raw_data` was release by garbage collector if it was allocated by Python.

## 1.0.1

Fix reader. Groups may have destination link to a group that has not yet been read.

## 1.0.0

Initial release.
