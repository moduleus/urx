#ifndef MATLAB_MACROS_HPP
#define MATLAB_MACROS_HPP

///
/// Define default constructor wrapper
///
#define MEX_DEFINE_DEFAULT_CONSTRUCTOR(__class)                            \
  MEX_DEFINE(new)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) { \
    InputArguments input(nrhs, prhs, 0);                                        \
    OutputArguments output(nlhs, plhs, 1);                                      \
    output.set(0, Session<__class>::create(new __class()));                     \
  }

///
/// Define default+copy constructor wrapper
///
#define MEX_DEFINE_DEFAULT_COPY_CONSTRUCTOR(__class)                          \
  MEX_DEFINE(new)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {    \
    InputArguments input;                                                          \
    input.define("default", 0);                                                    \
    input.define("copy", 1);                                                       \
    input.parse(nrhs, prhs);                                                       \
    OutputArguments output(nlhs, plhs, 1);                                         \
                                                                                   \
    __class* new_inst{nullptr};                                                    \
    if (input.is("default")) {                                                     \
      new_inst = new __class{};                                                    \
    } else {                                                                       \
      new_inst = new __class{*reinterpret_cast<__class*>(input.get<intptr_t>(0))}; \
    }                                                                              \
                                                                                   \
    output.set(0, Session<__class>::create(new_inst));                             \
  }

///
/// Define destructor wrapper
///
#define MEX_DEFINE_DESTRUCTOR(__class)                                        \
  MEX_DEFINE(delete)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) { \
    InputArguments input(nrhs, prhs, 1);                                           \
    OutputArguments output(nlhs, plhs, 0);                                         \
    Session<__class>::destroy(input.get(0));                                       \
  }

///
/// Define a getter/setter wrapper for usual numeric types
///
#define MEX_DEFINE_RW_NUMERIC_PROPERTY(__struct, __property, __getter_fn, __setter_fn) \
  MEX_DEFINE(__getter_fn)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {     \
    InputArguments input(nrhs, prhs, 1);                                                    \
    OutputArguments output(nlhs, plhs, 1);                                                  \
                                                                                            \
    __struct* inst = Session<__struct>::get(input.get(0));                                  \
    output.set(0, inst->__property);                                                        \
  }                                                                                         \
                                                                                            \
  MEX_DEFINE(__setter_fn)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {     \
    InputArguments input(nrhs, prhs, 2);                                                    \
    OutputArguments output(nlhs, plhs, 0);                                                  \
                                                                                            \
    __struct* inst = Session<__struct>::get(input.get(0));                                  \
    inst->__property = input.get<decltype(inst->__property)>(1);                            \
  }

///
/// Define a getter/setter wrapper for generic pointer types
///
#define MEX_DEFINE_RW_POINTER_PROPERTY(__struct, __property, __getter_fn, __setter_fn) \
  MEX_DEFINE(__getter_fn)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {     \
    InputArguments input(nrhs, prhs, 1);                                                    \
    OutputArguments output(nlhs, plhs, 1);                                                  \
                                                                                            \
    __struct* inst = Session<__struct>::get(input.get(0));                                  \
    output.set(0, reinterpret_cast<intptr_t>(inst->__property));                            \
  }                                                                                         \
                                                                                            \
  MEX_DEFINE(__setter_fn)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {     \
    InputArguments input(nrhs, prhs, 2);                                                    \
    OutputArguments output(nlhs, plhs, 0);                                                  \
                                                                                            \
    __struct* inst = Session<__struct>::get(input.get(0));                                  \
    inst->__property = reinterpret_cast<void*>(input.get<intptr_t>(1));                     \
  }

///
/// Define a getter/setter wrapper for enumeration types
///
#define MEX_DEFINE_RW_ENUMERATION_PROPERTY(__struct, __property, __getter_fn, __setter_fn) \
  MEX_DEFINE(__getter_fn)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {         \
    InputArguments input(nrhs, prhs, 1);                                                        \
    OutputArguments output(nlhs, plhs, 1);                                                      \
                                                                                                \
    __struct* inst = Session<__struct>::get(input.get(0));                                      \
    output.set(0, static_cast<int32_t>(inst->__property));                                      \
  }                                                                                             \
                                                                                                \
  MEX_DEFINE(__setter_fn)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {         \
    InputArguments input(nrhs, prhs, 2);                                                        \
    OutputArguments output(nlhs, plhs, 0);                                                      \
                                                                                                \
    __struct* inst = Session<__struct>::get(input.get(0));                                      \
    inst->__property = static_cast<decltype(inst->__property)>(input.get<int32_t>(1));          \
  }

#endif  // MATLAB_MACROS_HPP
