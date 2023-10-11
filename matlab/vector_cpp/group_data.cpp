#pragma once

#include <typeinfo>
#include <mexplus.h>

#include <complex>
#include <iostream>
#include <memory>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include <uff/group_data.h>

using namespace mexplus;

// namespace uff {

/**
 * @brief The UFF class containing all the retrieved data from the acquisition
 */
// struct GroupData {
//   /// Timestamp of the group launch [s]
//   double group_timestamp = std::numeric_limits<double>::quiet_NaN();

//   /// Timestamp list for each sequence launch [s]
//   std::vector<double> sequence_timestamps = {0.1,1.1,2.1};
  
//   void printData() { for(auto& v: sequence_timestamps) std::cout<<v<<std::endl;}
// };

// }  // namespace uff

namespace {

MEX_DEFINE(new)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  std::cout<<"new uff::GroupData"<<std::endl;
  InputArguments input(nrhs, prhs, 0);
  OutputArguments output(nlhs, plhs, 1);
  output.set(0, Session<uff::GroupData>::create(new uff::GroupData()));
}

MEX_DEFINE(group_timestamp)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  std::cout<<"group_timestamp"<<std::endl;
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  output.set(0, grp_data->group_timestamp);
}

MEX_DEFINE(sequence_timestamps)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  std::cout<<"set_sequence_timestamps"<<std::endl;
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  output.set(0, grp_data->sequence_timestamps);
}


MEX_DEFINE(set_sequence_timestamps)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  std::cout<<"set_sequence_timestamps"<<std::endl;
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 1);
  
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
//   decltype(grp_data->sequence_timestamps.data()) data = Session<decltype(grp_data->sequence_timestamps.data())>::get(input.get(1));
  // input.get<double>(1, data);
  double *data;
  data = (double *) mxGetData(prhs[1]);

  mwSize n;
  n = mxGetNumberOfElements(prhs[1]); // Number of elements of input variable
  std::cout<<"data size : " <<n<<std::endl;
  std::cout<<"data input"<<std::endl;
  // std::cout<<data[0]<<std::endl;
  // std::cout<<data[1]<<std::endl;
  // for (int i=0;i<n;i++) std::cout<<p[i]<<std::endl;
  // for (int i=0;i<n;i++) p[i]*=2;
  // for (int i=0;i<n;i++) std::cout<<p[i]<<std::endl;

  
//   grp_data->sequence_timestamps = decltype(grp_data->sequence_timestamps)(n);

  // std::copy(data, data+n, mxGetPr(plhs[0]));
  plhs[0]= mxCreateDoubleMatrix(1, n, mxREAL);
  std::copy(data, data+n, mxGetPr(plhs[0]));
  grp_data->sequence_timestamps = std::vector<double>(mxGetPr(plhs[0]),mxGetPr(plhs[0])+n);
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

MEX_DEFINE(create)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  std::cout<<"create"<<std::endl;
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 0);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  grp_data->sequence_timestamps = {7,8,9};
}

MEX_DEFINE(printData)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  std::cout<<"printData"<<std::endl;
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 0);
  uff::GroupData* grp_data = Session<uff::GroupData>::get(input.get(0));
  for(auto& v: grp_data->sequence_timestamps) std::cout<<v<<std::endl;
}

MEX_DEFINE(unlock)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 0);
  OutputArguments output(nlhs, plhs, 0);
  mexUnlock();
}

}  // namespace

MEX_DISPATCH


#ifdef __cplusplus
    #define INITIALIZER(f) \
        static void f(void); \
        struct f##_t_ { f##_t_(void) { f(); } }; static f##_t_ f##_; \
        static void f(void)
#elif defined(_MSC_VER)
    #pragma section(".CRT$XCU",read)
    #define INITIALIZER2_(f,p) \
        static void f(void); \
        __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
        __pragma(comment(linker,"/include:" p #f "_")) \
        static void f(void)
    #ifdef _WIN64
        #define INITIALIZER(f) INITIALIZER2_(f,"")
    #else
        #define INITIALIZER(f) INITIALIZER2_(f,"_")
    #endif
#else
    #define INITIALIZER(f) \
        static void f(void) __attribute__((constructor)); \
        static void f(void)
#endif

static void finalize(void)
{
    printf( "finalize\n");
}

INITIALIZER( initialize)
{
    printf( "initialize\n");
    atexit( finalize);
}

int main( int argc, char** argv)
{
    printf( "main\n");
    return 0;
}

