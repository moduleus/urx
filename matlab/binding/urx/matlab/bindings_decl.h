#ifndef URX_LIB_BINDING_DECL
#define URX_LIB_BINDING_DECL

#ifdef __cplusplus
#include <cstddef>
#else
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#endif

#include <urx/matlab/export.h>

#define FORCE_SEMICOLON static_assert(true)

// NOLINTBEGIN(readability-identifier-naming)

#define CONCAT2(a, b) a##_##b
#define CONCAT3(a, b, c) a##_##b##_##c
#define CONCAT4(a, b, c, d) a##_##b##_##c##_##d
#define CONCAT5(a, b, c, d, e) a##_##b##_##c##_##d##_##e
#define CONCAT_NS(a, b) a::b
#define CONCAT_NS3(a, b, c) a::b::c

#define _VECTOR_RAW_DECL(name)                                                                \
  URX_MATLAB_EXPORT void *CONCAT3(vector, name, new)(void);                                   \
  URX_MATLAB_EXPORT void CONCAT3(vector, name, delete)(void *this_ptr);                       \
  URX_MATLAB_EXPORT void CONCAT3(vector, name, clear)(void *this_ptr);                        \
  URX_MATLAB_EXPORT void CONCAT3(vector, name, push_back)(void *this_ptr, void *val);         \
  URX_MATLAB_EXPORT void CONCAT4(vector, name, push_back, shared)(void *this_ptr, void *val); \
  URX_MATLAB_EXPORT uint64_t CONCAT3(vector, name, size)(void *this_ptr);                     \
  URX_MATLAB_EXPORT void *CONCAT3(vector, name, data)(void *this_ptr, uint64_t i);            \
  URX_MATLAB_EXPORT void CONCAT3(vector, name, assign)(void *this_ptr, void *other_ptr)

#define VECTOR_RAW_NS_DECL(ns, type) _VECTOR_RAW_DECL(CONCAT2(ns, type))
#define VECTOR_RAW_DECL(type) _VECTOR_RAW_DECL(type)

#define _VECTOR_2D_RAW_DECL(name)                                                        \
  URX_MATLAB_EXPORT void *CONCAT3(vector_2d, name, new)(void);                           \
  URX_MATLAB_EXPORT void CONCAT3(vector_2d, name, delete)(void *this_ptr);               \
  URX_MATLAB_EXPORT void CONCAT3(vector_2d, name, clear)(void *this_ptr);                \
  URX_MATLAB_EXPORT void CONCAT3(vector_2d, name, push_back)(void *this_ptr, void *val); \
  URX_MATLAB_EXPORT uint64_t CONCAT3(vector_2d, name, size)(void *this_ptr);             \
  URX_MATLAB_EXPORT void *CONCAT3(vector_2d, name, data)(void *this_ptr, uint64_t i);    \
  URX_MATLAB_EXPORT void CONCAT3(vector_2d, name, assign)(void *this_ptr, void *other_ptr)

#define VECTOR_2D_RAW_NS_DECL(ns, type) _VECTOR_2D_RAW_DECL(CONCAT2(ns, type))
#define VECTOR_2D_RAW_DECL(type) _VECTOR_2D_RAW_DECL(type)

#define _VECTOR_SHARED_DECL(name)                                                            \
  URX_MATLAB_EXPORT void *CONCAT3(vector_shared, name, new)(void);                           \
  URX_MATLAB_EXPORT void CONCAT3(vector_shared, name, delete)(void *this_ptr);               \
  URX_MATLAB_EXPORT void CONCAT3(vector_shared, name, clear)(void *this_ptr);                \
  URX_MATLAB_EXPORT void CONCAT3(vector_shared, name, push_back)(void *this_ptr, void *val); \
  URX_MATLAB_EXPORT void CONCAT4(vector_shared, name, push_back, shared)(void *this_ptr,     \
                                                                         void *val);         \
  URX_MATLAB_EXPORT uint64_t CONCAT3(vector_shared, name, size)(void *this_ptr);             \
  URX_MATLAB_EXPORT void *CONCAT3(vector_shared, name, data)(void *this_ptr, uint64_t i);    \
  URX_MATLAB_EXPORT void CONCAT3(vector_shared, name, assign)(void *this_ptr, void *other_ptr)

