#include <mexplus.h>
#include <typeinfo>

#include <complex>
#include <iostream>
#include <memory>
#include <vector>

#include <uff/group_data.h>

using namespace mexplus;

namespace {

MEX_DEFINE(new_GroupData)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 0);
  OutputArguments output(nlhs, plhs, 1);
  output.set(0, Session<uff::GroupData>::create(new uff::GroupData()));
}

MEX_DEFINE(delete_GroupData)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 0);
  Session<uff::GroupData>::destroy(input.get(0));
}

MEX_DEFINE(get_group_timestamp)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  output.set(0, grp_data->group_timestamp);
}

MEX_DEFINE(set_group_timestamp)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  grp_data->group_timestamp = input.get<double>(1);
}

MEX_DEFINE(get_size_of_data_type)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  output.set(0, grp_data->size_of_data_type);
}

MEX_DEFINE(set_size_of_data_type)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  grp_data->size_of_data_type = input.get<uint8_t>(1);
}

MEX_DEFINE(get_data_type)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  output.set(0, int32_t(grp_data->data_type));
}

MEX_DEFINE(set_data_type)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  grp_data->data_type = uff::GroupData::DataType(input.get<int32_t>(1));
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
  grp_data->sequence_timestamps = input.get<std::vector<double>>(1);
}

MEX_DEFINE(get_event_timestamps)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  output.set(0, grp_data->event_timestamps);
}

MEX_DEFINE(set_event_timestamps)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);

  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  grp_data->event_timestamps = input.get<std::vector<std::vector<double>>>(1);
}

MEX_DEFINE(get_raw_data)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));

  std::visit([&](auto&& vec) { output.set(0, vec); }, grp_data->raw_data);
}

MEX_DEFINE(set_raw_data)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);

  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));

  const mxClassID category = mxGetClassID(prhs[1]);
  const mwSize n = mxGetNumberOfElements(prhs[1]);  // Number of elements of input variable
  bool isComplex = mxIsComplex(prhs[1]);
  plhs[0] = mxCreateNumericArray(1, &n, category, mxComplexity(isComplex));
  if (mxIsComplex(prhs[1])) {
    switch (category) {
      case mxINT16_CLASS: {
        mxComplexInt16* data = (mxComplexInt16*)mxGetData(prhs[1]);
        grp_data->raw_data = std::vector<std::complex<int16_t>>((std::complex<int16_t>*)data,
                                                                (std::complex<int16_t>*)data + n);
      } break;
      case mxINT32_CLASS: {
        mxComplexInt32* data = (mxComplexInt32*)mxGetData(prhs[1]);
        grp_data->raw_data = std::vector<std::complex<int32_t>>((std::complex<int32_t>*)data,
                                                                (std::complex<int32_t>*)data + n);
      } break;
      case mxSINGLE_CLASS: {
        mxComplexSingle* data = (mxComplexSingle*)mxGetData(prhs[1]);
        grp_data->raw_data = std::vector<std::complex<float>>((std::complex<float>*)data,
                                                              (std::complex<float>*)data + n);
      } break;
      case mxDOUBLE_CLASS: {
        mxComplexDouble* data = (mxComplexDouble*)mxGetData(prhs[1]);
        std::copy(data, data + n, mxGetComplexDoubles(plhs[0]));
        grp_data->raw_data = std::vector<std::complex<double>>((std::complex<double>*)data,
                                                               (std::complex<double>*)data + n);
      } break;
      default:
        break;
    }
  } else {
    switch (category) {
      case mxINT16_CLASS: {
        int16_t* data = (int16_t*)mxGetData(prhs[1]);
        grp_data->raw_data = std::vector<int16_t>((int16_t*)data, (int16_t*)data + n);
      } break;
      case mxINT32_CLASS: {
        int32_t* data = (int32_t*)mxGetData(prhs[1]);
        grp_data->raw_data = std::vector<int32_t>(data, data + n);
      } break;
      case mxSINGLE_CLASS: {
        float* data = (float*)mxGetData(prhs[1]);
        grp_data->raw_data = std::vector<float>(data, data + n);
      } break;
      case mxDOUBLE_CLASS: {
        double* data = (double*)mxGetData(prhs[1]);
        grp_data->raw_data = std::vector<double>(data, data + n);
      } break;
      default:
        break;
    }
  }
}

}  // namespace

// MEX_DISPATCH