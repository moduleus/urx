#include <mexplus/mexplus.h>
#include <typeinfo>

#include <complex>
#include <iostream>
#include <memory>
#include <vector>

#include <uff/acquisition.h>
#include <uff/group_data.h>

using namespace mexplus;

namespace {

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


}  // namespace

// MEX_DISPATCH
