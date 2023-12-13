#ifndef UFFLIBBINDING_H_
#define UFFLIBBINDING_H_

#include <stdlib.h>
#include <stdint.h>

#define CONCAT(a, b) a##_##b
#define CONCAT3(a, b, c) a##_##b##_##c
#define STRINGIZE(x) #x

#define STD_VECTOR_DECL(type)                                           \
  int CONCAT3(std_vector, type, sizeof)(void);                          \
  void* CONCAT3(std_vector, type, new)(void);                           \
  void CONCAT3(std_vector, type, delete)(void *this_ptr);               \
  void CONCAT3(std_vector, type, clear)(void *this_ptr);                \
  void CONCAT3(std_vector, type, push_back)(void *this_ptr, type val);  \
  int CONCAT3(std_vector, type, size)(void *this_ptr);                  \
  void* CONCAT3(std_vector, type, data)(void *this_ptr);

#define STD_VECTOR_2D_DECL(type)                                        \
  void* CONCAT3(std_vector_2d, type, new)(void);                        \
  void CONCAT3(std_vector_2d, type, delete)(void *this_ptr);            \
  void CONCAT3(std_vector_2d, type, clear)(void *this_ptr);             \
  void CONCAT3(std_vector_2d, type, push_back)(void *this_ptr, void* val); \
  int CONCAT3(std_vector_2d, type, size)(void *this_ptr);               \
  void* CONCAT3(std_vector_2d, type, data)(void *this_ptr);

#define UFF_OBJECT_DECL(name)                                               \
  int CONCAT(name, sizeof)(void);                                       \
  void* CONCAT(name, new)(void);                                        \
  void CONCAT(name, delete)(void *this_ptr)

#define UFF_OBJECT_ACCESSOR_DECL(name, member)      \
  void* CONCAT(name, member)(void *this_ptr)

#define UFF_STD_VECTOR_DECL(name)                                           \
  void* CONCAT3(std_vector_uff, name, new)(void);                       \
  void CONCAT3(std_vector_uff, name, delete)(void* this_ptr);           \
  void* CONCAT3(std_vector_uff, name, data)(void *this_ptr);            \
  size_t CONCAT3(std_vector_uff, name, size)(void *this_ptr);           \
  void CONCAT3(std_vector_uff, name, clear)(void *this_ptr);            \
  void CONCAT3(std_vector_uff, name, push_back)(void *this_ptr, void *val); \
  void CONCAT3(std_vector_uff, name, copy)(void *this_ptr, void *other_ptr); \

#ifdef __cplusplus
extern "C" {
#endif

  uint64_t ptr2val(void *p);
  void* val2ptr(uint64_t v);

  void std_string_set(void *this_ptr, const char *v);
  const char * std_string_get(void *this_ptr);

  STD_VECTOR_DECL(double);
  STD_VECTOR_2D_DECL(double);

  UFF_OBJECT_DECL(Acquisition);
  UFF_OBJECT_ACCESSOR_DECL(Acquisition, authors);
  UFF_OBJECT_ACCESSOR_DECL(Acquisition, description);
  UFF_OBJECT_ACCESSOR_DECL(Acquisition, local_time);
  UFF_OBJECT_ACCESSOR_DECL(Acquisition, country_code);
  UFF_OBJECT_ACCESSOR_DECL(Acquisition, system);
  UFF_OBJECT_ACCESSOR_DECL(Acquisition, sound_speed);
  UFF_OBJECT_ACCESSOR_DECL(Acquisition, groups);
  UFF_OBJECT_ACCESSOR_DECL(Acquisition, unique_receive_setups);

  UFF_OBJECT_DECL(Group);
  UFF_OBJECT_ACCESSOR_DECL(Group, sampling_type);
  UFF_OBJECT_ACCESSOR_DECL(Group, sequence);
  UFF_OBJECT_ACCESSOR_DECL(Group, description);
  UFF_STD_VECTOR_DECL(Group);

  UFF_OBJECT_DECL(Event);
  UFF_OBJECT_ACCESSOR_DECL(Event, transmit_setup);
  UFF_OBJECT_ACCESSOR_DECL(Event, receive_setup);
  UFF_STD_VECTOR_DECL(Event);

  UFF_OBJECT_DECL(ReceiveSetup);
  UFF_OBJECT_ACCESSOR_DECL(ReceiveSetup, sampling_frequency);
  UFF_STD_VECTOR_DECL(ReceiveSetup);

  UFF_OBJECT_DECL(GroupData);
  UFF_OBJECT_ACCESSOR_DECL(GroupData, group_timestamp);
  UFF_OBJECT_ACCESSOR_DECL(GroupData, sequence_timestamps);
  UFF_OBJECT_ACCESSOR_DECL(GroupData, event_timestamps);
  UFF_OBJECT_ACCESSOR_DECL(GroupData, group);
#ifdef __cplusplus
}
#endif

#endif // UFFLIBBINDING_H_
