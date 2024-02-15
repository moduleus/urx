#ifndef URX_LIB_BINDING
#define URX_LIB_BINDING

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>  // IWYU pragma: keep
#else
#include <stddef.h>  // IWYU pragma: keep
#include <stdint.h>  // IWYU pragma: keep
#endif

#define CONCAT2(a, b) a##_##b
#define CONCAT3(a, b, c) a##_##b##_##c
#define CONCAT4(a, b, c, d) a##_##b##_##c##_##d
#define CONCAT5(a, b, c, d, e) a##_##b##_##c##_##d##_##e
#define CONCAT_NS(a, b) a::b
#define CONCAT_NS3(a, b, c) a::b::c

#define _VECTOR_RAW_DECL(name)                                              \
  void *CONCAT3(vector, name, new)(void);                                   \
  void CONCAT3(vector, name, delete)(void *this_ptr);                       \
  void CONCAT3(vector, name, clear)(void *this_ptr);                        \
  void CONCAT3(vector, name, push_back)(void *this_ptr, void *val);         \
  void CONCAT4(vector, name, push_back, shared)(void *this_ptr, void *val); \
  uint64_t CONCAT3(vector, name, size)(void *this_ptr);                     \
  void *CONCAT3(vector, name, data)(void *this_ptr, uint64_t i);            \
  void CONCAT3(vector, name, assign)(void *this_ptr, void *other_ptr)

#define VECTOR_RAW_NS_DECL(ns, type) _VECTOR_RAW_DECL(CONCAT2(ns, type))
#define VECTOR_RAW_DECL(type) _VECTOR_RAW_DECL(type)

#define _VECTOR_2D_RAW_DECL(name)                                      \
  void *CONCAT3(vector_2d, name, new)(void);                           \
  void CONCAT3(vector_2d, name, delete)(void *this_ptr);               \
  void CONCAT3(vector_2d, name, clear)(void *this_ptr);                \
  void CONCAT3(vector_2d, name, push_back)(void *this_ptr, void *val); \
  uint64_t CONCAT3(vector_2d, name, size)(void *this_ptr);             \
  void *CONCAT3(vector_2d, name, data)(void *this_ptr, uint64_t i);    \
  void CONCAT3(vector_2d, name, assign)(void *this_ptr, void *other_ptr)

#define VECTOR_2D_RAW_DECL(type) _VECTOR_2D_RAW_DECL(type)

#define _VECTOR_SHARED_DECL(name)                                                  \
  void *CONCAT3(vector_shared, name, new)(void);                                   \
  void CONCAT3(vector_shared, name, delete)(void *this_ptr);                       \
  void CONCAT3(vector_shared, name, clear)(void *this_ptr);                        \
  void CONCAT3(vector_shared, name, push_back)(void *this_ptr, void *val);         \
  void CONCAT4(vector_shared, name, push_back, shared)(void *this_ptr, void *val); \
  uint64_t CONCAT3(vector_shared, name, size)(void *this_ptr);                     \
  void *CONCAT3(vector_shared, name, data)(void *this_ptr, uint64_t i);            \
  void CONCAT3(vector_shared, name, assign)(void *this_ptr, void *other_ptr)

#define VECTOR_SHARED_NS_DECL(ns, type) _VECTOR_SHARED_DECL(CONCAT2(ns, type))
#define VECTOR_SHARED_DECL(type) _VECTOR_SHARED_DECL(type)

#define _VECTOR_WEAK_DECL(name)                                                  \
  void *CONCAT3(vector_weak, name, new)(void);                                   \
  void CONCAT3(vector_weak, name, delete)(void *this_ptr);                       \
  void CONCAT3(vector_weak, name, clear)(void *this_ptr);                        \
  void CONCAT3(vector_weak, name, push_back)(void *this_ptr, void *val);         \
  void CONCAT4(vector_weak, name, push_back, shared)(void *this_ptr, void *val); \
  uint64_t CONCAT3(vector_weak, name, size)(void *this_ptr);                     \
  void *CONCAT3(vector_weak, name, data)(void *this_ptr, uint64_t i);            \
  void CONCAT3(vector_weak, name, assign)(void *this_ptr, void *other_ptr)

