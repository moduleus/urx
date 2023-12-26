#include "UrxLibBinding.h"

#include <chrono>
#include <cstdint>
#include <format>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/receive_setup.h>

#define xstr(s) str(s)
#define str(s) #s

static size_t alloc_cout = 0;

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
        std::cout << std::format("Urx Matlab wrapper: {} memory leaks\n", alloc_cout);
      }
      break;
  }
  return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
#else
#endif

// NOLINTBEGIN(cppcoreguidelines-owning-memory,bugprone-macro-parentheses)

#define _VECTOR_RAW_IMPL(snake, type)                                                           \
  void *CONCAT3(vector, snake, new)() {                                                         \
    alloc_cout++;                                                                               \
    return new std::vector<type>();                                                             \
  }                                                                                             \
  void CONCAT3(vector, snake, delete)(void *this_ptr) {                                         \
    alloc_cout--;                                                                               \
    delete static_cast<std::vector<type> *>(this_ptr);                                          \
  }                                                                                             \
  void CONCAT3(vector, snake, clear)(void *this_ptr) {                                          \
    static_cast<std::vector<type> *>(this_ptr)->clear();                                        \
  }                                                                                             \
  void CONCAT3(vector, snake, push_back)(void *this_ptr, void *val) {                           \
    static_cast<std::vector<type> *>(this_ptr)->push_back(*static_cast<type *>(val));           \
  }                                                                                             \
  void CONCAT4(vector, snake, push_back, shared)(void *this_ptr, void *val) {                   \
    static_cast<std::vector<type> *>(this_ptr)->push_back(                                      \
        *static_cast<std::shared_ptr<type> *>(val)->get());                                     \
  }                                                                                             \
  uint64_t CONCAT3(vector, snake, size)(void *this_ptr) {                                       \
    return static_cast<std::vector<type> *>(this_ptr)->size();                                  \
  }                                                                                             \
  void *CONCAT3(vector, snake, data)(void *this_ptr, uint64_t i) {                              \
    return &(*static_cast<std::vector<type> *>(this_ptr))[i];                                   \
  }                                                                                             \
  void CONCAT3(vector, snake, copy)(void *this_ptr, void *other_ptr) {                          \
    *static_cast<std::vector<type> *>(this_ptr) = *static_cast<std::vector<type> *>(other_ptr); \
  }

#define VECTOR_RAW_NS_IMPL(ns, name) _VECTOR_RAW_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define VECTOR_RAW_NS2_IMPL(ns, name_snake, name_real) \
  _VECTOR_RAW_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define VECTOR_RAW_IMPL(name) _VECTOR_RAW_IMPL(name, name)

#define _VECTOR_2D_RAW_IMPL(snake, type)                                    \
  void *CONCAT3(vector_2d, snake, new)() {                                  \
    alloc_cout++;                                                           \
    return new std::vector<std::vector<type>>();                            \
  }                                                                         \
  void CONCAT3(vector_2d, snake, delete)(void *this_ptr) {                  \
    alloc_cout--;                                                           \
    delete static_cast<std::vector<std::vector<type>> *>(this_ptr);         \
  }                                                                         \
  void CONCAT3(vector_2d, snake, clear)(void *this_ptr) {                   \
    static_cast<std::vector<std::vector<type>> *>(this_ptr)->clear();       \
  }                                                                         \
  void CONCAT3(vector_2d, snake, push_back)(void *this_ptr, void *val) {    \
    static_cast<std::vector<std::vector<type>> *>(this_ptr)->push_back(     \
        *static_cast<std::vector<type> *>(val));                            \
  }                                                                         \
  uint64_t CONCAT3(vector_2d, snake, size)(void *this_ptr) {                \
    return static_cast<std::vector<std::vector<type>> *>(this_ptr)->size(); \
  }                                                                         \
  void *CONCAT3(vector_2d, snake, data)(void *this_ptr, uint64_t i) {       \
    return &(*static_cast<std::vector<std::vector<type>> *>(this_ptr))[i];  \
  }                                                                         \
  void CONCAT3(vector_2d, snake, copy)(void *this_ptr, void *other_ptr) {   \
    *static_cast<std::vector<std::vector<type>> *>(this_ptr) =              \
        *static_cast<std::vector<std::vector<type>> *>(other_ptr);          \
  }