#define VECTOR_SHARED_NS_DECL(ns, type) _VECTOR_SHARED_DECL(CONCAT2(ns, type))
#define VECTOR_SHARED_DECL(type) _VECTOR_SHARED_DECL(type)

#define _VECTOR_WEAK_DECL(name)                                                                    \
  URX_MATLAB_EXPORT void *CONCAT3(vector_weak, name, new)(void);                                   \
  URX_MATLAB_EXPORT void CONCAT3(vector_weak, name, delete)(void *this_ptr);                       \
  URX_MATLAB_EXPORT void CONCAT3(vector_weak, name, clear)(void *this_ptr);                        \
  URX_MATLAB_EXPORT void CONCAT3(vector_weak, name, push_back)(void *this_ptr, void *val);         \
  URX_MATLAB_EXPORT void CONCAT4(vector_weak, name, push_back, shared)(void *this_ptr, void *val); \
  URX_MATLAB_EXPORT uint64_t CONCAT3(vector_weak, name, size)(void *this_ptr);                     \
  URX_MATLAB_EXPORT void *CONCAT3(vector_weak, name, data)(void *this_ptr, uint64_t i);            \
  URX_MATLAB_EXPORT void CONCAT3(vector_weak, name, assign)(void *this_ptr, void *other_ptr)

#define VECTOR_WEAK_NS_DECL(ns, type) _VECTOR_WEAK_DECL(CONCAT2(ns, type))
#define VECTOR_WEAK_DECL(type) _VECTOR_WEAK_DECL(type)

#define _RAW_DATA_SHARED_DECL(name, type_data, type_number)                                    \
  URX_MATLAB_EXPORT void *CONCAT5(name, type_data, type_number, shared, size)(void *this_ptr); \
  URX_MATLAB_EXPORT void *CONCAT5(name, type_data, type_number, shared, data)(void *this_ptr); \
  URX_MATLAB_EXPORT uint8_t CONCAT5(name, type_data, type_number, shared,                      \
                                    sampling_type)(void *this_ptr);                            \
  URX_MATLAB_EXPORT uint8_t CONCAT5(name, type_data, type_number, shared, data_type)(void *this_ptr)

#define _RAW_DATA_SHARED_NOT_TYPED_DECL(name)                                     \
  URX_MATLAB_EXPORT void *CONCAT3(name, shared, size)(void *this_ptr);            \
  URX_MATLAB_EXPORT void *CONCAT3(name, shared, data)(void *this_ptr);            \
  URX_MATLAB_EXPORT uint8_t CONCAT3(name, shared, sampling_type)(void *this_ptr); \
  URX_MATLAB_EXPORT uint8_t CONCAT3(name, shared, data_type)(void *this_ptr)

#define _RAW_DATA_SHARED_DECL_TYPED(name, type_data) \
  _RAW_DATA_SHARED_DECL(name, int16_t, type_data);   \
  _RAW_DATA_SHARED_DECL(name, int32_t, type_data);   \
  _RAW_DATA_SHARED_DECL(name, float, type_data);     \
  _RAW_DATA_SHARED_DECL(name, double, type_data)

#define RAW_DATA_SHARED_NS_DECL(ns, name)                  \
  _RAW_DATA_SHARED_DECL_TYPED(CONCAT2(ns, name), real);    \
  _RAW_DATA_SHARED_DECL_TYPED(CONCAT2(ns, name), complex); \
  _RAW_DATA_SHARED_NOT_TYPED_DECL(CONCAT2(ns, name))
#define RAW_DATA_SHARED_DECL(name)            \
  _RAW_DATA_SHARED_DECL_TYPED(name, real);    \
  _RAW_DATA_SHARED_DECL_TYPED(name, complex); \
  _RAW_DATA_SHARED_NOT_TYPED_DECL(name)

#define _OBJECT_DECL(name)                                                                       \
  URX_MATLAB_EXPORT void *CONCAT2(name, new)(void);                                              \
  URX_MATLAB_EXPORT void CONCAT2(name, delete)(void *this_ptr);                                  \
  URX_MATLAB_EXPORT void CONCAT4(name, assign, raw, raw)(void *this_ptr, void *other_ptr);       \
  URX_MATLAB_EXPORT void CONCAT4(name, assign, raw, shared)(void *this_ptr, void *other_ptr);    \
  URX_MATLAB_EXPORT void CONCAT4(name, assign, weak, shared)(void *this_ptr, void *other_ptr);   \
  URX_MATLAB_EXPORT void CONCAT4(name, assign, shared, shared)(void *this_ptr, void *other_ptr); \
  URX_MATLAB_EXPORT void CONCAT4(name, assign, optional, shared)(void *this_ptr, void *other_ptr)