#define VECTOR_WEAK_NS_DECL(ns, type) _VECTOR_WEAK_DECL(CONCAT2(ns, type))
#define VECTOR_WEAK_DECL(type) _VECTOR_WEAK_DECL(type)

#define _RAW_DATA_SHARED_NS_DECL(name, type_data, type_number)               \
  void *CONCAT5(name, type_data, type_number, shared, size)(void *this_ptr); \
  void *CONCAT5(name, type_data, type_number, shared, data)(void *this_ptr);

#define RAW_DATA_SHARED_NS_DECL(ns, type_data)                    \
  _RAW_DATA_SHARED_NS_DECL(CONCAT2(ns, RawData), type_data, real) \
  _RAW_DATA_SHARED_NS_DECL(CONCAT2(ns, RawData), type_data, complex)
#define RAW_DATA_SHARED_DECL(type_data)              \
  _RAW_DATA_SHARED_NS_DECL(RawData, type_data, real) \
  _RAW_DATA_SHARED_NS_DECL(RawData, type_data, complex)

#define _OBJECT_DECL(name)                                                   \
  void *CONCAT2(name, new)(void);                                            \
  void CONCAT2(name, delete)(void *this_ptr);                                \
  void CONCAT4(name, assign, raw, raw)(void *this_ptr, void *other_ptr);     \
  void CONCAT4(name, assign, raw, shared)(void *this_ptr, void *other_ptr);  \
  void CONCAT4(name, assign, weak, shared)(void *this_ptr, void *other_ptr); \
  void CONCAT4(name, assign, shared, shared)(void *this_ptr, void *other_ptr)

#define OBJECT_NS_DECL(ns, type) _OBJECT_DECL(CONCAT2(ns, type))
#define OBJECT_DECL(type) _OBJECT_DECL(type)

#define _OBJECT_RAW_DATA_DECL(name)           \
  void *CONCAT2(name, new)(uint64_t size);    \
  void CONCAT2(name, delete)(void *this_ptr); \
  void CONCAT4(name, assign, shared, shared)(void *this_ptr, void *other_ptr)

#define OBJECT_NS_RAW_DATA_DECL(ns, type, t1, t2) _OBJECT_RAW_DATA_DECL(CONCAT4(ns, type, t1, t2))
#define OBJECT_RAW_DATA_DECL(type, t2, t3) _OBJECT_RAW_DATA_DECL(CONCAT3(type, t1, t2))

#define _OBJECT_ACCESSOR_DECL(name, member)          \
  void *CONCAT2(name, member)(void *this_ptr);       \
  void *CONCAT3(name, weak, member)(void *this_ptr); \
  void *CONCAT3(name, shared, member)(void *this_ptr)

#define OBJECT_ACCESSOR_NS_DECL(ns, type, member) _OBJECT_ACCESSOR_DECL(CONCAT2(ns, type), member)
#define OBJECT_ACCESSOR_DECL(type, member) _OBJECT_ACCESSOR_DECL(type, member)

