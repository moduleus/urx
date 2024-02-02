#include "UrxLibBinding.h"

#include <complex>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iosfwd>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/raw_data.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/receive_setup.h>
#include <urx/transform.h>
#include <urx/transmit_setup.h>
#include <urx/vector.h>
#include <urx/version.h>
#include <urx/wave.h>

#ifdef WITH_HDF5
#include <urx/utils/io/reader.h>
#include <urx/utils/io/writer.h>
#endif

#define xstr(s) str(s)
#define str(s) #s

namespace {
size_t alloc_cout = 0;

constexpr bool log_new_delete = false;

std::ostream &getLog() {
  if constexpr (log_new_delete) {
    static std::ofstream outfile("c:\\temp\\test.txt");
    return outfile;
  } else {
    std::ostream &outfile(std::cout);
    return outfile;
  }
}
}  // namespace

#if WIN32
#include <windows.h>

BOOL WINAPI DllMain(HINSTANCE /*hinstDLL*/,  // handle to DLL module
                    DWORD fdwReason,         // reason for calling function
                    LPVOID /*lpReserved*/)   // reserved
{
  // Perform actions based on the reason for calling.
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      // Initialize once for each new process.
      // Return FALSE to fail DLL load.
      break;

    case DLL_THREAD_ATTACH:
      // Do thread-specific initialization.
      break;

    case DLL_THREAD_DETACH:
      // Do thread-specific cleanup.
      break;

    case DLL_PROCESS_DETACH:
      if (alloc_cout != 0) {
        getLog() << "Urx Matlab wrapper: " << alloc_cout << " memory leaks\n";
      }
      break;
  }
  return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
#else
#endif

// NOLINTBEGIN(cppcoreguidelines-owning-memory,bugprone-macro-parentheses)

