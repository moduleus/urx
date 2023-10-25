#include <mexplus.h>
#include <typeinfo>

#include <array>
#include <memory>
#include <vector>


#include <uff/dataset.h>

using namespace mexplus;

namespace {

MEX_DEFINE(new_Dataset)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 0);
  OutputArguments output(nlhs, plhs, 1);
  output.set(0, Session<uff::Dataset>::create(new uff::Dataset()));
}

MEX_DEFINE(delete_Dataset)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 0);
  Session<uff::Dataset>::destroy(input.get(0));
}

MEX_DEFINE(get_Dataset_version)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 3);
  std::shared_ptr<uff::Dataset> dataset = Session<uff::Dataset>::get_shared(input.get(0));
  output.set(0, dataset->version.major);
  output.set(1, dataset->version.minor);
  output.set(2, dataset->version.patch);
}

MEX_DEFINE(set_Dataset_version)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  std::shared_ptr<uff::Dataset> dataset = Session<uff::Dataset>::get_shared(input.get(0));
  auto version_prop = input.get<std::vector<uint16_t>>(1);
  dataset->version = {.major=version_prop[0], .minor=version_prop[1], .patch=version_prop[2]};
}

}  // namespace

// MEX_DISPATCH