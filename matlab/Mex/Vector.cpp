#include <mexplus/mexplus.h>

#include <cuda.h>

#include <eGal/Cuda/Macros.cuh>

using namespace mexplus;

class Stream {
 public:
  Stream(const bool is_null) : _ptr{nullptr} {
    if (!is_null) EGAL_CUDA_CALL(cuStreamCreate(&_ptr, CU_STREAM_DEFAULT));
  }

  ~Stream() noexcept(false) {
    if (_ptr) EGAL_CUDA_CALL(cuStreamDestroy(_ptr));
  }

  void synchronize() const { EGAL_CUDA_CALL(cuStreamSynchronize(_ptr)); }

  CUstream ptr() const { return _ptr; }

 private:
  CUstream _ptr;
};

template class Session<Stream>;

namespace {

MEX_DEFINE(new)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  output.set(0, Session<Stream>::create(new Stream(input.get<bool>(0))));
}

MEX_DEFINE(delete)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 0);
  Session<Stream>::destroy(input.get(0));
}

MEX_DEFINE(synchronize)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 0);
  Stream* stream = Session<Stream>::get(input.get(0));
  stream->synchronize();
}

MEX_DEFINE(ptr)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  InputArguments input(nrhs, prhs, 1);
  OutputArguments output(nlhs, plhs, 1);
  Stream* stream = Session<Stream>::get(input.get(0));
  output.set(0, (intptr_t)stream->ptr());
}

}  // namespace

MEX_DISPATCH