#define OBJECT_NS_DECL(ns, type) _OBJECT_DECL(CONCAT2(ns, type))
#define OBJECT_DECL(type) _OBJECT_DECL(type)

#define _OBJECT_RAW_DATA_DECL(name)                             \
  URX_MATLAB_EXPORT void *CONCAT2(name, new)(uint64_t size);    \
  URX_MATLAB_EXPORT void CONCAT2(name, delete)(void *this_ptr); \
  URX_MATLAB_EXPORT void CONCAT4(name, assign, shared, shared)(void *this_ptr, void *other_ptr)

#define _OBJECT_NS_RAW_DATA_DECL_TYPED(ns, name, type_data)  \
  _OBJECT_RAW_DATA_DECL(CONCAT4(ns, name, type_data, real)); \
  _OBJECT_RAW_DATA_DECL(CONCAT4(ns, name, type_data, complex))
#define _OBJECT_RAW_DATA_DECL_TYPED(name, type_data)     \
  _OBJECT_RAW_DATA_DECL(CONCAT3(name, type_data, real)); \
  _OBJECT_RAW_DATA_DECL(CONCAT3(name, type_data, complex))

#define OBJECT_NS_RAW_DATA_DECL(ns, name)            \
  _OBJECT_NS_RAW_DATA_DECL_TYPED(ns, name, int16_t); \
  _OBJECT_NS_RAW_DATA_DECL_TYPED(ns, name, int32_t); \
  _OBJECT_NS_RAW_DATA_DECL_TYPED(ns, name, float);   \
  _OBJECT_NS_RAW_DATA_DECL_TYPED(ns, name, double)
#define OBJECT_RAW_DATA_DECL(name)            \
  _OBJECT_RAW_DATA_DECL_TYPED(name, int16_t); \
  _OBJECT_RAW_DATA_DECL_TYPED(name, int32_t); \
  _OBJECT_RAW_DATA_DECL_TYPED(name, float);   \
  _OBJECT_RAW_DATA_DECL_TYPED(name, double)

#define _OBJECT_ACCESSOR_DECL(name, member)                                       \
  URX_MATLAB_EXPORT void *CONCAT3(name, raw, member)(void *this_ptr);             \
  URX_MATLAB_EXPORT void *CONCAT3(name, weak, member)(void *this_ptr);            \
  URX_MATLAB_EXPORT void *CONCAT3(name, shared, member)(void *this_ptr);          \
  URX_MATLAB_EXPORT void *CONCAT3(name, optional, member)(void *this_ptr);        \
  URX_MATLAB_EXPORT bool CONCAT4(name, raw, member, has_data)(void *this_ptr);    \
  URX_MATLAB_EXPORT bool CONCAT4(name, weak, member, has_data)(void *this_ptr);   \
  URX_MATLAB_EXPORT bool CONCAT4(name, shared, member, has_data)(void *this_ptr); \
  URX_MATLAB_EXPORT bool CONCAT4(name, optional, member, has_data)(void *this_ptr)

#define OBJECT_ACCESSOR_NS_DECL(ns, type, member) _OBJECT_ACCESSOR_DECL(CONCAT2(ns, type), member)
#define OBJECT_ACCESSOR_DECL(type, member) _OBJECT_ACCESSOR_DECL(type, member)

#define URX_MATLAB_ACQUISITION_DECL(ns)                   \
  OBJECT_NS_DECL(ns, Acquisition);                        \
  OBJECT_ACCESSOR_NS_DECL(ns, Acquisition, authors);      \
  OBJECT_ACCESSOR_NS_DECL(ns, Acquisition, description);  \
  OBJECT_ACCESSOR_NS_DECL(ns, Acquisition, local_time);   \
  OBJECT_ACCESSOR_NS_DECL(ns, Acquisition, country_code); \
  OBJECT_ACCESSOR_NS_DECL(ns, Acquisition, system);       \
  OBJECT_ACCESSOR_NS_DECL(ns, Acquisition, sound_speed);  \
  OBJECT_ACCESSOR_NS_DECL(ns, Acquisition, timestamp);    \
  OBJECT_ACCESSOR_NS_DECL(ns, Acquisition, probes);       \
  OBJECT_ACCESSOR_NS_DECL(ns, Acquisition, excitations);  \
  OBJECT_ACCESSOR_NS_DECL(ns, Acquisition, groups);       \
  OBJECT_ACCESSOR_NS_DECL(ns, Acquisition, groups_data)

