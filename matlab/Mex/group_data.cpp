#include <mexplus.h>
#include <typeinfo>

#include <complex>
#include <iostream>
#include <memory>
#include <vector>

#include <uff/acquisition.h>
#include <uff/group_data.h>

using namespace mexplus;

namespace {

std::map<mxClassID, uff::GroupData::DataType> mxClass2UffDataTypeMap = {
    {mxINT16_CLASS, uff::GroupData::DataType::INT16},
    {mxINT32_CLASS, uff::GroupData::DataType::INT32},
    {mxSINGLE_CLASS, uff::GroupData::DataType::FLOAT},
    {mxDOUBLE_CLASS, uff::GroupData::DataType::DOUBLE}};

std::map<uff::GroupData::DataType, mxClassID> uffDataType2MxClassMap = {
    {uff::GroupData::DataType::INT16, mxINT16_CLASS},
    {uff::GroupData::DataType::INT32, mxINT32_CLASS},
    {uff::GroupData::DataType::FLOAT, mxSINGLE_CLASS},
    {uff::GroupData::DataType::DOUBLE, mxDOUBLE_CLASS}};

MEX_DEFINE(GroupData_new)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 0);
  OutputArguments output(nlhs, plhs, 1);
  output.set(0, Session<uff::GroupData>::create(new uff::GroupData()));
}

MEX_DEFINE(GroupData_delete)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 0);
  Session<uff::GroupData>::destroy(input.get(0));
}

MEX_DEFINE(GroupData_get_group_timestamp)
(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));
  output.set(0, grp_data->group_timestamp);
}

MEX_DEFINE(GroupData_set_group_timestamp)
(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));
  grp_data->group_timestamp = input.get<double>(1);
}

MEX_DEFINE(GroupData_get_size_of_data_type)
(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));
  output.set(0, grp_data->size_of_data_type);
}

MEX_DEFINE(GroupData_set_size_of_data_type)
(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));
  grp_data->size_of_data_type = input.get<uint8_t>(1);
}

MEX_DEFINE(GroupData_get_data_type)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));
  output.set(0, int32_t(grp_data->data_type));
}

MEX_DEFINE(GroupData_set_data_type)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));
  grp_data->data_type = uff::GroupData::DataType(input.get<int32_t>(1));
}

MEX_DEFINE(GroupData_get_sequence_timestamps)
(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));
  output.set(0, grp_data->sequence_timestamps);
}

MEX_DEFINE(GroupData_set_sequence_timestamps)
(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);

  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));
  grp_data->sequence_timestamps = input.get<std::vector<double>>(1);
}

MEX_DEFINE(GroupData_get_event_timestamps)
(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));
  output.set(0, grp_data->event_timestamps);
}

MEX_DEFINE(GroupData_set_event_timestamps)
(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);

  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));
  grp_data->event_timestamps = input.get<std::vector<std::vector<double>>>(1);
}

MEX_DEFINE(GroupData_get_void_raw_data_length)
(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));

  output.set(0, grp_data->void_raw_data_length);
}

// MEX_DEFINE(GroupData_set_void_raw_data_length)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
//   InputArguments input(nrhs, prhs, 2);
//   OutputArguments output(nlhs, plhs, 0);
//   std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));

//   printf("GroupData_set_void_raw_data_length %d\n",input.get<int>(1));
//   grp_data->void_raw_data_length = input.get<uint64_t>(1);
//   printf("GroupData_set_void_raw_data_length %d\n",grp_data->void_raw_data_length);
// }

