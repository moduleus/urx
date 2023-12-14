#include "UrxLibBinding.h"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <urx/acquisition.h>
#include <urx/event.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/receive_setup.h>

// NOLINTBEGIN(cppcoreguidelines-owning-memory,bugprone-macro-parentheses)

#define STD_VECTOR_IMPL(type)                                                          \
  size_t CONCAT3(std_vector, type, sizeof)(void) { return sizeof(std::vector<type>); } \
  void *CONCAT3(std_vector, type, new)(void) { return new std::vector<type>; }         \
  void CONCAT3(std_vector, type, delete)(void *this_ptr) {                             \
    delete static_cast<std::vector<type> *>(this_ptr);                                 \
  }                                                                                    \
  void CONCAT3(std_vector, type, clear)(void *this_ptr) {                              \
    static_cast<std::vector<type> *>(this_ptr)->clear();                               \
  }                                                                                    \
  void CONCAT3(std_vector, type, push_back)(void *this_ptr, type val) {                \
    static_cast<std::vector<type> *>(this_ptr)->push_back(val);                        \
  }                                                                                    \
  size_t CONCAT3(std_vector, type, size)(void *this_ptr) {                             \
    return static_cast<std::vector<type> *>(this_ptr)->size();                         \
  }                                                                                    \
  void *CONCAT3(std_vector, type, data)(void *this_ptr) {                              \
    return static_cast<std::vector<type> *>(this_ptr)->data();                         \
  }

#define STD_VECTOR_2D_IMPL(type)                                                               \
  void *CONCAT3(std_vector_2d, type, new)(void) { return new std::vector<std::vector<type>>; } \
  void CONCAT3(std_vector_2d, type, delete)(void *this_ptr) {                                  \
    delete static_cast<std::vector<std::vector<type>> *>(this_ptr);                            \
  }                                                                                            \
  void CONCAT3(std_vector_2d, type, clear)(void *this_ptr) {                                   \
    for (auto &it : *static_cast<std::vector<std::vector<type>> *>(this_ptr)) it.clear();      \
    static_cast<std::vector<std::vector<type>> *>(this_ptr)->clear();                          \
  }                                                                                            \
  void CONCAT3(std_vector_2d, type, push_back)(void *this_ptr, void *val) {                    \
    static_cast<std::vector<std::vector<type>> *>(this_ptr)->push_back(                        \
        *static_cast<std::vector<type> *>(val));                                               \
  }                                                                                            \
  size_t CONCAT3(std_vector_2d, type, size)(void *this_ptr) {                                  \
    return static_cast<std::vector<std::vector<type>> *>(this_ptr)->size();                    \
  }                                                                                            \
  void *CONCAT3(std_vector_2d, type, data)(void *this_ptr) {                                   \
    return static_cast<std::vector<std::vector<type>> *>(this_ptr)->data();                    \
  }

#define URX_OBJECT_IMPL(name)                                     \
  size_t CONCAT(name, sizeof)(void) { return sizeof(urx::name); } \
  void *CONCAT(name, new)(void) { return new urx::name; }         \
  void CONCAT(name, delete)(void *this_ptr) { delete static_cast<urx::name *>(this_ptr); }

#define URX_OBJECT_ACCESSOR_IMPL(name, member) \
  void *CONCAT(name, member)(void *this_ptr) { return &static_cast<urx::name *>(this_ptr)->member; }

#define URX_STD_VECTOR_IMPL(name)                                                               \
  void *CONCAT3(std_vector_urx, name, new)(void) { return new std::vector<urx::name>; }         \
  void CONCAT3(std_vector_urx, name, delete)(void *this_ptr) {                                  \
    delete static_cast<std::vector<urx::name> *>(this_ptr);                                     \
  }                                                                                             \
  void *CONCAT3(std_vector_urx, name, data)(void *this_ptr) {                                   \
    return static_cast<std::vector<urx::name> *>(this_ptr)->data();                             \
  }                                                                                             \
  size_t CONCAT3(std_vector_urx, name, size)(void *this_ptr) {                                  \
    return static_cast<std::vector<urx::name> *>(this_ptr)->size();                             \
  }                                                                                             \
  void CONCAT3(std_vector_urx, name, clear)(void *this_ptr) {                                   \
    static_cast<std::vector<urx::name> *>(this_ptr)->clear();                                   \
  }                                                                                             \
  void CONCAT3(std_vector_urx, name, push_back)(void *this_ptr, void *val) {                    \
    static_cast<std::vector<urx::name> *>(this_ptr)->push_back(*static_cast<urx::name *>(val)); \
  }                                                                                             \
  void CONCAT3(std_vector_urx, name, copy)(void *this_ptr, void *other_ptr) {                   \
    *static_cast<std::vector<urx::name> *>(this_ptr) =                                          \
        *static_cast<std::vector<urx::name> *>(other_ptr);                                      \
  }

uintptr_t ptr2val(void *p) { return reinterpret_cast<uintptr_t>(p); }
void *val2ptr(uintptr_t v) { return reinterpret_cast<void *>(v); }

void std_string_set(void *this_ptr, const char *v) { *static_cast<std::string *>(this_ptr) = v; }
const char *std_string_get(void *this_ptr) { return static_cast<std::string *>(this_ptr)->c_str(); }

STD_VECTOR_IMPL(double);
STD_VECTOR_2D_IMPL(double);

URX_OBJECT_IMPL(Acquisition);
URX_OBJECT_ACCESSOR_IMPL(Acquisition, authors);
URX_OBJECT_ACCESSOR_IMPL(Acquisition, description);
URX_OBJECT_ACCESSOR_IMPL(Acquisition, local_time);
URX_OBJECT_ACCESSOR_IMPL(Acquisition, country_code);
URX_OBJECT_ACCESSOR_IMPL(Acquisition, system);
URX_OBJECT_ACCESSOR_IMPL(Acquisition, sound_speed);
URX_OBJECT_ACCESSOR_IMPL(Acquisition, groups);

URX_OBJECT_IMPL(Group);
URX_OBJECT_ACCESSOR_IMPL(Group, sampling_type);
URX_OBJECT_ACCESSOR_IMPL(Group, sequence);
URX_OBJECT_ACCESSOR_IMPL(Group, description);
URX_STD_VECTOR_IMPL(Group);

URX_OBJECT_IMPL(Event);
URX_OBJECT_ACCESSOR_IMPL(Event, transmit_setup);
URX_OBJECT_ACCESSOR_IMPL(Event, receive_setup);
URX_STD_VECTOR_IMPL(Event);

URX_OBJECT_IMPL(ReceiveSetup);
URX_OBJECT_ACCESSOR_IMPL(ReceiveSetup, sampling_frequency);
URX_STD_VECTOR_IMPL(ReceiveSetup);

URX_OBJECT_IMPL(GroupData);
URX_OBJECT_ACCESSOR_IMPL(GroupData, group_timestamp);
URX_OBJECT_ACCESSOR_IMPL(GroupData, sequence_timestamps);
URX_OBJECT_ACCESSOR_IMPL(GroupData, event_timestamps);
URX_OBJECT_ACCESSOR_IMPL(GroupData, group);

// NOLINTEND(cppcoreguidelines-owning-memory,bugprone-macro-parentheses)