#define URX_MATLAB_DATASET_DECL(ns)              \
  OBJECT_NS_DECL(ns, Dataset);                   \
  OBJECT_ACCESSOR_NS_DECL(ns, Dataset, version); \
  OBJECT_ACCESSOR_NS_DECL(ns, Dataset, acquisition)

#define URX_MATLAB_ELEMENT_GEOMETRY_DECL(ns) \
  OBJECT_NS_DECL(ns, ElementGeometry);       \
  OBJECT_ACCESSOR_NS_DECL(ns, ElementGeometry, perimeter)

#define URX_MATLAB_ELEMENT_DECL(ns)                       \
  OBJECT_NS_DECL(ns, Element);                            \
  OBJECT_ACCESSOR_NS_DECL(ns, Element, transform);        \
  OBJECT_ACCESSOR_NS_DECL(ns, Element, element_geometry); \
  OBJECT_ACCESSOR_NS_DECL(ns, Element, impulse_response)

#define URX_MATLAB_EVENT_DECL(ns)                     \
  OBJECT_NS_DECL(ns, Event);                          \
  OBJECT_ACCESSOR_NS_DECL(ns, Event, transmit_setup); \
  OBJECT_ACCESSOR_NS_DECL(ns, Event, receive_setup)

#define URX_MATLAB_EXCITATION_DECL(ns)                         \
  OBJECT_NS_DECL(ns, Excitation);                              \
  OBJECT_ACCESSOR_NS_DECL(ns, Excitation, pulse_shape);        \
  OBJECT_ACCESSOR_NS_DECL(ns, Excitation, transmit_frequency); \
  OBJECT_ACCESSOR_NS_DECL(ns, Excitation, sampling_frequency); \
  OBJECT_ACCESSOR_NS_DECL(ns, Excitation, waveform)

#define URX_MATLAB_GROUP_DATA_DECL(ns)                         \
  OBJECT_NS_DECL(ns, GroupData);                               \
  OBJECT_ACCESSOR_NS_DECL(ns, GroupData, group);               \
  OBJECT_ACCESSOR_NS_DECL(ns, GroupData, raw_data);            \
  OBJECT_ACCESSOR_NS_DECL(ns, GroupData, group_timestamp);     \
  OBJECT_ACCESSOR_NS_DECL(ns, GroupData, sequence_timestamps); \
  OBJECT_ACCESSOR_NS_DECL(ns, GroupData, event_timestamps)

#define URX_MATLAB_GROUP_DECL(ns)                    \
  OBJECT_NS_DECL(ns, Group);                         \
  OBJECT_ACCESSOR_NS_DECL(ns, Group, sampling_type); \
  OBJECT_ACCESSOR_NS_DECL(ns, Group, data_type);     \
  OBJECT_ACCESSOR_NS_DECL(ns, Group, description);   \
  OBJECT_ACCESSOR_NS_DECL(ns, Group, sequence)

#define URX_MATLAB_IMPULSE_RESPONSE_DECL(ns)                        \
  OBJECT_NS_DECL(ns, ImpulseResponse);                              \
  OBJECT_ACCESSOR_NS_DECL(ns, ImpulseResponse, sampling_frequency); \
  OBJECT_ACCESSOR_NS_DECL(ns, ImpulseResponse, time_offset);        \
  OBJECT_ACCESSOR_NS_DECL(ns, ImpulseResponse, units);              \
  OBJECT_ACCESSOR_NS_DECL(ns, ImpulseResponse, data)

#define URX_MATLAB_PROBE_DECL(ns)                         \
  OBJECT_NS_DECL(ns, Probe);                              \
  OBJECT_ACCESSOR_NS_DECL(ns, Probe, description);        \
  OBJECT_ACCESSOR_NS_DECL(ns, Probe, type);               \
  OBJECT_ACCESSOR_NS_DECL(ns, Probe, transform);          \
  OBJECT_ACCESSOR_NS_DECL(ns, Probe, element_geometries); \
  OBJECT_ACCESSOR_NS_DECL(ns, Probe, impulse_responses);  \
  OBJECT_ACCESSOR_NS_DECL(ns, Probe, elements)

