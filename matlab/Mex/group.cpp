#include <mexplus.h>

#include <iostream>
#include <memory>

#include <uff/group.h>

using namespace mexplus;

namespace {

MEX_DEFINE(new_Group)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 0);
  OutputArguments output(nlhs, plhs, 1);
  output.set(0, Session<uff::Group>::create(new uff::Group()));
}

MEX_DEFINE(delete_Group)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 0);
  Session<uff::Group>::destroy(input.get(0));
}

MEX_DEFINE(get_description)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::Group* grp_data = Session<uff::Group>::get(input.get(0));
  output.set(0, grp_data->description);
}

MEX_DEFINE(set_description)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  uff::Group* grp = Session<uff::Group>::get(input.get(0));
  printf("set_description\n");
  /* Examine input (right-hand-side) arguments. */
  mexPrintf("\n%d input argument(s).", nrhs);
  for (int i = 1; i < nrhs; i++) {
    mexPrintf("\n\tInput Arg %i is of type:\t%s ", i, mxGetClassName(prhs[i]));
  }
  std::string desc = mxArrayToString(prhs[0]);
  // std::string desc(mxGetChars(prhs[1]), mxGetChars(prhs[1]));
  
  printf("\n%s\n", desc);

  char* str;
  mxGetString(prhs[1], str, mxGetNumberOfElements(prhs[1]));

  printf("\n%s\n", str);

  printf("%s\n", input.get<std::string>(1));
  grp->description = input.get<std::string>(1);
}

MEX_DEFINE(get_sampling_type)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::Group* grp = Session<uff::Group>::get(input.get(0));
  output.set(0, int32_t(grp->sampling_type));
}

MEX_DEFINE(set_sampling_type)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  uff::Group* grp = Session<uff::Group>::get(input.get(0));
  grp->sampling_type = uff::Group::SamplingType(input.get<int32_t>(1));
}

}  // namespace

// MEX_DISPATCH