#define _VECTOR_RAW_IMPL(snake, type)                                                            \
  void *CONCAT3(vector, snake, new)() {                                                          \
    alloc_cout++;                                                                                \
    auto retval = new std::vector<type>();                                                       \
    getLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n" << std::flush;   \
    return retval;                                                                               \
  }                                                                                              \
  void CONCAT3(vector, snake, delete)(void *this_ptr) {                                          \
    alloc_cout--;                                                                                \
    getLog() << reinterpret_cast<size_t>(this_ptr) << " " << __FUNCTION__ << "\n" << std::flush; \
    delete static_cast<std::vector<type> *>(this_ptr);                                           \
  }                                                                                              \
  void CONCAT3(vector, snake, clear)(void *this_ptr) {                                           \
    static_cast<std::vector<type> *>(this_ptr)->clear();                                         \
  }                                                                                              \
  void CONCAT3(vector, snake, push_back)(void *this_ptr, void *val) {                            \
    static_cast<std::vector<type> *>(this_ptr)->push_back(*static_cast<type *>(val));            \
  }                                                                                              \
  void CONCAT4(vector, snake, push_back, shared)(void *this_ptr, void *val) {                    \
    static_cast<std::vector<type> *>(this_ptr)->push_back(                                       \
        *static_cast<std::shared_ptr<type> *>(val)->get());                                      \
  }                                                                                              \
  uint64_t CONCAT3(vector, snake, size)(void *this_ptr) {                                        \
    return static_cast<std::vector<type> *>(this_ptr)->size();                                   \
  }                                                                                              \
  void *CONCAT3(vector, snake, data)(void *this_ptr, uint64_t i) {                               \
    return &(*static_cast<std::vector<type> *>(this_ptr))[i];                                    \
  }                                                                                              \
  void CONCAT3(vector, snake, assign)(void *this_ptr, void *other_ptr) {                         \
    *static_cast<std::vector<type> *>(this_ptr) = *static_cast<std::vector<type> *>(other_ptr);  \
  }

#define VECTOR_RAW_NS_IMPL(ns, name) _VECTOR_RAW_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define VECTOR_RAW_NS2_IMPL(ns, name_snake, name_real) \
  _VECTOR_RAW_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define VECTOR_RAW_IMPL(name) _VECTOR_RAW_IMPL(name, name)

#define _VECTOR_2D_RAW_IMPL(snake, type)                                                         \
  void *CONCAT3(vector_2d, snake, new)() {                                                       \
    alloc_cout++;                                                                                \
    auto retval = new std::vector<std::vector<type>>();                                          \
    getLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n" << std::flush;   \
    return retval;                                                                               \
  }                                                                                              \
  void CONCAT3(vector_2d, snake, delete)(void *this_ptr) {                                       \
    alloc_cout--;                                                                                \
    getLog() << reinterpret_cast<size_t>(this_ptr) << " " << __FUNCTION__ << "\n" << std::flush; \
    delete static_cast<std::vector<std::vector<type>> *>(this_ptr);                              \
  }                                                                                              \
  void CONCAT3(vector_2d, snake, clear)(void *this_ptr) {                                        \
    static_cast<std::vector<std::vector<type>> *>(this_ptr)->clear();                            \
  }                                                                                              \
  void CONCAT3(vector_2d, snake, push_back)(void *this_ptr, void *val) {                         \
    static_cast<std::vector<std::vector<type>> *>(this_ptr)->push_back(                          \
        *static_cast<std::vector<type> *>(val));                                                 \
  }                                                                                              \
  uint64_t CONCAT3(vector_2d, snake, size)(void *this_ptr) {                                     \
    return static_cast<std::vector<std::vector<type>> *>(this_ptr)->size();                      \
  }                                                                                              \
  void *CONCAT3(vector_2d, snake, data)(void *this_ptr, uint64_t i) {                            \
    return &(*static_cast<std::vector<std::vector<type>> *>(this_ptr))[i];                       \
  }                                                                                              \
  void CONCAT3(vector_2d, snake, assign)(void *this_ptr, void *other_ptr) {                      \
    *static_cast<std::vector<std::vector<type>> *>(this_ptr) =                                   \
        *static_cast<std::vector<std::vector<type>> *>(other_ptr);                               \
  }

#define VECTOR_2D_RAW_NS_IMPL(ns, name) _VECTOR_2D_RAW_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define VECTOR_2D_RAW_NS2_IMPL(ns, name_snake, name_real) \
  _VECTOR_2D_RAW_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define VECTOR_2D_RAW_IMPL(name) _VECTOR_2D_RAW_IMPL(name, name)

#define _VECTOR_SHARED_IMPL(snake, type)                                                         \
  void *CONCAT3(vector_shared, snake, new)() {                                                   \
    alloc_cout++;                                                                                \
    auto retval = new std::vector<std::shared_ptr<type>>();                                      \
    getLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n" << std::flush;   \
    return retval;                                                                               \
  }                                                                                              \
  void CONCAT3(vector_shared, snake, delete)(void *this_ptr) {                                   \
    alloc_cout--;                                                                                \
    getLog() << reinterpret_cast<size_t>(this_ptr) << " " << __FUNCTION__ << "\n" << std::flush; \
    delete static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr);                          \
  }                                                                                              \
  void CONCAT3(vector_shared, snake, clear)(void *this_ptr) {                                    \
    static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr)->clear();                        \
  }                                                                                              \
  void CONCAT3(vector_shared, snake, push_back)(void *this_ptr, void *val) {                     \
    static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr)->push_back(                      \
        *static_cast<std::shared_ptr<type> *>(val));                                             \
  }                                                                                              \
  void CONCAT4(vector_shared, snake, push_back, shared)(void *this_ptr, void *val) {             \
    static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr)->push_back(                      \
        *static_cast<std::shared_ptr<type> *>(val));                                             \
  }                                                                                              \
  uint64_t CONCAT3(vector_shared, snake, size)(void *this_ptr) {                                 \
    return static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr)->size();                  \
  }                                                                                              \
  void *CONCAT3(vector_shared, snake, data)(void *this_ptr, uint64_t i) {                        \
    return &(*static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr))[i];                   \
  }                                                                                              \
  void CONCAT3(vector_shared, snake, assign)(void *this_ptr, void *other_ptr) {                  \
    *static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr) =                               \
        *static_cast<std::vector<std::shared_ptr<type>> *>(other_ptr);                           \
  }

#define VECTOR_SHARED_NS_IMPL(ns, name) _VECTOR_SHARED_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define VECTOR_SHARED_NS2_IMPL(ns, name_snake, name_real) \
  _VECTOR_SHARED_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define VECTOR_SHARED_IMPL(name) _VECTOR_SHARED_IMPL(name, name)

#define _VECTOR_WEAK_IMPL(snake, type)                                                           \
  void *CONCAT3(vector_weak, snake, new)() {                                                     \
    alloc_cout++;                                                                                \
    auto retval = new std::vector<std::weak_ptr<type>>();                                        \
    getLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n" << std::flush;   \
    return retval;                                                                               \
  }                                                                                              \
  void CONCAT3(vector_weak, snake, delete)(void *this_ptr) {                                     \
    alloc_cout--;                                                                                \
    getLog() << reinterpret_cast<size_t>(this_ptr) << " " << __FUNCTION__ << "\n" << std::flush; \
    delete static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr);                            \
  }                                                                                              \
  void CONCAT3(vector_weak, snake, clear)(void *this_ptr) {                                      \
    static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr)->clear();                          \
  }                                                                                              \
  void CONCAT3(vector_weak, snake, push_back)(void *this_ptr, void *val) {                       \
    static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr)->push_back(                        \
        *static_cast<std::weak_ptr<type> *>(val));                                               \
  }                                                                                              \
  void CONCAT4(vector_weak, snake, push_back, shared)(void *this_ptr, void *val) {               \
    static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr)->push_back(                        \
        *static_cast<std::weak_ptr<type> *>(val));                                               \
  }                                                                                              \
  uint64_t CONCAT3(vector_weak, snake, size)(void *this_ptr) {                                   \
    return static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr)->size();                    \
  }                                                                                              \
  void *CONCAT3(vector_weak, snake, data)(void *this_ptr, uint64_t i) {                          \
    return (*static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr))[i].lock().get();         \
  }                                                                                              \
  void CONCAT3(vector_weak, snake, assign)(void *this_ptr, void *other_ptr) {                    \
    *static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr) =                                 \
        *static_cast<std::vector<std::weak_ptr<type>> *>(other_ptr);                             \
  }

#define VECTOR_WEAK_NS_IMPL(ns, name) _VECTOR_WEAK_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define VECTOR_WEAK_NS2_IMPL(ns, name_snake, name_real) \
  _VECTOR_WEAK_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define VECTOR_WEAK_IMPL(name) _VECTOR_WEAK_IMPL(name, name)

#define _OBJECT_IMPL(snake, type)                                                                  \
  void *CONCAT2(snake, new)(void) {                                                                \
    alloc_cout++;                                                                                  \
    auto retval = new std::shared_ptr<type>(new type());                                           \
    getLog() << reinterpret_cast<size_t>(retval) << " " << reinterpret_cast<size_t>(retval->get()) \
             << " " << __FUNCTION__ << "\n"                                                        \
             << std::flush;                                                                        \
    return retval;                                                                                 \
  }                                                                                                \
  void CONCAT2(snake, delete)(void *this_ptr) {                                                    \
    alloc_cout--;                                                                                  \
    getLog() << reinterpret_cast<size_t>(this_ptr) << " "                                          \
             << reinterpret_cast<size_t>(static_cast<std::shared_ptr<type> *>(this_ptr)->get())    \
             << " " << static_cast<std::shared_ptr<type> *>(this_ptr)->use_count() << " "          \
             << __FUNCTION__ << "\n"                                                               \
             << std::flush;                                                                        \
    delete static_cast<std::shared_ptr<type> *>(this_ptr);                                         \
  }                                                                                                \
  void CONCAT4(snake, assign, raw, raw)(void *this_ptr, void *other_ptr) {                         \
    *static_cast<type *>(this_ptr) = *static_cast<type *>(other_ptr);                              \
  }                                                                                                \
  void CONCAT4(snake, assign, raw, shared)(void *this_ptr, void *other_ptr) {                      \
    *static_cast<type *>(this_ptr) = **static_cast<std::shared_ptr<type> *>(other_ptr);            \
  }                                                                                                \
  void CONCAT4(snake, assign, weak, shared)(void *this_ptr, void *other_ptr) {                     \
    *static_cast<std::weak_ptr<type> *>(this_ptr) =                                                \
        *static_cast<std::shared_ptr<type> *>(other_ptr);                                          \
  }                                                                                                \
  void CONCAT4(snake, assign, shared, shared)(void *this_ptr, void *other_ptr) {                   \
    *static_cast<std::shared_ptr<type> *>(this_ptr) =                                              \
        *static_cast<std::shared_ptr<type> *>(other_ptr);                                          \
  }

#define _RAW_DATA_SHARED_NS_IMPL_real_shared_size(name, type_data)                             \
  void *CONCAT5(name, type_data, real, shared, size)(void *this_ptr) {                         \
    static uint64_t retval;                                                                    \
    retval =                                                                                   \
        (*static_cast<std::shared_ptr<urx::RawDataNoInit<type_data>> *>(this_ptr))->getSize(); \
    return &retval;                                                                            \
  }
#define _RAW_DATA_SHARED_NS_IMPL_complex_shared_size(name, type_data)                            \
  void *CONCAT5(name, type_data, complex, shared, size)(void *this_ptr) {                        \
    static uint64_t retval;                                                                      \
    retval =                                                                                     \
        (*static_cast<std::shared_ptr<urx::RawDataNoInit<std::complex<type_data>>> *>(this_ptr)) \
            ->getSize();                                                                         \
    return &retval;                                                                              \
  }

#define _RAW_DATA_SHARED_NS_IMPL_real_shared_data(name, type_data)                    \
  void *CONCAT5(name, type_data, real, shared, data)(void *this_ptr) {                \
    return (*static_cast<std::shared_ptr<urx::RawDataNoInit<type_data>> *>(this_ptr)) \
        ->getBuffer();                                                                \
  }
#define _RAW_DATA_SHARED_NS_IMPL_complex_shared_data(name, type_data)                     \
  void *CONCAT5(name, type_data, complex, shared, data)(void *this_ptr) {                 \
    return (*static_cast<std::shared_ptr<urx::RawDataNoInit<std::complex<type_data>>> *>( \
                this_ptr))                                                                \
        ->getBuffer();                                                                    \
  }

#define _RAW_DATA_SHARED_NS_IMPL(name, type_data, type_number)           \
  _RAW_DATA_SHARED_NS_IMPL_##type_number##_shared_size(name, type_data); \
  _RAW_DATA_SHARED_NS_IMPL_##type_number##_shared_data(name, type_data);

#define RAW_DATA_SHARED_NS_IMPL(ns, name, type_data)           \
  _RAW_DATA_SHARED_NS_IMPL(CONCAT2(ns, name), type_data, real) \
  _RAW_DATA_SHARED_NS_IMPL(CONCAT2(ns, name), type_data, complex)
#define RAW_DATA_SHARED_IMPL(name, type_data)     \
  _RAW_DATA_SHARED_NS_IMPL(name, type_data, real) \
  _RAW_DATA_SHARED_NS_IMPL(name, type_data, complex)

#define OBJECT_NS_IMPL(ns, name) _OBJECT_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define OBJECT_NS2_IMPL(ns, name_snake, name_real) \
  _OBJECT_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define OBJECT_IMPL(name) _VECTOR_RAW_IMPL(name, name)

#define _OBJECT_RAW_DATA_IMPL(snake, type, other_type)                                             \
  void *CONCAT2(snake, new)(uint64_t size) {                                                       \
    alloc_cout++;                                                                                  \
    auto retval = new std::shared_ptr<other_type>(new other_type(size));                           \
    getLog() << reinterpret_cast<size_t>(retval) << " " << reinterpret_cast<size_t>(retval->get()) \
             << " " << __FUNCTION__ << "\n"                                                        \
             << std::flush;                                                                        \
    return retval;                                                                                 \
  }                                                                                                \
  void CONCAT2(snake, delete)(void *this_ptr) {                                                    \
    alloc_cout--;                                                                                  \
    getLog() << reinterpret_cast<size_t>(this_ptr) << " "                                          \
             << reinterpret_cast<size_t>(                                                          \
                    static_cast<std::shared_ptr<other_type> *>(this_ptr)->get())                   \
             << " " << static_cast<std::shared_ptr<other_type> *>(this_ptr)->use_count() << " "    \
             << __FUNCTION__ << "\n"                                                               \
             << std::flush;                                                                        \
    delete static_cast<std::shared_ptr<other_type> *>(this_ptr);                                   \
  }                                                                                                \
  void CONCAT4(snake, assign, shared, shared)(void *this_ptr, void *other_ptr) {                   \
    *static_cast<std::shared_ptr<type> *>(this_ptr) =                                              \
        std::dynamic_pointer_cast<type>(*static_cast<std::shared_ptr<other_type> *>(other_ptr));   \
  }

#define OBJECT_NS_RAW_DATA_IMPL(ns, name, t1, t2, other_name) \
  _OBJECT_RAW_DATA_IMPL(CONCAT4(ns, name, t1, t2), CONCAT_NS(ns, name), other_name)

#define _OBJECT_ACCESSOR_IMPL(snake, type, member)                                                \
  void *CONCAT2(snake, member)(void *this_ptr) { return &static_cast<type *>(this_ptr)->member; } \
  void *CONCAT3(snake, weak, member)(void *this_ptr) {                                            \
    return &static_cast<std::weak_ptr<type> *>(this_ptr)->lock()->member;                         \
  }                                                                                               \
  void *CONCAT3(snake, shared, member)(void *this_ptr) {                                          \
    return &static_cast<std::shared_ptr<type> *>(this_ptr)->get()->member;                        \
  }

#define OBJECT_ACCESSOR_NS_IMPL(ns, name, member) \
  _OBJECT_ACCESSOR_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name), member)
#define OBJECT_ACCESSOR_NS2_IMPL(ns, name_snake, name_real, member) \
  _OBJECT_ACCESSOR_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real), member)
