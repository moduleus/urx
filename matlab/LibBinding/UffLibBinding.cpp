#include "UffLibBinding.h"

#include <iostream>
#include <string>
#include <vector>

#include <uff/acquisition.h>


#define STD_VECTOR_IMPL(type)                                           \
  int CONCAT3(std_vector, type, sizeof)(void) { return sizeof (std::vector<type>); } \
  void* CONCAT3(std_vector, type, new)(void) { return new std::vector<type>; } \
  void CONCAT3(std_vector, type, delete)(void *this_ptr) { delete (std::vector<type>*)this_ptr; } \
  void CONCAT3(std_vector, type, clear)(void *this_ptr) { ((std::vector<type>*)this_ptr)->clear(); } \
  void CONCAT3(std_vector, type, push_back)(void *this_ptr, type val) { \
    ((std::vector<type>*)this_ptr)->push_back(val);                     \
  }                                                                     \
  int CONCAT3(std_vector, type, size)(void *this_ptr) {                 \
    return ((std::vector<type>*)this_ptr)->size();                      \
  }                                                                     \
  void* CONCAT3(std_vector, type, data)(void *this_ptr) {               \
    return ((std::vector<type>*)this_ptr)->data();                      \
  }

#define STD_VECTOR_2D_IMPL(type)                                        \
  void* CONCAT3(std_vector_2d, type, new)(void) { return new std::vector<std::vector<type>>; } \
  void CONCAT3(std_vector_2d, type, delete)(void *this_ptr) { delete (std::vector<std::vector<type>>*)this_ptr; } \
  void CONCAT3(std_vector_2d, type, clear)(void *this_ptr) {            \
    for (auto &it : *(std::vector<std::vector<type>>*)this_ptr)         \
      it.clear();                                                       \
    ((std::vector<type>*)this_ptr)->clear();                            \
  }                                                                     \
  void CONCAT3(std_vector_2d, type, push_back)(void *this_ptr, void* val) { \
    ((std::vector<std::vector<type>>*)this_ptr)->push_back(*(std::vector<type>*)val); \
  }                                                                     \
  int CONCAT3(std_vector_2d, type, size)(void *this_ptr) {              \
    return ((std::vector<std::vector<type>>*)this_ptr)->size();         \
  }                                                                     \
  void* CONCAT3(std_vector_2d, type, data)(void *this_ptr) {            \
    return ((std::vector<std::vector<type>>*)this_ptr)->data();         \
  }

#define UFF_OBJECT_IMPL(name)                                               \
  int CONCAT(name, sizeof)(void) { return sizeof (uff::name); }   \
  void* CONCAT(name, new)(void) {return new uff::name; }          \
  void CONCAT(name, delete)(void *this_ptr) { delete (uff::name*)this_ptr; }

#define UFF_OBJECT_ACCESSOR_IMPL(name, member)                              \
  void* CONCAT(name, member)(void *this_ptr) { return &((uff::name*)this_ptr)->member; }

#define UFF_STD_VECTOR_IMPL(name)                                           \
  void* CONCAT3(std_vector_uff, name, new)(void) { return new std::vector<uff::name>; } \
  void CONCAT3(std_vector_uff, name, delete)(void* this_ptr) {          \
    delete (std::vector<uff::name>*)this_ptr;                           \
  }                                                                     \
  void* CONCAT3(std_vector_uff, name, data)(void *this_ptr) {           \
    return ((std::vector<uff::name>*)this_ptr)->data();                 \
  }                                                                     \
  size_t CONCAT3(std_vector_uff, name, size)(void *this_ptr) {          \
    return ((std::vector<uff::name>*)this_ptr)->size();                 \
  }                                                                     \
  void CONCAT3(std_vector_uff, name, clear)(void *this_ptr) {           \
    ((std::vector<uff::name>*)this_ptr)->clear();                       \
  }                                                                     \
  void CONCAT3(std_vector_uff, name, push_back)(void *this_ptr, void *val) { \
    ((std::vector<uff::name>*)this_ptr)->push_back(*(uff::name*)val);   \
  }                                                                     \
  void CONCAT3(std_vector_uff, name, copy)(void *this_ptr, void *other_ptr) { \
    *(std::vector<uff::name>*)this_ptr = *(std::vector<uff::name>*)other_ptr; \
  }

uint64_t ptr2val(void *p) {return (uint64_t)p; }
void* val2ptr(uint64_t v) {return (void*)v; }

void std_string_set(void *this_ptr, const char *v) {*(std::string*)this_ptr = v;}
const char * std_string_get(void *this_ptr) { return ((std::string*)this_ptr)->c_str(); }

STD_VECTOR_IMPL(double);
STD_VECTOR_2D_IMPL(double);

UFF_OBJECT_IMPL(Acquisition);
UFF_OBJECT_ACCESSOR_IMPL(Acquisition, authors);
UFF_OBJECT_ACCESSOR_IMPL(Acquisition, description);
UFF_OBJECT_ACCESSOR_IMPL(Acquisition, local_time);
UFF_OBJECT_ACCESSOR_IMPL(Acquisition, country_code);
UFF_OBJECT_ACCESSOR_IMPL(Acquisition, system);
UFF_OBJECT_ACCESSOR_IMPL(Acquisition, sound_speed);
UFF_OBJECT_ACCESSOR_IMPL(Acquisition, groups);
UFF_OBJECT_ACCESSOR_IMPL(Acquisition, unique_receive_setups);

UFF_OBJECT_IMPL(Group);
UFF_OBJECT_ACCESSOR_IMPL(Group, sampling_type);
UFF_OBJECT_ACCESSOR_IMPL(Group, sequence);
UFF_OBJECT_ACCESSOR_IMPL(Group, description);
UFF_STD_VECTOR_IMPL(Group);

UFF_OBJECT_IMPL(Event);
UFF_OBJECT_ACCESSOR_IMPL(Event, transmit_setup);
UFF_OBJECT_ACCESSOR_IMPL(Event, receive_setup);
UFF_STD_VECTOR_IMPL(Event);

UFF_OBJECT_IMPL(ReceiveSetup);
UFF_OBJECT_ACCESSOR_IMPL(ReceiveSetup, sampling_frequency);
UFF_STD_VECTOR_IMPL(ReceiveSetup);

UFF_OBJECT_IMPL(GroupData);
UFF_OBJECT_ACCESSOR_IMPL(GroupData, group_timestamp);
UFF_OBJECT_ACCESSOR_IMPL(GroupData, sequence_timestamps);
UFF_OBJECT_ACCESSOR_IMPL(GroupData, event_timestamps);
UFF_OBJECT_ACCESSOR_IMPL(GroupData, group);