#ifdef __cplusplus
extern "C" {
#endif

uint64_t ptr2val(void *p);
void *val2ptr(uint64_t v);

void stdStringSet(void *this_ptr, const char *v);
const char *stdStringGet(void *this_ptr);

OBJECT_NS_DECL(urx, Acquisition);
OBJECT_ACCESSOR_NS_DECL(urx, Acquisition, authors);
OBJECT_ACCESSOR_NS_DECL(urx, Acquisition, description);
OBJECT_ACCESSOR_NS_DECL(urx, Acquisition, local_time);
OBJECT_ACCESSOR_NS_DECL(urx, Acquisition, country_code);
OBJECT_ACCESSOR_NS_DECL(urx, Acquisition, system);
OBJECT_ACCESSOR_NS_DECL(urx, Acquisition, sound_speed);
OBJECT_ACCESSOR_NS_DECL(urx, Acquisition, timestamp);
OBJECT_ACCESSOR_NS_DECL(urx, Acquisition, probes);
OBJECT_ACCESSOR_NS_DECL(urx, Acquisition, excitations);
OBJECT_ACCESSOR_NS_DECL(urx, Acquisition, groups);
OBJECT_ACCESSOR_NS_DECL(urx, Acquisition, groups_data);

OBJECT_NS_DECL(urx, Dataset);
OBJECT_ACCESSOR_NS_DECL(urx, Dataset, version);
OBJECT_ACCESSOR_NS_DECL(urx, Dataset, acquisition);

OBJECT_NS_DECL(urx, ElementGeometry);
OBJECT_ACCESSOR_NS_DECL(urx, ElementGeometry, perimeter);

OBJECT_NS_DECL(urx, Element);
OBJECT_ACCESSOR_NS_DECL(urx, Element, transform);
OBJECT_ACCESSOR_NS_DECL(urx, Element, element_geometry);
OBJECT_ACCESSOR_NS_DECL(urx, Element, impulse_response);

OBJECT_NS_DECL(urx, Event);
OBJECT_ACCESSOR_NS_DECL(urx, Event, transmit_setup);
OBJECT_ACCESSOR_NS_DECL(urx, Event, receive_setup);

OBJECT_NS_DECL(urx, Excitation);
OBJECT_ACCESSOR_NS_DECL(urx, Excitation, pulse_shape);
OBJECT_ACCESSOR_NS_DECL(urx, Excitation, transmit_frequency);
OBJECT_ACCESSOR_NS_DECL(urx, Excitation, sampling_frequency);
OBJECT_ACCESSOR_NS_DECL(urx, Excitation, waveform);

OBJECT_NS_DECL(urx, GroupData);
OBJECT_ACCESSOR_NS_DECL(urx, GroupData, group);
OBJECT_ACCESSOR_NS_DECL(urx, GroupData, raw_data);
OBJECT_ACCESSOR_NS_DECL(urx, GroupData, group_timestamp);
OBJECT_ACCESSOR_NS_DECL(urx, GroupData, sequence_timestamps);
OBJECT_ACCESSOR_NS_DECL(urx, GroupData, event_timestamps);

OBJECT_NS_DECL(urx, Group);
OBJECT_ACCESSOR_NS_DECL(urx, Group, sampling_type);
OBJECT_ACCESSOR_NS_DECL(urx, Group, data_type);
OBJECT_ACCESSOR_NS_DECL(urx, Group, description);
OBJECT_ACCESSOR_NS_DECL(urx, Group, sequence);

OBJECT_NS_DECL(urx, ImpulseResponse);
OBJECT_ACCESSOR_NS_DECL(urx, ImpulseResponse, sampling_frequency);
OBJECT_ACCESSOR_NS_DECL(urx, ImpulseResponse, time_offset);
OBJECT_ACCESSOR_NS_DECL(urx, ImpulseResponse, units);
OBJECT_ACCESSOR_NS_DECL(urx, ImpulseResponse, data);

OBJECT_NS_DECL(urx, Probe);
OBJECT_ACCESSOR_NS_DECL(urx, Probe, description);
OBJECT_ACCESSOR_NS_DECL(urx, Probe, type);
OBJECT_ACCESSOR_NS_DECL(urx, Probe, transform);
OBJECT_ACCESSOR_NS_DECL(urx, Probe, element_geometries);
OBJECT_ACCESSOR_NS_DECL(urx, Probe, impulse_responses);
OBJECT_ACCESSOR_NS_DECL(urx, Probe, elements);

OBJECT_NS_RAW_DATA_DECL(urx, RawData, int16_t, real);
OBJECT_NS_RAW_DATA_DECL(urx, RawData, int16_t, complex);
OBJECT_NS_RAW_DATA_DECL(urx, RawData, int32_t, real);
OBJECT_NS_RAW_DATA_DECL(urx, RawData, int32_t, complex);
OBJECT_NS_RAW_DATA_DECL(urx, RawData, float, real);
OBJECT_NS_RAW_DATA_DECL(urx, RawData, float, complex);
OBJECT_NS_RAW_DATA_DECL(urx, RawData, double, real);
OBJECT_NS_RAW_DATA_DECL(urx, RawData, double, complex);

OBJECT_NS_DECL(urx, ReceiveSetup);
OBJECT_ACCESSOR_NS_DECL(urx, ReceiveSetup, probe);
OBJECT_ACCESSOR_NS_DECL(urx, ReceiveSetup, probe_transform);
OBJECT_ACCESSOR_NS_DECL(urx, ReceiveSetup, sampling_frequency);
OBJECT_ACCESSOR_NS_DECL(urx, ReceiveSetup, number_samples);
OBJECT_ACCESSOR_NS_DECL(urx, ReceiveSetup, active_elements);
OBJECT_ACCESSOR_NS_DECL(urx, ReceiveSetup, tgc_profile);
OBJECT_ACCESSOR_NS_DECL(urx, ReceiveSetup, tgc_sampling_frequency);
OBJECT_ACCESSOR_NS_DECL(urx, ReceiveSetup, modulation_frequency);
OBJECT_ACCESSOR_NS_DECL(urx, ReceiveSetup, time_offset);

OBJECT_NS_DECL(urx, Transform);
OBJECT_ACCESSOR_NS_DECL(urx, Transform, rotation);
OBJECT_ACCESSOR_NS_DECL(urx, Transform, translation);

OBJECT_NS_DECL(urx, TransmitSetup);
OBJECT_ACCESSOR_NS_DECL(urx, TransmitSetup, probe);
OBJECT_ACCESSOR_NS_DECL(urx, TransmitSetup, wave);
OBJECT_ACCESSOR_NS_DECL(urx, TransmitSetup, active_elements);
OBJECT_ACCESSOR_NS_DECL(urx, TransmitSetup, excitations);
OBJECT_ACCESSOR_NS_DECL(urx, TransmitSetup, delays);
OBJECT_ACCESSOR_NS_DECL(urx, TransmitSetup, probe_transform);
OBJECT_ACCESSOR_NS_DECL(urx, TransmitSetup, time_offset);

OBJECT_NS_DECL(urx, Vector3D);
OBJECT_ACCESSOR_NS_DECL(urx, Vector3D, x);
OBJECT_ACCESSOR_NS_DECL(urx, Vector3D, y);
OBJECT_ACCESSOR_NS_DECL(urx, Vector3D, z);

OBJECT_NS_DECL(urx, Vector2D);
OBJECT_ACCESSOR_NS_DECL(urx, Vector2D, x);
OBJECT_ACCESSOR_NS_DECL(urx, Vector2D, y);

OBJECT_NS_DECL(urx, Version);
OBJECT_ACCESSOR_NS_DECL(urx, Version, major);
OBJECT_ACCESSOR_NS_DECL(urx, Version, minor);
OBJECT_ACCESSOR_NS_DECL(urx, Version, patch);

OBJECT_NS_DECL(urx, Wave);
OBJECT_ACCESSOR_NS_DECL(urx, Wave, type);
OBJECT_ACCESSOR_NS_DECL(urx, Wave, time_zero);
OBJECT_ACCESSOR_NS_DECL(urx, Wave, time_zero_reference_point);
OBJECT_ACCESSOR_NS_DECL(urx, Wave, parameters);

VECTOR_RAW_DECL(double);
VECTOR_RAW_DECL(uint32_t);

VECTOR_RAW_NS_DECL(urx, Element);
VECTOR_RAW_NS_DECL(urx, Event);
VECTOR_RAW_NS_DECL(urx, GroupData);
VECTOR_RAW_NS_DECL(urx, Vector3D);

VECTOR_2D_RAW_DECL(double);
VECTOR_2D_RAW_DECL(uint32_t);

VECTOR_SHARED_NS_DECL(urx, Probe);
VECTOR_SHARED_NS_DECL(urx, Excitation);
VECTOR_SHARED_NS_DECL(urx, Wave);
VECTOR_SHARED_NS_DECL(urx, Group);
VECTOR_SHARED_NS_DECL(urx, ElementGeometry);
VECTOR_SHARED_NS_DECL(urx, ImpulseResponse);
VECTOR_WEAK_NS_DECL(urx, Excitation);

RAW_DATA_SHARED_NS_DECL(urx, int16_t);
RAW_DATA_SHARED_NS_DECL(urx, int32_t);
RAW_DATA_SHARED_NS_DECL(urx, float);
RAW_DATA_SHARED_NS_DECL(urx, double);

uint64_t getPointer(void *ptr);

void *loadFromFile(const char *filename);
void saveToFile(const char *filename, void *dataset);

#ifdef __cplusplus
}
#endif

#endif  // #define URX_LIB_BINDING
