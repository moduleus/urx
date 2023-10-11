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

MEX_DEFINE(get_sequence_timestamps)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  output.set(0, grp_data->sequence_timestamps);
}

MEX_DEFINE(set_sequence_timestamps)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 1);

  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  //   decltype(grp_data->sequence_timestamps.data()) data = Session<decltype(grp_data->sequence_timestamps.data())>::get(input.get(1));
  // input.get<double>(1, data);
  double* data;
  data = (double*)mxGetData(prhs[1]);

  mwSize n;
  n = mxGetNumberOfElements(prhs[1]);  // Number of elements of input variable

  //   grp_data->sequence_timestamps = decltype(grp_data->sequence_timestamps)(n);

  // std::copy(data, data+n, mxGetPr(plhs[0]));
  plhs[0] = mxCreateDoubleMatrix(1, n, mxREAL);
  std::copy(data, data + n, mxGetPr(plhs[0]));
  grp_data->sequence_timestamps = std::vector<double>(mxGetPr(plhs[0]), mxGetPr(plhs[0]) + n);
  // plhs[0]= (mxDouble*)grp_data->sequence_timestamps.data();
  // std::cout<<"grp_data->sequence_timestamps "<<std::endl;
  // for (int i=0;i<n;i++) std::cout<<grp_data->sequence_timestamps[i]<<std::endl;

  // plhs[0] = mxDuplicateArray(prhs[0]);
  // plhs[0] = mxCreateDoubleMatrix(1,(mwSize)n,mxREAL);
  // mxDouble* pointer;
  // pointer = mxGetPr(plhs[0]);

  // output.set(0, (mxDouble)grp_data->sequence_timestamps.data());

  // output.set(0, (double *)grp_data->sequence_timestamps.data());
  // mxSetData(plhs[0], grp_data->sequence_timestamps.data());
  // plhs[0] = grp_data->sequence_timestamps.data();
  // plhs[0] = mxGetPr(grp_data->sequence_timestamps.data())

  // std::cout<<"data output"<<std::endl;
  // for (int i=0;i<n;i++) std::cout<<output[0][i]<<std::endl;
  //   p = (double *) mxGetData(prhs[0]); // Pointer to output variable data
  //   for( i=0; i<n; i++ ) {
  //       p[i] *= 2.0; // Multiply the output elements by 2.0
  //   }
  //   GroupData* grp_data = Session<GroupData>::get(input.get(0));
  //   grp_data->sequence_timestamps = decltype(GroupData::sequence_timestamps)
}

template <typename T>
std::string to_string(const T& t) {
  std::ostringstream s;
  s << t;
  return s.str();
}

MEX_DEFINE(get_raw_data)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));

  // auto v = std::vector<std::complex<double>>{{1, 2}, {3, 4}};
  std::visit(
      [&](auto&& vec) {
        output.set(0, vec);
      },
      grp_data->raw_data);
}

MEX_DEFINE(set_raw_data)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 2);

  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));

  const mxClassID category = mxGetClassID(prhs[1]);
  const mwSize n = mxGetNumberOfElements(prhs[1]);  // Number of elements of input variable
  bool isComplex = mxIsComplex(prhs[1]);
  plhs[0] = mxCreateNumericArray(1, &n, category, mxComplexity(isComplex));
  std::string res;
  if (mxIsComplex(prhs[1])) {
    res += "complex ";

    switch (category) {
      case mxINT16_CLASS: {
        res += "mxINT16_CLASS\n";

        mxComplexInt16* data = (mxComplexInt16*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetComplexInt16s(plhs[0]));
        grp_data->raw_data =
            std::vector<std::complex<int16_t>>((std::complex<int16_t>*)mxGetData(plhs[0]),
                                               (std::complex<int16_t>*)mxGetData(plhs[0]) + n);
      } break;
      case mxINT32_CLASS: {
        res += "mxINT32_CLASS\n";

        mxComplexInt32* data = (mxComplexInt32*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetComplexInt32s(plhs[0]));
        grp_data->raw_data =
            std::vector<std::complex<int32_t>>((std::complex<int32_t>*)mxGetData(plhs[0]),
                                               (std::complex<int32_t>*)mxGetData(plhs[0]) + n);
      } break;
      case mxSINGLE_CLASS: {
        res += "mxSINGLE_CLASS\n";

        mxComplexSingle* data = (mxComplexSingle*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetComplexSingles(plhs[0]));
        grp_data->raw_data = std::vector<std::complex<float>>(
            (std::complex<float>*)mxGetData(plhs[0]), (std::complex<float>*)mxGetData(plhs[0]) + n);
      } break;
      case mxDOUBLE_CLASS: {
        res += "mxDOUBLE_CLASS\n";

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
        res += "mxINT16_CLASS\n";

        int16_t* data = (int16_t*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetInt16s(plhs[0]));
        grp_data->raw_data =
            std::vector<int16_t>((int16_t*)mxGetData(plhs[0]), (int16_t*)mxGetData(plhs[0]) + n);
      } break;
      case mxINT32_CLASS: {
        res += "mxINT32_CLASS\n";

        int32_t* data = (int32_t*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetInt32s(plhs[0]));
        grp_data->raw_data = std::vector<int32_t>(mxGetInt32s(plhs[0]), mxGetInt32s(plhs[0]) + n);
      } break;
      case mxSINGLE_CLASS: {
        res += "mxSINGLE_CLASS\n";

        float* data = (float*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetSingles(plhs[0]));
        grp_data->raw_data = std::vector<float>(mxGetSingles(plhs[0]), mxGetSingles(plhs[0]) + n);
      } break;
      case mxDOUBLE_CLASS: {
        res += "mxDOUBLE_CLASS\n";

        double* data = (double*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetDoubles(plhs[0]));
        grp_data->raw_data = std::vector<double>(mxGetDoubles(plhs[0]), mxGetDoubles(plhs[0]) + n);
      } break;
      default:
        break;
    }
  }
  std::visit(
      [&res](auto&& vec) {
        for (auto& v : vec) res += to_string(v) + "\n";
      },
      grp_data->raw_data);
  plhs[1] = mxCreateString(res.c_str());
}

MEX_DEFINE(create)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 0);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  grp_data->sequence_timestamps = {7, 8, 9};
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