#define VECTOR_2D_RAW_NS_IMPL(ns, name) _VECTOR_2D_RAW_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define VECTOR_2D_RAW_NS2_IMPL(ns, name_snake, name_real) \
  _VECTOR_2D_RAW_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define VECTOR_2D_RAW_IMPL(name) _VECTOR_2D_RAW_IMPL(name, name)

#define _VECTOR_SHARED_IMPL(snake, type)                                             \
  void *CONCAT3(vector_shared, snake, new)() {                                       \
    alloc_cout++;                                                                    \
    return new std::vector<std::shared_ptr<type>>();                                 \
  }                                                                                  \
  void CONCAT3(vector_shared, snake, delete)(void *this_ptr) {                       \
    alloc_cout--;                                                                    \
    delete static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr);              \
  }                                                                                  \
  void CONCAT3(vector_shared, snake, clear)(void *this_ptr) {                        \
    static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr)->clear();            \
  }                                                                                  \
  void CONCAT3(vector_shared, snake, push_back)(void *this_ptr, void *val) {         \
    static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr)->push_back(          \
        *static_cast<std::shared_ptr<type> *>(val));                                 \
  }                                                                                  \
  void CONCAT4(vector_shared, snake, push_back, shared)(void *this_ptr, void *val) { \
    static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr)->push_back(          \
        *static_cast<std::shared_ptr<type> *>(val));                                 \
  }                                                                                  \
  uint64_t CONCAT3(vector_shared, snake, size)(void *this_ptr) {                     \
    return static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr)->size();      \
  }                                                                                  \
  void *CONCAT3(vector_shared, snake, data)(void *this_ptr, uint64_t i) {            \
    return &(*static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr))[i];       \
  }                                                                                  \
  void CONCAT3(vector_shared, snake, copy)(void *this_ptr, void *other_ptr) {        \
    *static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr) =                   \
        *static_cast<std::vector<std::shared_ptr<type>> *>(other_ptr);               \
  }

#define VECTOR_SHARED_NS_IMPL(ns, name) _VECTOR_SHARED_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define VECTOR_SHARED_NS2_IMPL(ns, name_snake, name_real) \
  _VECTOR_SHARED_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define VECTOR_SHARED_IMPL(name) _VECTOR_SHARED_IMPL(name, name)

#define _VECTOR_WEAK_IMPL(snake, type)                                                   \
  void *CONCAT3(vector_weak, snake, new)() {                                             \
    alloc_cout++;                                                                        \
    return new std::vector<std::weak_ptr<type>>();                                       \
  }                                                                                      \
  void CONCAT3(vector_weak, snake, delete)(void *this_ptr) {                             \
    alloc_cout--;                                                                        \
    delete static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr);                    \
  }                                                                                      \
  void CONCAT3(vector_weak, snake, clear)(void *this_ptr) {                              \
    static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr)->clear();                  \
  }                                                                                      \
  void CONCAT3(vector_weak, snake, push_back)(void *this_ptr, void *val) {               \
    static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr)->push_back(                \
        *static_cast<std::weak_ptr<type> *>(val));                                       \
  }                                                                                      \
  void CONCAT4(vector_weak, snake, push_back, shared)(void *this_ptr, void *val) {       \
    static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr)->push_back(                \
        *static_cast<std::weak_ptr<type> *>(val));                                       \
  }                                                                                      \
  uint64_t CONCAT3(vector_weak, snake, size)(void *this_ptr) {                           \
    return static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr)->size();            \
  }                                                                                      \
  void *CONCAT3(vector_weak, snake, data)(void *this_ptr, uint64_t i) {                  \
    return (*static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr))[i].lock().get(); \
  }                                                                                      \
  void CONCAT3(vector_weak, snake, copy)(void *this_ptr, void *other_ptr) {              \
    *static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr) =                         \
        *static_cast<std::vector<std::weak_ptr<type>> *>(other_ptr);                     \
  }

