#include <mexplus.h>
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

MEX_DEFINE(Acquisition_get_groups)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  uff::Acquisition* acq = Session<uff::Acquisition>::get(input.get(0));
  // output.set(0, acq->groups);
}

MEX_DEFINE(Acquisition_get_i_group)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 1);
  uff::Acquisition* acq = Session<uff::Acquisition>::get(input.get(0));
  size_t grp_idx = input.get<size_t>(1);
  uff::Group* grp = acq->groups[grp_idx];
  intptr_t id = reinterpret_cast<intptr_t>(grp.get());
  if (Session<uff::Group>::exist(id))
    output.set(0, id);
  else
    output.set(0, Session<uff::Group>::create_shared(grp));
}

MEX_DEFINE(Acquisition_del_i_group)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  uff::Acquisition* acq = Session<uff::Acquisition>::get(input.get(0));
  size_t grp_idx = input.get<size_t>(1);
  if (grp_idx < acq->groups.size())
    acq->groups.erase(acq->groups.begin() + grp_idx);
  else
    throw std::runtime_error("Access violation: try to erase index above groups size");
}

MEX_DEFINE(Acquisition_add_group)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 2);
  OutputArguments output(nlhs, plhs, 0);
  uff::Acquisition* acq = Session<uff::Acquisition>::get(input.get(0));
  uff::Group* grp = Session<uff::Group>::get(input.get(1));
  bool already_in = false;
  for (size_t i = 0; i < acq->groups.size(); ++i) {
    if (grp == acq->groups[i]) {
      already_in = true;
    }
  }
  if (!already_in)
    acq->groups.push_back(uff::Group > (grp));
  else
    throw std::runtime_error("Group already referenced in the acquisition");
}

MEX_DEFINE(Acquisition_add_new_group)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 0);
  uff::Acquisition* acq = Session<uff::Acquisition>::get(input.get(0));
  acq->groups.push_back(std::make_shared<uff::Group>());
}

}  // namespace

// MEX_DISPATCH