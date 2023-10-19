#pragma once

#include <mexplus.h>
#include <mexplus/dispatch.h>
#include <typeinfo>

#include <complex>
#include <iostream>
#include <memory>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include <uff/group_data.h>

using namespace mexplus;

namespace {

MEX_DEFINE(newGroupData)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 0);
  OutputArguments output(nlhs, plhs, 1);
  output.set(0, Session<uff::GroupData>::create(new uff::GroupData()));
}

MEX_DEFINE(get_group_timestamp)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  output.set(0, grp_data->group_timestamp);
}

MEX_DEFINE(set_group_timestamp)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  grp_data->group_timestamp = input.get<double>(1);
  output.set(0, grp_data->group_timestamp);
}

MEX_DEFINE(get_size_of_data_type)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  output.set(0, grp_data->size_of_data_type);
}

MEX_DEFINE(set_size_of_data_type)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  grp_data->size_of_data_type = input.get<uint8_t>(1);
  output.set(0, grp_data->size_of_data_type);
}

MEX_DEFINE(get_sequence_timestamps)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  output.set(0, grp_data->sequence_timestamps);
}

MEX_DEFINE(set_sequence_timestamps)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);

  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  double* data = (double*)mxGetData(prhs[1]);
  mwSize n = mxGetNumberOfElements(prhs[1]);  // Number of elements of input variable
  grp_data->sequence_timestamps = std::vector<double>(data, data + n);
}

MEX_DEFINE(get_raw_data)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));

  std::visit(
      [&](auto&& vec) {
        output.set(0, vec);
      },
      grp_data->raw_data);
}

MEX_DEFINE(set_raw_data)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 1);

  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));

  const mxClassID category = mxGetClassID(prhs[1]);
  const mwSize n = mxGetNumberOfElements(prhs[1]);  // Number of elements of input variable
  bool isComplex = mxIsComplex(prhs[1]);
  plhs[0] = mxCreateNumericArray(1, &n, category, mxComplexity(isComplex));
  if (mxIsComplex(prhs[1])) {
    switch (category) {
      case mxINT16_CLASS: {
        mxComplexInt16* data = (mxComplexInt16*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetComplexInt16s(plhs[0]));
        grp_data->raw_data =
            std::vector<std::complex<int16_t>>((std::complex<int16_t>*)mxGetData(plhs[0]),
                                               (std::complex<int16_t>*)mxGetData(plhs[0]) + n);
      } break;
      case mxINT32_CLASS: {
        mxComplexInt32* data = (mxComplexInt32*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetComplexInt32s(plhs[0]));
        grp_data->raw_data =
            std::vector<std::complex<int32_t>>((std::complex<int32_t>*)mxGetData(plhs[0]),
                                               (std::complex<int32_t>*)mxGetData(plhs[0]) + n);
      } break;
      case mxSINGLE_CLASS: {
        mxComplexSingle* data = (mxComplexSingle*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetComplexSingles(plhs[0]));
        grp_data->raw_data = std::vector<std::complex<float>>(
            (std::complex<float>*)mxGetData(plhs[0]), (std::complex<float>*)mxGetData(plhs[0]) + n);
      } break;
      case mxDOUBLE_CLASS: {
        mxComplexDouble* data = (mxComplexDouble*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetComplexDoubles(plhs[0]));
        grp_data->raw_data =
            std::vector<std::complex<double>>((std::complex<double>*)mxGetData(plhs[0]),
                                              (std::complex<double>*)mxGetData(plhs[0]) + n);
      } break;
      default:
        break;
    }
  } else {
    switch (category) {
      case mxINT16_CLASS: {
        int16_t* data = (int16_t*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetInt16s(plhs[0]));
        grp_data->raw_data =
            std::vector<int16_t>((int16_t*)mxGetData(plhs[0]), (int16_t*)mxGetData(plhs[0]) + n);
      } break;
      case mxINT32_CLASS: {
        int32_t* data = (int32_t*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetInt32s(plhs[0]));
        grp_data->raw_data = std::vector<int32_t>(mxGetInt32s(plhs[0]), mxGetInt32s(plhs[0]) + n);
      } break;
      case mxSINGLE_CLASS: {
        float* data = (float*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetSingles(plhs[0]));
        grp_data->raw_data = std::vector<float>(mxGetSingles(plhs[0]), mxGetSingles(plhs[0]) + n);
      } break;
      case mxDOUBLE_CLASS: {
        double* data = (double*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetDoubles(plhs[0]));
        grp_data->raw_data = std::vector<double>(mxGetDoubles(plhs[0]), mxGetDoubles(plhs[0]) + n);
      } break;
      default:
        break;
    }
  }
}

MEX_DEFINE(unlock)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 0);
  OutputArguments output(nlhs, plhs, 0);
  mexUnlock();
}

}  // namespace

MEX_DISPATCH

#ifdef __cplusplus
#define INITIALIZER(f)    \
  static void f(void);    \
  struct f##_t_ {         \
    f##_t_(void) { f(); } \
  };                      \
  static f##_t_ f##_;     \
  static void f(void)
#elif defined(_MSC_VER)
#pragma section(".CRT$XCU", read)
#define INITIALIZER2_(f, p)                                \
  static void f(void);                                     \
  __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
  __pragma(comment(linker, "/include:" p #f "_")) static void f(void)
#ifdef _WIN64
#define INITIALIZER(f) INITIALIZER2_(f, "")
#else
#define INITIALIZER(f) INITIALIZER2_(f, "_")
#endif
#else
#define INITIALIZER(f)                              \
  static void f(void) __attribute__((constructor)); \
  static void f(void)
#endif

static void finalize(void) { printf("finalize\n"); }

INITIALIZER(initialize) {
  printf("initialize\n");
  atexit(finalize);
}

int main(int argc, char** argv) {
  printf("main\n");
  return 0;
}