#define OBJECT_ACCESSOR_IMPL(name, member) _OBJECT_ACCESSOR_IMPL(name, name, member)

uint64_t ptr2val(void *p) { return reinterpret_cast<uint64_t>(p); }
void *val2ptr(uint64_t v) { return reinterpret_cast<void *>(v); }

void std_string_set(void *this_ptr, const char *v) { *static_cast<std::string *>(this_ptr) = v; }
const char *std_string_get(void *this_ptr) { return static_cast<std::string *>(this_ptr)->c_str(); }

OBJECT_NS_IMPL(urx, Acquisition)
OBJECT_ACCESSOR_NS_IMPL(urx, Acquisition, authors)
OBJECT_ACCESSOR_NS_IMPL(urx, Acquisition, description)
OBJECT_ACCESSOR_NS_IMPL(urx, Acquisition, local_time)
OBJECT_ACCESSOR_NS_IMPL(urx, Acquisition, country_code)
OBJECT_ACCESSOR_NS_IMPL(urx, Acquisition, system)
OBJECT_ACCESSOR_NS_IMPL(urx, Acquisition, sound_speed)
OBJECT_ACCESSOR_NS_IMPL(urx, Acquisition, timestamp)
OBJECT_ACCESSOR_NS_IMPL(urx, Acquisition, probes)
OBJECT_ACCESSOR_NS_IMPL(urx, Acquisition, excitations)
OBJECT_ACCESSOR_NS_IMPL(urx, Acquisition, groups)
OBJECT_ACCESSOR_NS_IMPL(urx, Acquisition, groups_data)