#define URX_MATLAB_RECEIVE_SETUP_DECL(ns)                            \
  OBJECT_NS_DECL(ns, ReceiveSetup);                                  \
  OBJECT_ACCESSOR_NS_DECL(ns, ReceiveSetup, probe);                  \
  OBJECT_ACCESSOR_NS_DECL(ns, ReceiveSetup, probe_transform);        \
  OBJECT_ACCESSOR_NS_DECL(ns, ReceiveSetup, sampling_frequency);     \
  OBJECT_ACCESSOR_NS_DECL(ns, ReceiveSetup, number_samples);         \
  OBJECT_ACCESSOR_NS_DECL(ns, ReceiveSetup, active_elements);        \
  OBJECT_ACCESSOR_NS_DECL(ns, ReceiveSetup, tgc_profile);            \
  OBJECT_ACCESSOR_NS_DECL(ns, ReceiveSetup, tgc_sampling_frequency); \
  OBJECT_ACCESSOR_NS_DECL(ns, ReceiveSetup, modulation_frequency);   \
  OBJECT_ACCESSOR_NS_DECL(ns, ReceiveSetup, time_offset)

#define URX_MATLAB_TRANSFORM_DECL(ns)               \
  OBJECT_NS_DECL(ns, Transform);                    \
  OBJECT_ACCESSOR_NS_DECL(ns, Transform, rotation); \
  OBJECT_ACCESSOR_NS_DECL(ns, Transform, translation)

#define URX_MATLAB_TRANSMIT_SETUP_DECL(ns)                     \
  OBJECT_NS_DECL(ns, TransmitSetup);                           \
  OBJECT_ACCESSOR_NS_DECL(ns, TransmitSetup, probe);           \
  OBJECT_ACCESSOR_NS_DECL(ns, TransmitSetup, wave);            \
  OBJECT_ACCESSOR_NS_DECL(ns, TransmitSetup, active_elements); \
  OBJECT_ACCESSOR_NS_DECL(ns, TransmitSetup, excitations);     \
  OBJECT_ACCESSOR_NS_DECL(ns, TransmitSetup, delays);          \
  OBJECT_ACCESSOR_NS_DECL(ns, TransmitSetup, probe_transform); \
  OBJECT_ACCESSOR_NS_DECL(ns, TransmitSetup, time_offset)

#define URX_MATLAB_VECTOR3D_DECL(ns)        \
  OBJECT_NS_DECL(ns, Vector3D);             \
  OBJECT_ACCESSOR_NS_DECL(ns, Vector3D, x); \
  OBJECT_ACCESSOR_NS_DECL(ns, Vector3D, y); \
  OBJECT_ACCESSOR_NS_DECL(ns, Vector3D, z)

#define URX_MATLAB_VECTOR2D_DECL(ns)        \
  OBJECT_NS_DECL(ns, Vector2D);             \
  OBJECT_ACCESSOR_NS_DECL(ns, Vector2D, x); \
  OBJECT_ACCESSOR_NS_DECL(ns, Vector2D, y)

#define URX_MATLAB_VERSION_DECL(ns)            \
  OBJECT_NS_DECL(ns, Version);                 \
  OBJECT_ACCESSOR_NS_DECL(ns, Version, major); \
  OBJECT_ACCESSOR_NS_DECL(ns, Version, minor); \
  OBJECT_ACCESSOR_NS_DECL(ns, Version, patch)

#define URX_MATLAB_WAVE_DECL(ns)                                \
  OBJECT_NS_DECL(ns, Wave);                                     \
  OBJECT_ACCESSOR_NS_DECL(ns, Wave, type);                      \
  OBJECT_ACCESSOR_NS_DECL(ns, Wave, time_zero);                 \
  OBJECT_ACCESSOR_NS_DECL(ns, Wave, time_zero_reference_point); \
  OBJECT_ACCESSOR_NS_DECL(ns, Wave, parameters)

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

// NOLINTEND(readability-identifier-naming)

#endif  // #define URX_LIB_BINDING_DECL
