#pragma once

//#include <complex>
#include <iostream>
#include <memory>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

// namespace uff {

/**
 * @brief The UFF class containing all the retrieved data from the acquisition
 */
struct GroupData {
  /// Timestamp of the group launch [s]
  double group_timestamp = std::numeric_limits<double>::quiet_NaN();

  /// Timestamp list for each sequence launch [s]
  std::vector<double> sequence_timestamps = {0.1,1.1,2.1};
  
  size_t sequence_timestamps_size() const {return sequence_timestamps.size();}
  auto sequence_timestamps_ptr() {
    double* res = sequence_timestamps.data();
    return res;
    }
  
  void printData() { for(auto& v: sequence_timestamps) std::cout<<v<<std::endl;}
  
  void setSeq(std::vector<double> seq) { sequence_timestamps=std::move(seq);}
};


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