OBJECT_NS_IMPL(urx, Dataset)
OBJECT_ACCESSOR_NS_IMPL(urx, Dataset, version)
OBJECT_ACCESSOR_NS_IMPL(urx, Dataset, acquisition)

OBJECT_NS_IMPL(urx, ElementGeometry)
OBJECT_ACCESSOR_NS_IMPL(urx, ElementGeometry, perimeter)

OBJECT_NS_IMPL(urx, Element)
OBJECT_ACCESSOR_NS_IMPL(urx, Element, transform)
OBJECT_ACCESSOR_NS_IMPL(urx, Element, element_geometry)
OBJECT_ACCESSOR_NS_IMPL(urx, Element, impulse_response)

OBJECT_NS_IMPL(urx, Event)
OBJECT_ACCESSOR_NS_IMPL(urx, Event, transmit_setup)
OBJECT_ACCESSOR_NS_IMPL(urx, Event, receive_setup)

OBJECT_NS_IMPL(urx, Excitation)
OBJECT_ACCESSOR_NS_IMPL(urx, Excitation, pulse_shape)
OBJECT_ACCESSOR_NS_IMPL(urx, Excitation, transmit_frequency)
OBJECT_ACCESSOR_NS_IMPL(urx, Excitation, sampling_frequency)
OBJECT_ACCESSOR_NS_IMPL(urx, Excitation, waveform)

