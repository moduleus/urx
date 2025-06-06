# Changelog

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
