# Changelog

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