OBJECT_NS_IMPL(urx, GroupData)
OBJECT_ACCESSOR_NS_IMPL(urx, GroupData, group)
OBJECT_ACCESSOR_NS_IMPL(urx, GroupData, raw_data)
OBJECT_ACCESSOR_NS_IMPL(urx, GroupData, group_timestamp)
OBJECT_ACCESSOR_NS_IMPL(urx, GroupData, sequence_timestamps)
OBJECT_ACCESSOR_NS_IMPL(urx, GroupData, event_timestamps)

OBJECT_NS_IMPL(urx, Group)
OBJECT_ACCESSOR_NS_IMPL(urx, Group, sampling_type)
OBJECT_ACCESSOR_NS_IMPL(urx, Group, data_type)
OBJECT_ACCESSOR_NS_IMPL(urx, Group, description)
OBJECT_ACCESSOR_NS_IMPL(urx, Group, sequence)

OBJECT_NS_IMPL(urx, ImpulseResponse)
OBJECT_ACCESSOR_NS_IMPL(urx, ImpulseResponse, sampling_frequency)
OBJECT_ACCESSOR_NS_IMPL(urx, ImpulseResponse, time_offset)
OBJECT_ACCESSOR_NS_IMPL(urx, ImpulseResponse, units)
OBJECT_ACCESSOR_NS_IMPL(urx, ImpulseResponse, data)

