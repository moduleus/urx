#include <mexplus/mexplus.h>
#include <typeinfo>

#include <memory>
#include <vector>

#include <uff/acquisition.h>

using namespace mexplus;

namespace {

MEX_DEFINE(Acquisition_new)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 0);
  OutputArguments output(nlhs, plhs, 1);
  output.set(0, Session<uff::Acquisition>::create(new uff::Acquisition()));
}

MEX_DEFINE(Acquisition_delete)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 0);
  Session<uff::Acquisition>::destroy(input.get(0));
}


}  // namespace

// MEX_DISPATCH