MEX_DEFINE(GroupData_get_void_raw_data)
(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  printf("GroupData_get_void_raw_data\n");
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));
  printf("GroupData_get_void_raw_data uffDataType2MxClassMap[grp_data->data_type] %d\n",uffDataType2MxClassMap[grp_data->data_type]);
  printf("GroupData_get_void_raw_data grp_data->void_raw_data %d\n",grp_data->void_raw_data);
  printf("GroupData_get_void_raw_data grp_data->sampling_type %d\n",grp_data->sampling_type);
  printf("GroupData_get_void_raw_data grp_data->data_type %d\n",grp_data->data_type);
  printf("GroupData_get_void_raw_data grp_data->void_raw_data_length %d\n\n",grp_data->void_raw_data_length);

  for (int i=0; i<grp_data->void_raw_data_length; ++i) {
    switch (grp_data->data_type)
    {
    case uff::GroupData::DataType::DOUBLE:
      printf("i %d : %f\n",i,((double*)(grp_data->void_raw_data))[i]);
      break;
    case uff::GroupData::DataType::FLOAT:
      printf("i %d : %f\n",i,((float*)(grp_data->void_raw_data))[i]);
      break;
    case uff::GroupData::DataType::INT32:
      printf("i %d : %d\n",i,((int32_t*)(grp_data->void_raw_data))[i]);
      break;
    case uff::GroupData::DataType::INT16:
      printf("i %d : %d\n",i,((int16_t*)(grp_data->void_raw_data))[i]);
      break;
    
    default:
      break;
    }
  }
    

  // output.set(0, grp_data->void_raw_data);
  // mxArray *mxData = mxCreateNumericMatrix(
  //     0, 0, uffDataType2MxClassMap[grp_data->data_type],
  //     grp_data->sampling_type == uff::Group::SamplingType::IQ ? mxCOMPLEX : mxREAL);
  // mxSetM(mxData, 1);
  // mxSetN(mxData, grp_data->void_raw_data_length);
  // mxSetInt16s(mxData, (int16_t*)grp_data->void_raw_data);
  // auto* data = mxGetInt16s(mxData);
  // data[0] = 1;
  // data[1] = 2;
  // data[2] = 3;
  plhs[0] = mxCreateNumericMatrix(
      1, grp_data->void_raw_data_length, uffDataType2MxClassMap[grp_data->data_type],
      grp_data->sampling_type == uff::Group::SamplingType::IQ ? mxCOMPLEX : mxREAL);
  // mxSetM(plhs[0], 1);
  // mxSetN(plhs[0], grp_data->void_raw_data_length);
  mxSetInt16s(plhs[0], (mxInt16*)grp_data->void_raw_data);
  
  auto* data = mxGetInt16s(plhs[0]);
  for (int i=0; i<grp_data->void_raw_data_length; ++i) {
    switch (grp_data->data_type)
    {
    case uff::GroupData::DataType::DOUBLE:
      printf("data i %d : %f\n",i,((double*)(data))[i]);
      break;
    case uff::GroupData::DataType::FLOAT:
      printf("data i %d : %f\n",i,((float*)(data))[i]);
      break;
    case uff::GroupData::DataType::INT32:
      printf("data i %d : %d\n",i,((int32_t*)(data))[i]);
      break;
    case uff::GroupData::DataType::INT16:
      printf("data i %d : %d\n",i,((int16_t*)(data))[i]);
      break;
    
    default:
      break;
    }
  }
}

MEX_DEFINE(GroupData_set_void_raw_data)
(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  printf("GroupData_set_void_raw_data\n");
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));
  printf("GroupData_set_void_raw_data data_type %d\n",grp_data->data_type);
  printf("GroupData_set_void_raw_data sampling_type %d\n",grp_data->sampling_type);

  printf("1\n");
  const mxClassID category = mxGetClassID(prhs[1]);
  printf("2\n");
  const mwSize n = mxGetNumberOfElements(prhs[1]);  // Number of elements of input variable
  printf("3\n");
  bool isComplex = mxIsComplex(prhs[1]);
  printf("4\n");
  grp_data->void_raw_data = (int16_t*)mxGetData(prhs[1]);
  printf("5\n");
  grp_data->void_raw_data_length = n;
  printf("6\n");
  grp_data->data_type = mxClass2UffDataTypeMap[category];
  printf("7\n");
  grp_data->sampling_type =
      isComplex ? uff::Group::SamplingType::IQ : uff::Group::SamplingType::DIRECT_RF;
  printf("end\n");
}

MEX_DEFINE(GroupData_get_raw_data)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));

  std::visit([&](auto&& vec) { output.set(0, vec); }, grp_data->raw_data);
}

MEX_DEFINE(GroupData_set_raw_data)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);

  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));

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

MEX_DEFINE(GroupData_get_group)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));
  intptr_t id = reinterpret_cast<intptr_t>(grp_data->group.get());
  if (grp_data->group) {
    if (Session<uff::Group>::exist(id))
      output.set(0, id);
    else
      output.set(0, Session<uff::Group>::create_shared(grp_data->group));
  }
}

MEX_DEFINE(GroupData_set_group)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 3);
  OutputArguments output(nlhs, plhs, 0);

  std::shared_ptr<uff::GroupData> grp_data = Session<uff::GroupData>::get_shared(input.get(0));
  std::shared_ptr<uff::Group> grp = Session<uff::Group>::get_shared(input.get(1));
  std::shared_ptr<uff::Acquisition> acq = Session<uff::Acquisition>::get_shared(input.get(2));
  for (size_t i = 0; i < acq->groups.size(); ++i) {
    if (grp == acq->groups[i]) grp_data->group = acq->groups[i];
  }
}

}  // namespace

// MEX_DISPATCH