#define VECTOR_WEAK_NS_IMPL(ns, name) _VECTOR_WEAK_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define VECTOR_WEAK_NS2_IMPL(ns, name_snake, name_real) \
  _VECTOR_WEAK_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define VECTOR_WEAK_IMPL(name) _VECTOR_WEAK_IMPL(name, name)

#define _OBJECT_IMPL(snake, type)                                                       \
  void *CONCAT2(snake, new)(void) {                                                     \
    alloc_cout++;                                                                       \
    return new std::shared_ptr<type>(new type());                                       \
  }                                                                                     \
  void CONCAT2(snake, delete)(void *this_ptr) {                                         \
    alloc_cout--;                                                                       \
    delete static_cast<std::shared_ptr<type> *>(this_ptr);                              \
  }                                                                                     \
  void CONCAT4(snake, assign, raw, raw)(void *this_ptr, void *other_ptr) {              \
    *static_cast<type *>(this_ptr) = *static_cast<type *>(other_ptr);                   \
  }                                                                                     \
  void CONCAT4(snake, assign, shared, raw)(void *this_ptr, void *other_ptr) {           \
    **static_cast<std::shared_ptr<type> *>(this_ptr) = *static_cast<type *>(other_ptr); \
  }                                                                                     \
  void CONCAT4(snake, assign, raw, shared)(void *this_ptr, void *other_ptr) {           \
    *static_cast<type *>(this_ptr) = **static_cast<std::shared_ptr<type> *>(other_ptr); \
  }                                                                                     \
  void CONCAT4(snake, assign, weak, shared)(void *this_ptr, void *other_ptr) {          \
    *static_cast<std::weak_ptr<type> *>(other_ptr) =                                    \
        *static_cast<std::shared_ptr<type> *>(other_ptr);                               \
  }                                                                                     \
  void CONCAT4(snake, assign, shared, shared)(void *this_ptr, void *other_ptr) {        \
    **static_cast<std::shared_ptr<type> *>(this_ptr) =                                  \
        **static_cast<std::shared_ptr<type> *>(other_ptr);                              \
  }

#define OBJECT_NS_IMPL(ns, name) _OBJECT_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define OBJECT_NS2_IMPL(ns, name_snake, name_real) \
  _OBJECT_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define OBJECT_IMPL(name) _VECTOR_RAW_IMPL(name, name)

#define _OBJECT_ACCESSOR_IMPL(snake, type, member)                                                \
  void *CONCAT2(snake, member)(void *this_ptr) { return &static_cast<type *>(this_ptr)->member; } \
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

void std_string_set(void *this_ptr, const char *c) { *static_cast<std::string *>(this_ptr) = c; }
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
OBJECT_ACCESSOR_NS_IMPL(urx, Acquisition, waves)
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
//OBJECT_ACCESSOR_NS_IMPL(urx, GroupData, raw_data)
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

OBJECT_NS_IMPL(urx, ReceiveSetup)
OBJECT_ACCESSOR_NS_IMPL(urx, ReceiveSetup, probe)
OBJECT_ACCESSOR_NS_IMPL(urx, ReceiveSetup, probe_transform)
OBJECT_ACCESSOR_NS_IMPL(urx, ReceiveSetup, sampling_frequency)
OBJECT_ACCESSOR_NS_IMPL(urx, ReceiveSetup, number_samples)
OBJECT_ACCESSOR_NS_IMPL(urx, ReceiveSetup, channel_mapping)
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
OBJECT_ACCESSOR_NS_IMPL(urx, Wave, channel_mapping)
OBJECT_ACCESSOR_NS_IMPL(urx, Wave, channel_excitations)
OBJECT_ACCESSOR_NS_IMPL(urx, Wave, channel_delays)
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

// NOLINTEND(cppcoreguidelines-owning-memory,bugprone-macro-parentheses)