OBJECT_NS_IMPL(urx, Probe)
OBJECT_ACCESSOR_NS_IMPL(urx, Probe, description)
OBJECT_ACCESSOR_NS_IMPL(urx, Probe, type)
OBJECT_ACCESSOR_NS_IMPL(urx, Probe, transform)
OBJECT_ACCESSOR_NS_IMPL(urx, Probe, element_geometries)
OBJECT_ACCESSOR_NS_IMPL(urx, Probe, impulse_responses)
OBJECT_ACCESSOR_NS_IMPL(urx, Probe, elements)

OBJECT_NS_RAW_DATA_IMPL(urx, RawData, int16_t, real, urx::RawDataNoInit<int16_t>);
OBJECT_NS_RAW_DATA_IMPL(urx, RawData, int16_t, complex, urx::RawDataNoInit<std::complex<int16_t>>);
OBJECT_NS_RAW_DATA_IMPL(urx, RawData, int32_t, real, urx::RawDataNoInit<int32_t>);
OBJECT_NS_RAW_DATA_IMPL(urx, RawData, int32_t, complex, urx::RawDataNoInit<std::complex<int32_t>>);
OBJECT_NS_RAW_DATA_IMPL(urx, RawData, float, real, urx::RawDataNoInit<float>);
OBJECT_NS_RAW_DATA_IMPL(urx, RawData, float, complex, urx::RawDataNoInit<std::complex<float>>);
OBJECT_NS_RAW_DATA_IMPL(urx, RawData, double, real, urx::RawDataNoInit<double>);
OBJECT_NS_RAW_DATA_IMPL(urx, RawData, double, complex, urx::RawDataNoInit<std::complex<double>>);

OBJECT_NS_IMPL(urx, ReceiveSetup)
OBJECT_ACCESSOR_NS_IMPL(urx, ReceiveSetup, probe)
OBJECT_ACCESSOR_NS_IMPL(urx, ReceiveSetup, probe_transform)
OBJECT_ACCESSOR_NS_IMPL(urx, ReceiveSetup, sampling_frequency)
OBJECT_ACCESSOR_NS_IMPL(urx, ReceiveSetup, number_samples)
OBJECT_ACCESSOR_NS_IMPL(urx, ReceiveSetup, active_elements)
OBJECT_ACCESSOR_NS_IMPL(urx, ReceiveSetup, tgc_profile)
OBJECT_ACCESSOR_NS_IMPL(urx, ReceiveSetup, tgc_sampling_frequency)
OBJECT_ACCESSOR_NS_IMPL(urx, ReceiveSetup, modulation_frequency)
OBJECT_ACCESSOR_NS_IMPL(urx, ReceiveSetup, time_offset)

OBJECT_NS_IMPL(urx, Transform)
OBJECT_ACCESSOR_NS_IMPL(urx, Transform, rotation)
OBJECT_ACCESSOR_NS_IMPL(urx, Transform, translation)

