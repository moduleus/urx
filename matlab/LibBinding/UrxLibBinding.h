#pragma once

#include <stddef.h>  // IWYU pragma: keep
#include <stdint.h>  // IWYU pragma: keep
// IWYU pragma: no_include <cstddef>

#define CONCAT(a, b) a##_##b
#define CONCAT3(a, b, c) a##_##b##_##c
#define STRINGIZE(x) #x

#define STD_VECTOR_DECL(type)                                          \
  size_t CONCAT3(std_vector, type, sizeof)(void);                      \
  void *CONCAT3(std_vector, type, new)(void);                          \
  void CONCAT3(std_vector, type, delete)(void *this_ptr);              \
  void CONCAT3(std_vector, type, clear)(void *this_ptr);               \
  void CONCAT3(std_vector, type, push_back)(void *this_ptr, type val); \
  size_t CONCAT3(std_vector, type, size)(void *this_ptr);              \
  void *CONCAT3(std_vector, type, data)(void *this_ptr);

#define STD_VECTOR_2D_DECL(type)                                           \
  void *CONCAT3(std_vector_2d, type, new)(void);                           \
  void CONCAT3(std_vector_2d, type, delete)(void *this_ptr);               \
  void CONCAT3(std_vector_2d, type, clear)(void *this_ptr);                \
  void CONCAT3(std_vector_2d, type, push_back)(void *this_ptr, void *val); \
  size_t CONCAT3(std_vector_2d, type, size)(void *this_ptr);               \
  void *CONCAT3(std_vector_2d, type, data)(void *this_ptr);

#define URX_OBJECT_DECL(name)        \
  size_t CONCAT(name, sizeof)(void); \
  void *CONCAT(name, new)(void);     \
  void CONCAT(name, delete)(void *this_ptr)

#define URX_OBJECT_ACCESSOR_DECL(name, member) void *CONCAT(name, member)(void *this_ptr)

#define URX_STD_VECTOR_DECL(name)                                           \
  void *CONCAT3(std_vector_urx, name, new)(void);                           \
  void CONCAT3(std_vector_urx, name, delete)(void *this_ptr);               \
  void *CONCAT3(std_vector_urx, name, data)(void *this_ptr);                \
  size_t CONCAT3(std_vector_urx, name, size)(void *this_ptr);               \
  void CONCAT3(std_vector_urx, name, clear)(void *this_ptr);                \
  void CONCAT3(std_vector_urx, name, push_back)(void *this_ptr, void *val); \
  void CONCAT3(std_vector_urx, name, copy)(void *this_ptr, void *other_ptr);

#ifdef __cplusplus
extern "C" {
#endif

uint64_t ptr2val(void *p);
void *val2ptr(uint64_t v);

void std_string_set(void *this_ptr, const char *v);
const char *std_string_get(void *this_ptr);

STD_VECTOR_DECL(double);
STD_VECTOR_2D_DECL(double);

URX_OBJECT_DECL(Acquisition);
URX_OBJECT_ACCESSOR_DECL(Acquisition, authors);
URX_OBJECT_ACCESSOR_DECL(Acquisition, description);
URX_OBJECT_ACCESSOR_DECL(Acquisition, local_time);
URX_OBJECT_ACCESSOR_DECL(Acquisition, country_code);
URX_OBJECT_ACCESSOR_DECL(Acquisition, system);
URX_OBJECT_ACCESSOR_DECL(Acquisition, sound_speed);
URX_OBJECT_ACCESSOR_DECL(Acquisition, groups);
//URX_OBJECT_ACCESSOR_DECL(Acquisition, unique_receive_setups);

URX_OBJECT_DECL(Group);
URX_OBJECT_ACCESSOR_DECL(Group, sampling_type);
URX_OBJECT_ACCESSOR_DECL(Group, sequence);
URX_OBJECT_ACCESSOR_DECL(Group, description);
URX_STD_VECTOR_DECL(Group);

URX_OBJECT_DECL(Event);
URX_OBJECT_ACCESSOR_DECL(Event, transmit_setup);
URX_OBJECT_ACCESSOR_DECL(Event, receive_setup);
URX_STD_VECTOR_DECL(Event);

URX_OBJECT_DECL(ReceiveSetup);
URX_OBJECT_ACCESSOR_DECL(ReceiveSetup, sampling_frequency);
URX_STD_VECTOR_DECL(ReceiveSetup);

URX_OBJECT_DECL(GroupData);
URX_OBJECT_ACCESSOR_DECL(GroupData, group_timestamp);
URX_OBJECT_ACCESSOR_DECL(GroupData, sequence_timestamps);
URX_OBJECT_ACCESSOR_DECL(GroupData, event_timestamps);
URX_OBJECT_ACCESSOR_DECL(GroupData, group);
#ifdef __cplusplus
}
#endif
