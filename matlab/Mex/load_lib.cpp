#include <cstdio>
#include <cstdlib>

#include <mexplus.h>

namespace {
MEX_DEFINE(unlock)(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) { mexUnlock(); }
}  // namespace

MEX_DISPATCH

#ifdef __cplusplus
#define INITIALIZER(f)    \
  static void f(void);    \
  struct f##_t_ {         \
    f##_t_(void) { f(); } \
  };                      \
  static f##_t_ f##_;     \
  static void f(void)
#elif defined(_MSC_VER)
#pragma section(".CRT$XCU", read)
#define INITIALIZER2_(f, p)                                \
  static void f(void);                                     \
  __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
  __pragma(comment(linker, "/include:" p #f "_")) static void f(void)
#ifdef _WIN64
#define INITIALIZER(f) INITIALIZER2_(f, "")
#else
#define INITIALIZER(f) INITIALIZER2_(f, "_")
#endif
#else
#define INITIALIZER(f)                              \
  static void f(void) __attribute__((constructor)); \
  static void f(void)
#endif

static void finalize(void) { printf("finalize\n"); }

INITIALIZER(initialize) {
  printf("initialize\n");
  atexit(finalize);
}

int main(int argc, char** argv) {
  printf("main\n");
  return 0;
}