OBJECT_NS_IMPL(urx, TransmitSetup)
OBJECT_ACCESSOR_NS_IMPL(urx, TransmitSetup, probe)
OBJECT_ACCESSOR_NS_IMPL(urx, TransmitSetup, wave)
OBJECT_ACCESSOR_NS_IMPL(urx, TransmitSetup, active_elements)
OBJECT_ACCESSOR_NS_IMPL(urx, TransmitSetup, excitations)
OBJECT_ACCESSOR_NS_IMPL(urx, TransmitSetup, delays)
OBJECT_ACCESSOR_NS_IMPL(urx, TransmitSetup, probe_transform)
OBJECT_ACCESSOR_NS_IMPL(urx, TransmitSetup, time_offset)

OBJECT_NS2_IMPL(urx, Vector3D, Vector3D<double>)
OBJECT_ACCESSOR_NS2_IMPL(urx, Vector3D, Vector3D<double>, x)
OBJECT_ACCESSOR_NS2_IMPL(urx, Vector3D, Vector3D<double>, y)
OBJECT_ACCESSOR_NS2_IMPL(urx, Vector3D, Vector3D<double>, z)

OBJECT_NS2_IMPL(urx, Vector2D, Vector2D<double>)
OBJECT_ACCESSOR_NS2_IMPL(urx, Vector2D, Vector2D<double>, x)
OBJECT_ACCESSOR_NS2_IMPL(urx, Vector2D, Vector2D<double>, y)

OBJECT_NS_IMPL(urx, Version)
OBJECT_ACCESSOR_NS_IMPL(urx, Version, major)
OBJECT_ACCESSOR_NS_IMPL(urx, Version, minor)
OBJECT_ACCESSOR_NS_IMPL(urx, Version, patch)

OBJECT_NS_IMPL(urx, Wave)
OBJECT_ACCESSOR_NS_IMPL(urx, Wave, type)
OBJECT_ACCESSOR_NS_IMPL(urx, Wave, time_zero)
OBJECT_ACCESSOR_NS_IMPL(urx, Wave, time_zero_reference_point)
OBJECT_ACCESSOR_NS_IMPL(urx, Wave, parameters)

VECTOR_RAW_IMPL(double);
VECTOR_RAW_IMPL(uint32_t);

VECTOR_RAW_NS_IMPL(urx, Element)
VECTOR_RAW_NS_IMPL(urx, Event)
VECTOR_RAW_NS_IMPL(urx, GroupData)
VECTOR_RAW_NS2_IMPL(urx, Vector3D, Vector3D<double>)

VECTOR_2D_RAW_IMPL(double)
VECTOR_2D_RAW_IMPL(uint32_t)

VECTOR_SHARED_NS_IMPL(urx, Probe)
VECTOR_SHARED_NS_IMPL(urx, Excitation)
VECTOR_SHARED_NS_IMPL(urx, Wave)
VECTOR_SHARED_NS_IMPL(urx, Group)
VECTOR_SHARED_NS_IMPL(urx, ElementGeometry)
VECTOR_SHARED_NS_IMPL(urx, ImpulseResponse)
VECTOR_WEAK_NS_IMPL(urx, Excitation)

RAW_DATA_SHARED_NS_IMPL(urx, RawData, int16_t);
RAW_DATA_SHARED_NS_IMPL(urx, RawData, int32_t);
RAW_DATA_SHARED_NS_IMPL(urx, RawData, float);
RAW_DATA_SHARED_NS_IMPL(urx, RawData, double);

uint64_t get_pointer(void *ptr) { return reinterpret_cast<uint64_t>(ptr); }

void *load_from_file(const char *filename) {
#ifdef WITH_HDF5
  return new std::shared_ptr<urx::Dataset>(urx::utils::io::Reader::load_from_file(filename));
#else
  return nullptr;
#endif
}

void save_to_file(const char *filename, void *dataset) {
#ifdef WITH_HDF5
  urx::utils::io::Writer::saveToFile(filename,
                                     **static_cast<std::shared_ptr<urx::Dataset> *>(dataset));
#endif
}

// NOLINTEND(cppcoreguidelines-owning-memory,bugprone-macro-parentheses)
