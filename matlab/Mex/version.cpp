#include <mexplus.h>
#include <typeinfo>

#include <memory>
#include <vector>

#include <uff/version.h>

using namespace mexplus;

namespace {

MEX_DEFINE(new_Version)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 0);
  OutputArguments output(nlhs, plhs, 1);
  output.set(0, Session<uff::Version>::create(new uff::Version()));
}

MEX_DEFINE(delete_Version)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 0);
  Session<uff::Version>::destroy(input.get(0));
}

MEX_DEFINE(get_Version_major)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::Version* version = Session<uff::Version>::get(input.get(0));
  output.set(0, version->major);
}

MEX_DEFINE(set_Version_major)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  uff::Version* version = Session<uff::Version>::get(input.get(0));
  version->major = input.get<uint16_t>(1);
}

MEX_DEFINE(get_Version_minor)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::Version* version = Session<uff::Version>::get(input.get(0));
  output.set(0, version->minor);
}

MEX_DEFINE(set_Version_minor)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  uff::Version* version = Session<uff::Version>::get(input.get(0));
  version->minor = input.get<uint16_t>(1);
}

MEX_DEFINE(get_Version_patch)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::Version* version = Session<uff::Version>::get(input.get(0));
  output.set(0, version->patch);
}

MEX_DEFINE(set_Version_patch)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  uff::Version* version = Session<uff::Version>::get(input.get(0));
  version->patch = input.get<uint16_t>(1);
}

}  // namespace