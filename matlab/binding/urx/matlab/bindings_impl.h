#ifndef URX_MATLAB_BINDING_IMPL
#define URX_MATLAB_BINDING_IMPL

#include <algorithm>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include <urx/acquisition.h>
#include <urx/dataset.h>
#include <urx/detail/double_nan.h>
#include <urx/detail/raw_data.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/matlab/bindings_decl.h>
#include <urx/probe.h>
#include <urx/receive_setup.h>
#include <urx/transform.h>
#include <urx/transmit_setup.h>
#include <urx/utils/group_data_reader.h>
#include <urx/utils/io/reader_options.h>
#include <urx/utils/io/stream.h>
#include <urx/utils/io/writer_options.h>
#include <urx/vector.h>
#include <urx/version.h>
#include <urx/wave.h>

namespace urx {

template <typename T>
struct IsOptional : std::false_type {};

template <typename T>
struct IsOptional<std::optional<T>> : std::true_type {};

template <typename T>
struct IsWeakPtr : std::false_type {};

template <typename T>
struct IsWeakPtr<std::weak_ptr<T>> : std::true_type {};

template <typename T>
struct IsSharedPtr : std::false_type {};

template <typename T>
struct IsSharedPtr<std::shared_ptr<T>> : std::true_type {};

constexpr bool stringViewEqual(const char *a, const char *b) {
  return std::string_view(a) == std::string_view(b);
}

// NOLINTBEGIN(readability-identifier-naming)
static constexpr char raw_str[] = "raw";
static constexpr char shared_str[] = "shared";
// NOLINTEND(readability-identifier-naming)

template <typename T, const char *Type>
using PtrTypeT = std::conditional_t<stringViewEqual(Type, "shared"), std::shared_ptr<T> *, T *>;

template <typename T, const char *Type>
using RawTypeT = std::conditional_t<stringViewEqual(Type, "shared"), std::shared_ptr<T>, T *>;

template <typename T, const char *Type, typename U>
auto staticCastType(U *pointer) {
  if constexpr (std::is_pointer_v<RawTypeT<T, Type>>) {
    return static_cast<RawTypeT<T, Type>>(pointer);
  } else {
    return *static_cast<PtrTypeT<T, Type>>(pointer);
  }
}

}  // namespace urx

// NOLINTBEGIN(bugprone-macro-parentheses)

#define _VECTOR_RAW_IMPL(snake, type)                                                             \
  void *CONCAT3(vector, snake, new)() {                                                           \
    urxIncAllocCount();                                                                           \
    auto retval = new std::vector<type>();                                                        \
    urxGetLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n" << std::flush; \
    return retval;                                                                                \
  }                                                                                               \
  void CONCAT3(vector, snake, delete)(void *this_ptr) {                                           \
    urxDecAllocCount();                                                                           \
    urxGetLog() << reinterpret_cast<size_t>(this_ptr) << " " << __FUNCTION__ << "\n"              \
                << std::flush;                                                                    \
    delete static_cast<std::vector<type> *>(this_ptr);                                            \
  }                                                                                               \
  void CONCAT3(vector, snake, clear)(void *this_ptr) {                                            \
    static_cast<std::vector<type> *>(this_ptr)->clear();                                          \
  }                                                                                               \
  void CONCAT3(vector, snake, push_back)(void *this_ptr, void *val) {                             \
    static_cast<std::vector<type> *>(this_ptr)->push_back(*static_cast<type *>(val));             \
  }                                                                                               \
  void CONCAT4(vector, snake, push_back, shared)(void *this_ptr, void *val) {                     \
    static_cast<std::vector<type> *>(this_ptr)->push_back(                                        \
        *static_cast<std::shared_ptr<type> *>(val)->get());                                       \
  }                                                                                               \
  uint64_t CONCAT3(vector, snake, size)(void *this_ptr) {                                         \
    return static_cast<std::vector<type> *>(this_ptr)->size();                                    \
  }                                                                                               \
  void *CONCAT3(vector, snake, data)(void *this_ptr, uint64_t i) {                                \
    return &(*static_cast<std::vector<type> *>(this_ptr))[i];                                     \
  }                                                                                               \
  void CONCAT3(vector, snake, assign)(void *this_ptr, void *other_ptr) {                          \
    *static_cast<std::vector<type> *>(this_ptr) = *static_cast<std::vector<type> *>(other_ptr);   \
  }                                                                                               \
  bool CONCAT3(vector, snake, contains)(void *this_ptr, void *other_ptr) {                        \
    const auto &vec = *static_cast<std::vector<type> *>(this_ptr);                                \
    const type *other_ptr_casted = static_cast<type *>(other_ptr);                                \
    for (const auto &i : vec) {                                                                   \
      if (&i == other_ptr_casted) {                                                               \
        return true;                                                                              \
      }                                                                                           \
    }                                                                                             \
    return false;                                                                                 \
  }                                                                                               \
  FORCE_SEMICOLON

#define _VECTOR_RAW_IMPL_RAW(snake, type)                                 \
  void CONCAT4(vector, snake, push_back, raw)(void *this_ptr, type val) { \
    static_cast<std::vector<type> *>(this_ptr)->push_back(val);           \
  }                                                                       \
  FORCE_SEMICOLON

#define VECTOR_RAW_NS_IMPL(ns, name) _VECTOR_RAW_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define VECTOR_RAW_NS2_IMPL(ns, name_snake, name_real) \
  _VECTOR_RAW_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define VECTOR_RAW_IMPL(name) _VECTOR_RAW_IMPL(name, name)

#define VECTOR_RAW_NS_IMPL_RAW(ns, name)                    \
  _VECTOR_RAW_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name)); \
  _VECTOR_RAW_IMPL_RAW(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define VECTOR_RAW_IMPL_RAW(name) \
  _VECTOR_RAW_IMPL(name, name);   \
  _VECTOR_RAW_IMPL_RAW(name, name)

#define _VECTOR_2D_RAW_IMPL(snake, type)                                                          \
  void *CONCAT3(vector_2d, snake, new)() {                                                        \
    urxIncAllocCount();                                                                           \
    auto retval = new std::vector<std::vector<type>>();                                           \
    urxGetLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n" << std::flush; \
    return retval;                                                                                \
  }                                                                                               \
  void CONCAT3(vector_2d, snake, delete)(void *this_ptr) {                                        \
    urxDecAllocCount();                                                                           \
    urxGetLog() << reinterpret_cast<size_t>(this_ptr) << " " << __FUNCTION__ << "\n"              \
                << std::flush;                                                                    \
    delete static_cast<std::vector<std::vector<type>> *>(this_ptr);                               \
  }                                                                                               \
  void CONCAT3(vector_2d, snake, clear)(void *this_ptr) {                                         \
    static_cast<std::vector<std::vector<type>> *>(this_ptr)->clear();                             \
  }                                                                                               \
  void CONCAT3(vector_2d, snake, push_back)(void *this_ptr, void *val) {                          \
    static_cast<std::vector<std::vector<type>> *>(this_ptr)->push_back(                           \
        *static_cast<std::vector<type> *>(val));                                                  \
  }                                                                                               \
  uint64_t CONCAT3(vector_2d, snake, size)(void *this_ptr) {                                      \
    return static_cast<std::vector<std::vector<type>> *>(this_ptr)->size();                       \
  }                                                                                               \
  void *CONCAT3(vector_2d, snake, data)(void *this_ptr, uint64_t i) {                             \
    return &(*static_cast<std::vector<std::vector<type>> *>(this_ptr))[i];                        \
  }                                                                                               \
  void CONCAT3(vector_2d, snake, assign)(void *this_ptr, void *other_ptr) {                       \
    *static_cast<std::vector<std::vector<type>> *>(this_ptr) =                                    \
        *static_cast<std::vector<std::vector<type>> *>(other_ptr);                                \
  }                                                                                               \
  FORCE_SEMICOLON

#define VECTOR_2D_RAW_NS_IMPL(ns, name) _VECTOR_2D_RAW_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define VECTOR_2D_RAW_NS2_IMPL(ns, name_snake, name_real) \
  _VECTOR_2D_RAW_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define VECTOR_2D_RAW_IMPL(name) _VECTOR_2D_RAW_IMPL(name, name)

#define _VECTOR_SHARED_IMPL(snake, type)                                                          \
  void *CONCAT3(vector_shared, snake, new)() {                                                    \
    urxIncAllocCount();                                                                           \
    auto retval = new std::vector<std::shared_ptr<type>>();                                       \
    urxGetLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n" << std::flush; \
    return retval;                                                                                \
  }                                                                                               \
  void CONCAT3(vector_shared, snake, delete)(void *this_ptr) {                                    \
    urxDecAllocCount();                                                                           \
    urxGetLog() << reinterpret_cast<size_t>(this_ptr) << " " << __FUNCTION__ << "\n"              \
                << std::flush;                                                                    \
    delete static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr);                           \
  }                                                                                               \
  void CONCAT3(vector_shared, snake, clear)(void *this_ptr) {                                     \
    static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr)->clear();                         \
  }                                                                                               \
  void CONCAT3(vector_shared, snake, push_back)(void *this_ptr, void *val) {                      \
    static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr)->push_back(                       \
        *static_cast<std::shared_ptr<type> *>(val));                                              \
  }                                                                                               \
  void CONCAT4(vector_shared, snake, push_back, shared)(void *this_ptr, void *val) {              \
    static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr)->push_back(                       \
        *static_cast<std::shared_ptr<type> *>(val));                                              \
  }                                                                                               \
  uint64_t CONCAT3(vector_shared, snake, size)(void *this_ptr) {                                  \
    return static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr)->size();                   \
  }                                                                                               \
  void *CONCAT3(vector_shared, snake, data)(void *this_ptr, uint64_t i) {                         \
    return &(*static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr))[i];                    \
  }                                                                                               \
  void CONCAT3(vector_shared, snake, assign)(void *this_ptr, void *other_ptr) {                   \
    *static_cast<std::vector<std::shared_ptr<type>> *>(this_ptr) =                                \
        *static_cast<std::vector<std::shared_ptr<type>> *>(other_ptr);                            \
  }                                                                                               \
  FORCE_SEMICOLON

#define VECTOR_SHARED_NS_IMPL(ns, name) _VECTOR_SHARED_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define VECTOR_SHARED_NS2_IMPL(ns, name_snake, name_real) \
  _VECTOR_SHARED_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define VECTOR_SHARED_IMPL(name) _VECTOR_SHARED_IMPL(name, name)

#define _VECTOR_WEAK_IMPL(snake, type)                                                            \
  void *CONCAT3(vector_weak, snake, new)() {                                                      \
    urxIncAllocCount();                                                                           \
    auto retval = new std::vector<std::weak_ptr<type>>();                                         \
    urxGetLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n" << std::flush; \
    return retval;                                                                                \
  }                                                                                               \
  void CONCAT3(vector_weak, snake, delete)(void *this_ptr) {                                      \
    urxDecAllocCount();                                                                           \
    urxGetLog() << reinterpret_cast<size_t>(this_ptr) << " " << __FUNCTION__ << "\n"              \
                << std::flush;                                                                    \
    delete static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr);                             \
  }                                                                                               \
  void CONCAT3(vector_weak, snake, clear)(void *this_ptr) {                                       \
    static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr)->clear();                           \
  }                                                                                               \
  void CONCAT3(vector_weak, snake, push_back)(void *this_ptr, void *val) {                        \
    static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr)->push_back(                         \
        *static_cast<std::weak_ptr<type> *>(val));                                                \
  }                                                                                               \
  void CONCAT4(vector_weak, snake, push_back, shared)(void *this_ptr, void *val) {                \
    static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr)->push_back(                         \
        *static_cast<std::weak_ptr<type> *>(val));                                                \
  }                                                                                               \
  uint64_t CONCAT3(vector_weak, snake, size)(void *this_ptr) {                                    \
    return static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr)->size();                     \
  }                                                                                               \
  void *CONCAT3(vector_weak, snake, data)(void *this_ptr, uint64_t i) {                           \
    return &(*static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr))[i];                      \
  }                                                                                               \
  void CONCAT3(vector_weak, snake, assign)(void *this_ptr, void *other_ptr) {                     \
    *static_cast<std::vector<std::weak_ptr<type>> *>(this_ptr) =                                  \
        *static_cast<std::vector<std::weak_ptr<type>> *>(other_ptr);                              \
  }                                                                                               \
  FORCE_SEMICOLON

#define VECTOR_WEAK_NS_IMPL(ns, name) _VECTOR_WEAK_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define VECTOR_WEAK_NS2_IMPL(ns, name_snake, name_real) \
  _VECTOR_WEAK_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define VECTOR_WEAK_IMPL(name) _VECTOR_WEAK_IMPL(name, name)

#define _OBJECT_IMPL(snake, type)                                                                  \
  void *CONCAT3(snake, new, raw)(void) {                                                           \
    urxIncAllocCount();                                                                            \
    auto retval = new type();                                                                      \
    urxGetLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n" << std::flush;  \
    return retval;                                                                                 \
  }                                                                                                \
  void *CONCAT3(snake, new, shared)(void) {                                                        \
    urxIncAllocCount();                                                                            \
    auto retval = new std::shared_ptr<type>(new type());                                           \
    urxGetLog() << reinterpret_cast<size_t>(retval) << " "                                         \
                << reinterpret_cast<size_t>(retval->get()) << " " << __FUNCTION__ << "\n"          \
                << std::flush;                                                                     \
    return retval;                                                                                 \
  }                                                                                                \
  void CONCAT3(snake, delete, raw)(void *this_ptr) {                                               \
    urxDecAllocCount();                                                                            \
    urxGetLog() << reinterpret_cast<size_t>(this_ptr) << " " << __FUNCTION__ << "\n"               \
                << std::flush;                                                                     \
    delete static_cast<type *>(this_ptr);                                                          \
  }                                                                                                \
  void CONCAT3(snake, delete, shared)(void *this_ptr) {                                            \
    urxDecAllocCount();                                                                            \
    urxGetLog() << reinterpret_cast<size_t>(this_ptr) << " "                                       \
                << reinterpret_cast<size_t>(static_cast<std::shared_ptr<type> *>(this_ptr)->get()) \
                << " " << static_cast<std::shared_ptr<type> *>(this_ptr)->use_count() << " "       \
                << __FUNCTION__ << "\n"                                                            \
                << std::flush;                                                                     \
    delete static_cast<std::shared_ptr<type> *>(this_ptr);                                         \
  }                                                                                                \
  void CONCAT4(snake, assign, raw, raw)(void *this_ptr, void *other_ptr) {                         \
    *static_cast<type *>(this_ptr) = *static_cast<type *>(other_ptr);                              \
  }                                                                                                \
  void CONCAT4(snake, assign, raw, shared)(void *this_ptr, void *other_ptr) {                      \
    *static_cast<type *>(this_ptr) = **static_cast<std::shared_ptr<type> *>(other_ptr);            \
  }                                                                                                \
  void CONCAT4(snake, assign, weak, shared)(void *this_ptr, void *other_ptr) {                     \
    if (other_ptr) {                                                                               \
      *static_cast<std::weak_ptr<type> *>(this_ptr) =                                              \
          *static_cast<std::shared_ptr<type> *>(other_ptr);                                        \
    } else {                                                                                       \
      static_cast<std::weak_ptr<type> *>(this_ptr)->reset();                                       \
    }                                                                                              \
  }                                                                                                \
  void CONCAT4(snake, assign, shared, shared)(void *this_ptr, void *other_ptr) {                   \
    *static_cast<std::shared_ptr<type> *>(this_ptr) =                                              \
        *static_cast<std::shared_ptr<type> *>(other_ptr);                                          \
  }                                                                                                \
  void CONCAT4(snake, assign, optional, shared)(void *this_ptr, void *other_ptr) {                 \
    if (other_ptr) {                                                                               \
      *static_cast<std::optional<type> *>(this_ptr) =                                              \
          **static_cast<std::shared_ptr<type> *>(other_ptr);                                       \
    } else {                                                                                       \
      static_cast<std::optional<type> *>(this_ptr)->reset();                                       \
    }                                                                                              \
  }                                                                                                \
  uint64_t CONCAT3(snake, raw_ptr, raw)(void *this_ptr) {                                          \
    return reinterpret_cast<uint64_t>(this_ptr);                                                   \
  }                                                                                                \
  uint64_t CONCAT3(snake, raw_ptr, weak)(void *this_ptr) {                                         \
    auto ptr_lock = static_cast<std::weak_ptr<type> *>(this_ptr)->lock();                          \
    if (!ptr_lock) {                                                                               \
      return 0;                                                                                    \
    }                                                                                              \
    return reinterpret_cast<uint64_t>(ptr_lock.get());                                             \
  }                                                                                                \
  uint64_t CONCAT3(snake, raw_ptr, shared)(void *this_ptr) {                                       \
    return reinterpret_cast<uint64_t>(static_cast<std::shared_ptr<type> *>(this_ptr)->get());      \
  }                                                                                                \
  uint64_t CONCAT3(snake, raw_ptr, optional)(void *this_ptr) {                                     \
    auto *opt_ptr = static_cast<std::optional<type> *>(this_ptr);                                  \
    if (!*opt_ptr) {                                                                               \
      return 0;                                                                                    \
    }                                                                                              \
    return reinterpret_cast<uint64_t>(&opt_ptr->value());                                          \
  }                                                                                                \
  bool CONCAT4(snake, cmp, raw, raw)(void *this_ptr, void *other_ptr) {                            \
    return *static_cast<type *>(this_ptr) == *static_cast<type *>(other_ptr);                      \
  }                                                                                                \
  bool CONCAT4(snake, cmp, raw, weak)(void *this_ptr, void *other_ptr) {                           \
    auto ptr_lock = static_cast<std::weak_ptr<type> *>(other_ptr)->lock();                         \
    if (!ptr_lock) {                                                                               \
      return false;                                                                                \
    }                                                                                              \
    return *static_cast<type *>(this_ptr) == *ptr_lock;                                            \
  }                                                                                                \
  bool CONCAT4(snake, cmp, raw, shared)(void *this_ptr, void *other_ptr) {                         \
    return *static_cast<type *>(this_ptr) == **static_cast<std::shared_ptr<type> *>(other_ptr);    \
  }                                                                                                \
  bool CONCAT4(snake, cmp, raw, optional)(void *this_ptr, void *other_ptr) {                       \
    auto *opt_ptr = static_cast<std::optional<type> *>(other_ptr);                                 \
    if (!*opt_ptr) {                                                                               \
      return false;                                                                                \
    }                                                                                              \
    return *static_cast<type *>(this_ptr) == *opt_ptr;                                             \
  }                                                                                                \
  bool CONCAT4(snake, cmp, weak, raw)(void *this_ptr, void *other_ptr) {                           \
    auto ptr_lock = static_cast<std::weak_ptr<type> *>(this_ptr)->lock();                          \
    if (!ptr_lock) {                                                                               \
      return false;                                                                                \
    }                                                                                              \
    return *ptr_lock == *static_cast<type *>(other_ptr);                                           \
  }                                                                                                \
  bool CONCAT4(snake, cmp, weak, weak)(void *this_ptr, void *other_ptr) {                          \
    auto ptr_lock1 = static_cast<std::weak_ptr<type> *>(this_ptr)->lock();                         \
    if (!ptr_lock1) {                                                                              \
      return false;                                                                                \
    }                                                                                              \
    auto ptr_lock2 = static_cast<std::weak_ptr<type> *>(other_ptr)->lock();                        \
    if (!ptr_lock2) {                                                                              \
      return false;                                                                                \
    }                                                                                              \
    return *ptr_lock1 == *ptr_lock2;                                                               \
  }                                                                                                \
  bool CONCAT4(snake, cmp, weak, shared)(void *this_ptr, void *other_ptr) {                        \
    auto ptr_lock = static_cast<std::weak_ptr<type> *>(this_ptr)->lock();                          \
    if (!ptr_lock) {                                                                               \
      return false;                                                                                \
    }                                                                                              \
    return *ptr_lock == **static_cast<std::shared_ptr<type> *>(other_ptr);                         \
  }                                                                                                \
  bool CONCAT4(snake, cmp, weak, optional)(void *this_ptr, void *other_ptr) {                      \
    auto ptr_lock = static_cast<std::weak_ptr<type> *>(this_ptr)->lock();                          \
    if (!ptr_lock) {                                                                               \
      return false;                                                                                \
    }                                                                                              \
    auto *opt_ptr = static_cast<std::optional<type> *>(other_ptr);                                 \
    if (!*opt_ptr) {                                                                               \
      return false;                                                                                \
    }                                                                                              \
    return *ptr_lock == *opt_ptr;                                                                  \
  }                                                                                                \
  bool CONCAT4(snake, cmp, shared, raw)(void *this_ptr, void *other_ptr) {                         \
    return **static_cast<std::shared_ptr<type> *>(this_ptr) == *static_cast<type *>(other_ptr);    \
  }                                                                                                \
  bool CONCAT4(snake, cmp, shared, weak)(void *this_ptr, void *other_ptr) {                        \
    auto ptr_lock = static_cast<std::weak_ptr<type> *>(other_ptr)->lock();                         \
    if (!ptr_lock) {                                                                               \
      return false;                                                                                \
    }                                                                                              \
    return **static_cast<std::shared_ptr<type> *>(this_ptr) == *ptr_lock;                          \
  }                                                                                                \
  bool CONCAT4(snake, cmp, shared, shared)(void *this_ptr, void *other_ptr) {                      \
    return **static_cast<std::shared_ptr<type> *>(this_ptr) ==                                     \
           **static_cast<std::shared_ptr<type> *>(other_ptr);                                      \
  }                                                                                                \
  bool CONCAT4(snake, cmp, shared, optional)(void *this_ptr, void *other_ptr) {                    \
    auto *opt_ptr = static_cast<std::optional<type> *>(other_ptr);                                 \
    if (!*opt_ptr) {                                                                               \
      return false;                                                                                \
    }                                                                                              \
    return **static_cast<std::shared_ptr<type> *>(this_ptr) == *opt_ptr;                           \
  }                                                                                                \
  bool CONCAT4(snake, cmp, optional, raw)(void *this_ptr, void *other_ptr) {                       \
    auto *opt_ptr = static_cast<std::optional<type> *>(this_ptr);                                  \
    if (!*opt_ptr) {                                                                               \
      return false;                                                                                \
    }                                                                                              \
    return *opt_ptr == *static_cast<type *>(other_ptr);                                            \
  }                                                                                                \
  bool CONCAT4(snake, cmp, optional, weak)(void *this_ptr, void *other_ptr) {                      \
    auto *opt_ptr = static_cast<std::optional<type> *>(this_ptr);                                  \
    if (!*opt_ptr) {                                                                               \
      return false;                                                                                \
    }                                                                                              \
    auto ptr_lock = static_cast<std::weak_ptr<type> *>(other_ptr)->lock();                         \
    if (!ptr_lock) {                                                                               \
      return false;                                                                                \
    }                                                                                              \
    return *opt_ptr == *ptr_lock;                                                                  \
  }                                                                                                \
  bool CONCAT4(snake, cmp, optional, shared)(void *this_ptr, void *other_ptr) {                    \
    auto *opt_ptr = static_cast<std::optional<type> *>(this_ptr);                                  \
    if (!*opt_ptr) {                                                                               \
      return false;                                                                                \
    }                                                                                              \
    return *opt_ptr == **static_cast<std::shared_ptr<type> *>(other_ptr);                          \
  }                                                                                                \
  bool CONCAT4(snake, cmp, optional, optional)(void *this_ptr, void *other_ptr) {                  \
    auto *opt_ptr1 = static_cast<std::optional<type> *>(this_ptr);                                 \
    if (!*opt_ptr1) {                                                                              \
      return false;                                                                                \
    }                                                                                              \
    auto *opt_ptr2 = static_cast<std::optional<type> *>(other_ptr);                                \
    if (!*opt_ptr2) {                                                                              \
      return false;                                                                                \
    }                                                                                              \
    return *opt_ptr1 == *opt_ptr2;                                                                 \
  }                                                                                                \
  FORCE_SEMICOLON

// All method are virtual, it also could be
// (*static_cast<std::shared_ptr<urx::RawData> *>(this_ptr))->
#define _RAW_DATA_SHARED_IMPL_real_shared_size(name, type_data)                                \
  void *CONCAT5(name, type_data, real, shared, size)(void *this_ptr) {                         \
    static uint64_t retval;                                                                    \
    retval =                                                                                   \
        (*static_cast<std::shared_ptr<urx::RawDataNoInit<type_data>> *>(this_ptr))->getSize(); \
    return &retval;                                                                            \
  }                                                                                            \
  FORCE_SEMICOLON
#define _RAW_DATA_SHARED_IMPL_complex_shared_size(name, type_data)                               \
  void *CONCAT5(name, type_data, complex, shared, size)(void *this_ptr) {                        \
    static uint64_t retval;                                                                      \
    retval =                                                                                     \
        (*static_cast<std::shared_ptr<urx::RawDataNoInit<std::complex<type_data>>> *>(this_ptr)) \
            ->getSize();                                                                         \
    return &retval;                                                                              \
  }                                                                                              \
  FORCE_SEMICOLON
#define _RAW_DATA_SHARED_IMPL_real_shared_data(name, type_data)                       \
  void *CONCAT5(name, type_data, real, shared, data)(void *this_ptr) {                \
    return (*static_cast<std::shared_ptr<urx::RawDataNoInit<type_data>> *>(this_ptr)) \
        ->getBuffer();                                                                \
  }                                                                                   \
  FORCE_SEMICOLON
#define _RAW_DATA_SHARED_IMPL_complex_shared_data(name, type_data)                        \
  void *CONCAT5(name, type_data, complex, shared, data)(void *this_ptr) {                 \
    return (*static_cast<std::shared_ptr<urx::RawDataNoInit<std::complex<type_data>>> *>( \
                this_ptr))                                                                \
        ->getBuffer();                                                                    \
  }                                                                                       \
  FORCE_SEMICOLON
#define _RAW_DATA_SHARED_IMPL_real_shared_sampling_type(name, type_data)           \
  uint8_t CONCAT5(name, type_data, real, shared, sampling_type)(void *this_ptr) {  \
    return static_cast<uint8_t>(                                                   \
        (*static_cast<std::shared_ptr<urx::RawDataNoInit<type_data>> *>(this_ptr)) \
            ->getSamplingType());                                                  \
  }                                                                                \
  FORCE_SEMICOLON
#define _RAW_DATA_SHARED_IMPL_complex_shared_sampling_type(name, type_data)                      \
  uint8_t CONCAT5(name, type_data, complex, shared, sampling_type)(void *this_ptr) {             \
    return static_cast<uint8_t>(                                                                 \
        (*static_cast<std::shared_ptr<urx::RawDataNoInit<std::complex<type_data>>> *>(this_ptr)) \
            ->getSamplingType());                                                                \
  }                                                                                              \
  FORCE_SEMICOLON
#define _RAW_DATA_SHARED_IMPL_real_shared_data_type(name, type_data)               \
  uint8_t CONCAT5(name, type_data, real, shared, data_type)(void *this_ptr) {      \
    return static_cast<uint8_t>(                                                   \
        (*static_cast<std::shared_ptr<urx::RawDataNoInit<type_data>> *>(this_ptr)) \
            ->getDataType());                                                      \
  }                                                                                \
  FORCE_SEMICOLON
#define _RAW_DATA_SHARED_IMPL_complex_shared_data_type(name, type_data)                          \
  uint8_t CONCAT5(name, type_data, complex, shared, data_type)(void *this_ptr) {                 \
    return static_cast<uint8_t>(                                                                 \
        (*static_cast<std::shared_ptr<urx::RawDataNoInit<std::complex<type_data>>> *>(this_ptr)) \
            ->getDataType());                                                                    \
  }                                                                                              \
  FORCE_SEMICOLON

#define _RAW_DATA_SHARED_IMPL_NOT_TYPED(name)                                          \
  void *CONCAT3(name, shared, size)(void *this_ptr) {                                  \
    static uint64_t retval;                                                            \
    retval = (*static_cast<std::shared_ptr<urx::RawData> *>(this_ptr))->getSize();     \
    return &retval;                                                                    \
  }                                                                                    \
  void *CONCAT3(name, shared, data)(void *this_ptr) {                                  \
    return (*static_cast<std::shared_ptr<urx::RawData> *>(this_ptr))->getBuffer();     \
  }                                                                                    \
  uint8_t CONCAT3(name, shared, sampling_type)(void *this_ptr) {                       \
    return static_cast<uint8_t>(                                                       \
        (*static_cast<std::shared_ptr<urx::RawData> *>(this_ptr))->getSamplingType()); \
  }                                                                                    \
  uint8_t CONCAT3(name, shared, data_type)(void *this_ptr) {                           \
    return static_cast<uint8_t>(                                                       \
        (*static_cast<std::shared_ptr<urx::RawData> *>(this_ptr))->getDataType());     \
  }                                                                                    \
  bool CONCAT4(name, cmp, shared, raw)(void *this_ptr, void *other_ptr) {              \
    return **static_cast<std::shared_ptr<urx::RawData> *>(this_ptr) ==                 \
           *static_cast<urx::RawData *>(other_ptr);                                    \
  }                                                                                    \
  bool CONCAT4(name, cmp, shared, shared)(void *this_ptr, void *other_ptr) {           \
    return **static_cast<std::shared_ptr<urx::RawData> *>(this_ptr) ==                 \
           **static_cast<std::shared_ptr<urx::RawData> *>(other_ptr);                  \
  }                                                                                    \
  FORCE_SEMICOLON

#define _RAW_DATA_SHARED_IMPL(name, type_data, type_number)                    \
  _RAW_DATA_SHARED_IMPL_##type_number##_shared_size(name, type_data);          \
  _RAW_DATA_SHARED_IMPL_##type_number##_shared_data(name, type_data);          \
  _RAW_DATA_SHARED_IMPL_##type_number##_shared_sampling_type(name, type_data); \
  _RAW_DATA_SHARED_IMPL_##type_number##_shared_data_type(name, type_data)

#define _RAW_DATA_SHARED_NS_IMPL_TYPED(ns, name, type_data)  \
  _RAW_DATA_SHARED_IMPL(CONCAT2(ns, name), type_data, real); \
  _RAW_DATA_SHARED_IMPL(CONCAT2(ns, name), type_data, complex)
#define _RAW_DATA_SHARED_IMPL_TYPED(name, type_data) \
  _RAW_DATA_SHARED_IMPL(name, type_data, real);      \
  _RAW_DATA_SHARED_IMPL(name, type_data, complex)

#define RAW_DATA_SHARED_NS_IMPL(ns, name)            \
  _RAW_DATA_SHARED_NS_IMPL_TYPED(ns, name, int16_t); \
  _RAW_DATA_SHARED_NS_IMPL_TYPED(ns, name, int32_t); \
  _RAW_DATA_SHARED_NS_IMPL_TYPED(ns, name, float);   \
  _RAW_DATA_SHARED_NS_IMPL_TYPED(ns, name, double);  \
  _RAW_DATA_SHARED_IMPL_NOT_TYPED(CONCAT2(ns, name))
#define RAW_DATA_SHARED_IMPL(name)            \
  _RAW_DATA_SHARED_IMPL_TYPED(name, int16_t); \
  _RAW_DATA_SHARED_IMPL_TYPED(name, int32_t); \
  _RAW_DATA_SHARED_IMPL_TYPED(name, float);   \
  _RAW_DATA_SHARED_IMPL_TYPED(name, double);  \
  _RAW_DATA_SHARED_IMPL_NOT_TYPED(name)

#define OBJECT_NS_IMPL(ns, name) _OBJECT_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name))
#define OBJECT_NS2_IMPL(ns, name_snake, name_real) \
  _OBJECT_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real))
#define OBJECT_IMPL(name) _OBJECT_IMPL(name, name)

#define _OBJECT_RAW_DATA_IMPL(snake, type, other_type)                                             \
  void *CONCAT3(snake, new, raw)(uint64_t size) {                                                  \
    urxIncAllocCount();                                                                            \
    auto retval = new other_type(size);                                                            \
    urxGetLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n" << std::flush;  \
    return retval;                                                                                 \
  }                                                                                                \
  void *CONCAT3(snake, new, shared)(uint64_t size) {                                               \
    urxIncAllocCount();                                                                            \
    auto retval = new std::shared_ptr<other_type>(new other_type(size));                           \
    urxGetLog() << reinterpret_cast<size_t>(retval) << " "                                         \
                << reinterpret_cast<size_t>(retval->get()) << " " << __FUNCTION__ << "\n"          \
                << std::flush;                                                                     \
    return retval;                                                                                 \
  }                                                                                                \
  void CONCAT3(snake, delete, raw)(void *this_ptr) {                                               \
    urxDecAllocCount();                                                                            \
    urxGetLog() << reinterpret_cast<size_t>(this_ptr) << " " << __FUNCTION__ << "\n"               \
                << std::flush;                                                                     \
    delete static_cast<other_type *>(this_ptr);                                                    \
  }                                                                                                \
  void CONCAT3(snake, delete, shared)(void *this_ptr) {                                            \
    urxDecAllocCount();                                                                            \
    urxGetLog() << reinterpret_cast<size_t>(this_ptr) << " "                                       \
                << reinterpret_cast<size_t>(                                                       \
                       static_cast<std::shared_ptr<other_type> *>(this_ptr)->get())                \
                << " " << static_cast<std::shared_ptr<other_type> *>(this_ptr)->use_count() << " " \
                << __FUNCTION__ << "\n"                                                            \
                << std::flush;                                                                     \
    delete static_cast<std::shared_ptr<other_type> *>(this_ptr);                                   \
  }                                                                                                \
  void CONCAT4(snake, assign, shared, shared)(void *this_ptr, void *other_ptr) {                   \
    *static_cast<std::shared_ptr<type> *>(this_ptr) =                                              \
        *static_cast<std::shared_ptr<other_type> *>(other_ptr);                                    \
  }                                                                                                \
  FORCE_SEMICOLON

namespace urx::matlab::detail {

template <typename T>
bool checkHasValue(const T &argument) {
  if constexpr (urx::IsOptional<T>::value) {
    return argument.has_value();
  } else if constexpr (urx::IsWeakPtr<T>::value) {
    return argument.owner_before(T{}) || T{}.owner_before(argument);
  } else if constexpr (urx::IsSharedPtr<T>::value) {
    return !!argument;
  } else {
    return true;
  }
}

}  // namespace urx::matlab::detail

#define OBJECT_NS_RAW_DATA_IMPL(ns, name, t1, t2, other_name) \
  _OBJECT_RAW_DATA_IMPL(CONCAT4(ns, name, t1, t2), CONCAT_NS(ns, name), other_name)

#define _OBJECT_ACCESSOR_IMPL(snake, type, member)                                    \
  void *CONCAT3(snake, raw, member)(void *this_ptr) {                                 \
    return &static_cast<type *>(this_ptr)->member;                                    \
  }                                                                                   \
  void *CONCAT3(snake, weak, member)(void *this_ptr) {                                \
    return &static_cast<std::weak_ptr<type> *>(this_ptr)->lock()->member;             \
  }                                                                                   \
  void *CONCAT3(snake, shared, member)(void *this_ptr) {                              \
    return &static_cast<std::shared_ptr<type> *>(this_ptr)->get()->member;            \
  }                                                                                   \
  void *CONCAT3(snake, optional, member)(void *this_ptr) {                            \
    auto *opt_ptr = static_cast<std::optional<type> *>(this_ptr);                     \
    if (!*opt_ptr) {                                                                  \
      return nullptr;                                                                 \
    }                                                                                 \
    return &(*opt_ptr)->member;                                                       \
  }                                                                                   \
  bool CONCAT4(snake, raw, member, has_data)(void *this_ptr) {                        \
    return urx::matlab::detail::checkHasValue(static_cast<type *>(this_ptr)->member); \
  }                                                                                   \
  bool CONCAT4(snake, weak, member, has_data)(void *this_ptr) {                       \
    auto lock_ptr = static_cast<std::weak_ptr<type> *>(this_ptr)->lock();             \
    if (!lock_ptr) {                                                                  \
      return false;                                                                   \
    }                                                                                 \
    return urx::matlab::detail::checkHasValue(lock_ptr->member);                      \
  }                                                                                   \
  bool CONCAT4(snake, shared, member, has_data)(void *this_ptr) {                     \
    return urx::matlab::detail::checkHasValue(                                        \
        static_cast<std::shared_ptr<type> *>(this_ptr)->get()->member);               \
  }                                                                                   \
  bool CONCAT4(snake, optional, member, has_data)(void *this_ptr) {                   \
    auto *opt_ptr = static_cast<std::optional<type> *>(this_ptr);                     \
    if (!*opt_ptr) {                                                                  \
      return false;                                                                   \
    }                                                                                 \
    return urx::matlab::detail::checkHasValue((*opt_ptr)->member);                    \
  }                                                                                   \
  FORCE_SEMICOLON

#define OBJECT_ACCESSOR_NS_IMPL(ns, name, member) \
  _OBJECT_ACCESSOR_IMPL(CONCAT2(ns, name), CONCAT_NS(ns, name), member)
#define OBJECT_ACCESSOR_NS2_IMPL(ns, name_snake, name_real, member) \
  _OBJECT_ACCESSOR_IMPL(CONCAT2(ns, name_snake), CONCAT_NS(ns, name_real), member)
#define OBJECT_ACCESSOR_IMPL(name, member) _OBJECT_ACCESSOR_IMPL(name, name, member)

#define URX_MATLAB_ACQUISITION_IMPL(ns)                   \
  OBJECT_NS_IMPL(ns, Acquisition);                        \
  OBJECT_ACCESSOR_NS_IMPL(ns, Acquisition, authors);      \
  OBJECT_ACCESSOR_NS_IMPL(ns, Acquisition, description);  \
  OBJECT_ACCESSOR_NS_IMPL(ns, Acquisition, local_time);   \
  OBJECT_ACCESSOR_NS_IMPL(ns, Acquisition, country_code); \
  OBJECT_ACCESSOR_NS_IMPL(ns, Acquisition, system);       \
  OBJECT_ACCESSOR_NS_IMPL(ns, Acquisition, timestamp);    \
  OBJECT_ACCESSOR_NS_IMPL(ns, Acquisition, probes);       \
  OBJECT_ACCESSOR_NS_IMPL(ns, Acquisition, excitations);  \
  OBJECT_ACCESSOR_NS_IMPL(ns, Acquisition, groups)

#define URX_MATLAB_DATASET_IMPL(ns)              \
  OBJECT_NS_IMPL(ns, Dataset);                   \
  OBJECT_ACCESSOR_NS_IMPL(ns, Dataset, version); \
  OBJECT_ACCESSOR_NS_IMPL(ns, Dataset, acquisition)

#define URX_MATLAB_ELEMENT_GEOMETRY_IMPL(ns) \
  OBJECT_NS_IMPL(ns, ElementGeometry);       \
  OBJECT_ACCESSOR_NS_IMPL(ns, ElementGeometry, perimeter)

#define URX_MATLAB_ELEMENT_IMPL(ns)                       \
  OBJECT_NS_IMPL(ns, Element);                            \
  OBJECT_ACCESSOR_NS_IMPL(ns, Element, transform);        \
  OBJECT_ACCESSOR_NS_IMPL(ns, Element, element_geometry); \
  OBJECT_ACCESSOR_NS_IMPL(ns, Element, impulse_response)

#define URX_MATLAB_EVENT_IMPL(ns)                     \
  OBJECT_NS_IMPL(ns, Event);                          \
  OBJECT_ACCESSOR_NS_IMPL(ns, Event, transmit_setup); \
  OBJECT_ACCESSOR_NS_IMPL(ns, Event, receive_setup)

#define URX_MATLAB_EXCITATION_IMPL(ns)                         \
  OBJECT_NS_IMPL(ns, Excitation);                              \
  OBJECT_ACCESSOR_NS_IMPL(ns, Excitation, pulse_shape);        \
  OBJECT_ACCESSOR_NS_IMPL(ns, Excitation, transmit_frequency); \
  OBJECT_ACCESSOR_NS_IMPL(ns, Excitation, sampling_frequency); \
  OBJECT_ACCESSOR_NS_IMPL(ns, Excitation, waveform)

#define URX_MATLAB_GROUP_DATA_IMPL(ns)                         \
  OBJECT_NS_IMPL(ns, GroupData);                               \
  OBJECT_ACCESSOR_NS_IMPL(ns, GroupData, group);               \
  OBJECT_ACCESSOR_NS_IMPL(ns, GroupData, raw_data);            \
  OBJECT_ACCESSOR_NS_IMPL(ns, GroupData, group_timestamp);     \
  OBJECT_ACCESSOR_NS_IMPL(ns, GroupData, sequence_timestamps); \
  OBJECT_ACCESSOR_NS_IMPL(ns, GroupData, event_timestamps)

#define URX_MATLAB_GROUP_IMPL(ns)                    \
  OBJECT_NS_IMPL(ns, Group);                         \
  OBJECT_ACCESSOR_NS_IMPL(ns, Group, sampling_type); \
  OBJECT_ACCESSOR_NS_IMPL(ns, Group, data_type);     \
  OBJECT_ACCESSOR_NS_IMPL(ns, Group, description);   \
  OBJECT_ACCESSOR_NS_IMPL(ns, Group, sound_speed);   \
  OBJECT_ACCESSOR_NS_IMPL(ns, Group, sequence)

#define URX_MATLAB_IMPULSE_RESPONSE_IMPL(ns)                        \
  OBJECT_NS_IMPL(ns, ImpulseResponse);                              \
  OBJECT_ACCESSOR_NS_IMPL(ns, ImpulseResponse, sampling_frequency); \
  OBJECT_ACCESSOR_NS_IMPL(ns, ImpulseResponse, time_offset);        \
  OBJECT_ACCESSOR_NS_IMPL(ns, ImpulseResponse, units);              \
  OBJECT_ACCESSOR_NS_IMPL(ns, ImpulseResponse, data)

#define URX_MATLAB_PROBE_IMPL(ns)                         \
  OBJECT_NS_IMPL(ns, Probe);                              \
  OBJECT_ACCESSOR_NS_IMPL(ns, Probe, description);        \
  OBJECT_ACCESSOR_NS_IMPL(ns, Probe, type);               \
  OBJECT_ACCESSOR_NS_IMPL(ns, Probe, transform);          \
  OBJECT_ACCESSOR_NS_IMPL(ns, Probe, element_geometries); \
  OBJECT_ACCESSOR_NS_IMPL(ns, Probe, impulse_responses);  \
  OBJECT_ACCESSOR_NS_IMPL(ns, Probe, elements)

#define URX_MATLAB_RECEIVE_SETUP_IMPL(ns)                            \
  OBJECT_NS_IMPL(ns, ReceiveSetup);                                  \
  OBJECT_ACCESSOR_NS_IMPL(ns, ReceiveSetup, probe);                  \
  OBJECT_ACCESSOR_NS_IMPL(ns, ReceiveSetup, probe_transform);        \
  OBJECT_ACCESSOR_NS_IMPL(ns, ReceiveSetup, sampling_frequency);     \
  OBJECT_ACCESSOR_NS_IMPL(ns, ReceiveSetup, number_samples);         \
  OBJECT_ACCESSOR_NS_IMPL(ns, ReceiveSetup, active_elements);        \
  OBJECT_ACCESSOR_NS_IMPL(ns, ReceiveSetup, tgc_profile);            \
  OBJECT_ACCESSOR_NS_IMPL(ns, ReceiveSetup, tgc_sampling_frequency); \
  OBJECT_ACCESSOR_NS_IMPL(ns, ReceiveSetup, modulation_frequency);   \
  OBJECT_ACCESSOR_NS_IMPL(ns, ReceiveSetup, time_offset)

#define URX_MATLAB_TRANSFORM_IMPL(ns)               \
  OBJECT_NS_IMPL(ns, Transform);                    \
  OBJECT_ACCESSOR_NS_IMPL(ns, Transform, rotation); \
  OBJECT_ACCESSOR_NS_IMPL(ns, Transform, translation)

#define URX_MATLAB_TRANSMIT_SETUP_IMPL(ns)                     \
  OBJECT_NS_IMPL(ns, TransmitSetup);                           \
  OBJECT_ACCESSOR_NS_IMPL(ns, TransmitSetup, probe);           \
  OBJECT_ACCESSOR_NS_IMPL(ns, TransmitSetup, wave);            \
  OBJECT_ACCESSOR_NS_IMPL(ns, TransmitSetup, active_elements); \
  OBJECT_ACCESSOR_NS_IMPL(ns, TransmitSetup, excitations);     \
  OBJECT_ACCESSOR_NS_IMPL(ns, TransmitSetup, delays);          \
  OBJECT_ACCESSOR_NS_IMPL(ns, TransmitSetup, probe_transform); \
  OBJECT_ACCESSOR_NS_IMPL(ns, TransmitSetup, time_offset)

#define URX_MATLAB_VECTOR3D_IMPL(ns)                           \
  OBJECT_NS2_IMPL(ns, Vector3D, Vector3D<double>);             \
  OBJECT_ACCESSOR_NS2_IMPL(ns, Vector3D, Vector3D<double>, x); \
  OBJECT_ACCESSOR_NS2_IMPL(ns, Vector3D, Vector3D<double>, y); \
  OBJECT_ACCESSOR_NS2_IMPL(ns, Vector3D, Vector3D<double>, z)

#define URX_MATLAB_VECTOR2D_IMPL(ns)                           \
  OBJECT_NS2_IMPL(ns, Vector2D, Vector2D<double>);             \
  OBJECT_ACCESSOR_NS2_IMPL(ns, Vector2D, Vector2D<double>, x); \
  OBJECT_ACCESSOR_NS2_IMPL(ns, Vector2D, Vector2D<double>, y)

#define URX_MATLAB_VERSION_IMPL(ns)            \
  OBJECT_NS_IMPL(ns, Version);                 \
  OBJECT_ACCESSOR_NS_IMPL(ns, Version, major); \
  OBJECT_ACCESSOR_NS_IMPL(ns, Version, minor); \
  OBJECT_ACCESSOR_NS_IMPL(ns, Version, patch)

#define URX_MATLAB_WAVE_IMPL(ns)                                \
  OBJECT_NS_IMPL(ns, Wave);                                     \
  OBJECT_ACCESSOR_NS_IMPL(ns, Wave, type);                      \
  OBJECT_ACCESSOR_NS_IMPL(ns, Wave, time_zero);                 \
  OBJECT_ACCESSOR_NS_IMPL(ns, Wave, time_zero_reference_point); \
  OBJECT_ACCESSOR_NS_IMPL(ns, Wave, parameters)

#define _URX_MATLAB_STREAM_IMPL(ns, type)                                                          \
  void *CONCAT4(ns, Stream, new, type)(const char *filename, void *shared_dataset) {               \
    urxIncAllocCount();                                                                            \
    if constexpr (urx::stringViewEqual(urx::type##_str, "raw")) {                                  \
      auto retval = new urx::utils::io::Stream(                                                    \
          filename, *static_cast<std::shared_ptr<urx::Dataset> *>(shared_dataset));                \
      urxGetLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n"               \
                  << std::flush;                                                                   \
      return retval;                                                                               \
    } else {                                                                                       \
      auto retval = new std::shared_ptr<urx::utils::io::Stream>(new urx::utils::io::Stream(        \
          filename, *static_cast<std::shared_ptr<urx::Dataset> *>(shared_dataset)));               \
      urxGetLog() << reinterpret_cast<size_t>(retval) << " "                                       \
                  << reinterpret_cast<size_t>(retval->get()) << " " << __FUNCTION__ << "\n";       \
      return retval;                                                                               \
    }                                                                                              \
  }                                                                                                \
  void CONCAT4(ns, Stream, delete, type)(void *raw_stream) {                                       \
    urxDecAllocCount();                                                                            \
    urxGetLog() << reinterpret_cast<size_t>(raw_stream) << " " << __FUNCTION__ << "\n"             \
                << std::flush;                                                                     \
    auto stream = static_cast<urx::PtrTypeT<urx::utils::io::Stream, urx::type##_str>>(raw_stream); \
    delete stream;                                                                                 \
  }                                                                                                \
  void *CONCAT4(ns, Stream, dataset, type)(void *raw_stream) {                                     \
    auto stream = urx::staticCastType<urx::utils::io::Stream, urx::type##_str>(raw_stream);        \
    urxIncAllocCount();                                                                            \
    auto retval = new std::shared_ptr<urx::Dataset>(stream->getDataset());                         \
    urxGetLog() << reinterpret_cast<size_t>(retval) << " "                                         \
                << reinterpret_cast<size_t>(retval->get()) << " " << __FUNCTION__ << "\n";         \
    return retval;                                                                                 \
  }                                                                                                \
  void CONCAT4(ns, Stream, save_to_file, type)(void *raw_stream) {                                 \
    auto stream = urx::staticCastType<urx::utils::io::Stream, urx::type##_str>(raw_stream);        \
    stream->saveToFile();                                                                          \
  }                                                                                                \
  void CONCAT4(ns, Stream, load_from_file, type)(void *raw_stream) {                               \
    auto stream = urx::staticCastType<urx::utils::io::Stream, urx::type##_str>(raw_stream);        \
    stream->loadFromFile();                                                                        \
  }                                                                                                \
  void CONCAT4(ns, Stream, set_reader_options, type)(void *raw_stream, int raw_data_load_policy) { \
    auto stream = urx::staticCastType<urx::utils::io::Stream, urx::type##_str>(raw_stream);        \
    stream->readerOptions().setRawDataLoadPolicy(                                                  \
        static_cast<urx::utils::io::RawDataLoadPolicy>(raw_data_load_policy));                     \
  }                                                                                                \
  void CONCAT4(ns, Stream, set_writer_options, type)(void *raw_stream, bool chunk_group_data,      \
                                                     bool clean_unusable_data, bool check_data) {  \
    auto stream = urx::staticCastType<urx::utils::io::Stream, urx::type##_str>(raw_stream);        \
    stream->writerOptions().setChunkGroupData(chunk_group_data);                                   \
    stream->writerOptions().setCleanUnusableData(clean_unusable_data);                             \
    stream->writerOptions().setCheckData(check_data);                                              \
  }                                                                                                \
  void *CONCAT5(ns, Stream, create_group_data, type, raw)(void *raw_stream, void *shared_group,    \
                                                          double timestamp) {                      \
    auto stream = urx::staticCastType<urx::utils::io::Stream, urx::type##_str>(raw_stream);        \
    urxIncAllocCount();                                                                            \
    auto retval = new urx::utils::io::GroupDataStream(stream->createGroupData(                     \
        *static_cast<std::shared_ptr<urx::Group> *>(shared_group), urx::DoubleNan(timestamp)));    \
    urxGetLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n" << std::flush;  \
    return retval;                                                                                 \
  }                                                                                                \
  void *CONCAT5(ns, Stream, create_group_data, type, shared)(void *raw_stream, void *shared_group, \
                                                             double timestamp) {                   \
    auto stream = urx::staticCastType<urx::utils::io::Stream, urx::type##_str>(raw_stream);        \
    urxIncAllocCount();                                                                            \
    auto retval =                                                                                  \
        new std::shared_ptr<urx::utils::io::GroupDataStream>(new urx::utils::io::GroupDataStream(  \
            stream->createGroupData(*static_cast<std::shared_ptr<urx::Group> *>(shared_group),     \
                                    urx::DoubleNan(timestamp))));                                  \
    urxGetLog() << reinterpret_cast<size_t>(retval) << " "                                         \
                << reinterpret_cast<size_t>(retval->get()) << " " << __FUNCTION__ << "\n";         \
    return retval;                                                                                 \
  }                                                                                                \
  void CONCAT4(ns, Stream, read_raw_data, type)(                                                   \
      void *raw_stream, size_t group_data, void *shared_raw_data, size_t sequence_urx_raw_data,    \
      size_t sequence_h5_raw_data, size_t count) {                                                 \
    auto stream = urx::staticCastType<urx::utils::io::Stream, urx::type##_str>(raw_stream);        \
    std::shared_ptr<urx::RawData> *raw_data =                                                      \
        static_cast<std::shared_ptr<urx::RawData> *>(shared_raw_data);                             \
    stream->readRawData(group_data, *raw_data, sequence_urx_raw_data, sequence_h5_raw_data,        \
                        count);                                                                    \
  }                                                                                                \
  FORCE_SEMICOLON

#define URX_MATLAB_STREAM_IMPL(ns)  \
  _URX_MATLAB_STREAM_IMPL(ns, raw); \
  _URX_MATLAB_STREAM_IMPL(ns, shared)

#define _URX_MATLAB_GROUP_DATA_STREAM_IMPL(ns, type)                                              \
  void CONCAT4(ns, GroupDataStream, delete, type)(void *raw_group_data_stream) {                  \
    urxDecAllocCount();                                                                           \
    urxGetLog() << reinterpret_cast<size_t>(raw_group_data_stream) << " " << __FUNCTION__ << "\n" \
                << std::flush;                                                                    \
    auto group_data_stream =                                                                      \
        static_cast<urx::PtrTypeT<urx::utils::io::GroupDataStream, urx::type##_str>>(             \
            raw_group_data_stream);                                                               \
    delete group_data_stream;                                                                     \
  }                                                                                               \
  void CONCAT4(ns, GroupDataStream, append, type)(                                                \
      void *raw_group_data_stream, void *shared_raw_data, double sequence_timestamp,              \
      double *event_timestamp, size_t event_timestamp_size) {                                     \
    auto group_data_stream =                                                                      \
        urx::staticCastType<urx::utils::io::GroupDataStream, urx::type##_str>(                    \
            raw_group_data_stream);                                                               \
    std::shared_ptr<urx::RawData> *raw_data =                                                     \
        static_cast<std::shared_ptr<urx::RawData> *>(shared_raw_data);                            \
    const std::vector<double> v_event_timestamp = {event_timestamp,                               \
                                                   event_timestamp + event_timestamp_size};       \
    group_data_stream->append(*raw_data, sequence_timestamp, v_event_timestamp);                  \
  }                                                                                               \
  void *CONCAT4(ns, GroupDataStream, get_group_data, type)(void *raw_group_data_stream) {         \
    auto group_data_stream =                                                                      \
        urx::staticCastType<urx::utils::io::GroupDataStream, urx::type##_str>(                    \
            raw_group_data_stream);                                                               \
    return &group_data_stream->getGroupData();                                                    \
  }                                                                                               \
  FORCE_SEMICOLON

#define URX_MATLAB_GROUP_DATA_STREAM_IMPL(ns)  \
  _URX_MATLAB_GROUP_DATA_STREAM_IMPL(ns, raw); \
  _URX_MATLAB_GROUP_DATA_STREAM_IMPL(ns, shared)

#define _URX_MATLAB_GROUP_DATA_READER_IMPL(ns, type)                                             \
  void *CONCAT5(ns, GroupDataReader, new, type, raw)(void *raw_group_data) {                     \
    urxIncAllocCount();                                                                          \
    if constexpr (urx::stringViewEqual(urx::type##_str, "raw")) {                                \
      auto retval =                                                                              \
          new urx::utils::GroupDataReader(*static_cast<urx::GroupData *>(raw_group_data));       \
      urxGetLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n"             \
                  << std::flush;                                                                 \
      return retval;                                                                             \
    } else {                                                                                     \
      auto retval = new std::shared_ptr<urx::utils::GroupDataReader>(                            \
          new urx::utils::GroupDataReader(*static_cast<urx::GroupData *>(raw_group_data)));      \
      urxGetLog() << reinterpret_cast<size_t>(retval) << " "                                     \
                  << reinterpret_cast<size_t>(retval->get()) << " " << __FUNCTION__ << "\n";     \
      return retval;                                                                             \
    }                                                                                            \
  }                                                                                              \
  void *CONCAT5(ns, GroupDataReader, new, type, shared)(void *shared_group_data) {               \
    urxIncAllocCount();                                                                          \
    if constexpr (urx::stringViewEqual(urx::type##_str, "raw")) {                                \
      auto retval = new urx::utils::GroupDataReader(                                             \
          **static_cast<std::shared_ptr<urx::GroupData> *>(shared_group_data));                  \
      urxGetLog() << reinterpret_cast<size_t>(retval) << " " << __FUNCTION__ << "\n"             \
                  << std::flush;                                                                 \
      return retval;                                                                             \
    } else {                                                                                     \
      auto retval =                                                                              \
          new std::shared_ptr<urx::utils::GroupDataReader>(new urx::utils::GroupDataReader(      \
              **static_cast<std::shared_ptr<urx::GroupData> *>(shared_group_data)));             \
      urxGetLog() << reinterpret_cast<size_t>(retval) << " "                                     \
                  << reinterpret_cast<size_t>(retval->get()) << " " << __FUNCTION__ << "\n";     \
      return retval;                                                                             \
    }                                                                                            \
  }                                                                                              \
  void CONCAT4(ns, GroupDataReader, delete, type)(void *raw_reader) {                            \
    urxDecAllocCount();                                                                          \
    urxGetLog() << reinterpret_cast<size_t>(raw_reader) << " " << __FUNCTION__ << "\n"           \
                << std::flush;                                                                   \
    auto reader =                                                                                \
        static_cast<urx::PtrTypeT<urx::utils::GroupDataReader, urx::type##_str>>(raw_reader);    \
    delete reader;                                                                               \
  }                                                                                              \
  size_t CONCAT4(ns, GroupDataReader, sequencesCount, type)(void *raw_reader) {                  \
    auto reader = urx::staticCastType<urx::utils::GroupDataReader, urx::type##_str>(raw_reader); \
    return reader->sequencesCount();                                                             \
  }                                                                                              \
  size_t CONCAT4(ns, GroupDataReader, eventsCount, type)(void *raw_reader) {                     \
    auto reader = urx::staticCastType<urx::utils::GroupDataReader, urx::type##_str>(raw_reader); \
    return reader->eventsCount();                                                                \
  }                                                                                              \
  size_t CONCAT4(ns, GroupDataReader, channelsCount, type)(void *raw_reader, size_t event_idx) { \
    auto reader = urx::staticCastType<urx::utils::GroupDataReader, urx::type##_str>(raw_reader); \
    return reader->channelsCount(event_idx);                                                     \
  }                                                                                              \
  size_t CONCAT4(ns, GroupDataReader, samplesCount, type)(void *raw_reader, size_t event_idx) {  \
    auto reader = urx::staticCastType<urx::utils::GroupDataReader, urx::type##_str>(raw_reader); \
    return reader->samplesCount(event_idx);                                                      \
  }                                                                                              \
  size_t CONCAT4(ns, GroupDataReader, offset, type)(void *raw_reader, size_t sequence_idx,       \
                                                    size_t event_idx, size_t channel_idx,        \
                                                    size_t sample_idx) {                         \
    auto reader = urx::staticCastType<urx::utils::GroupDataReader, urx::type##_str>(raw_reader); \
    return reader->offset(sequence_idx, event_idx, channel_idx, sample_idx);                     \
  }                                                                                              \
  size_t CONCAT4(ns, GroupDataReader, sampleByteSize, type)(void *raw_reader) {                  \
    auto reader = urx::staticCastType<urx::utils::GroupDataReader, urx::type##_str>(raw_reader); \
    return reader->sampleByteSize();                                                             \
  }                                                                                              \
  FORCE_SEMICOLON

#define URX_MATLAB_GROUP_DATA_READER_IMPL(ns)  \
  _URX_MATLAB_GROUP_DATA_READER_IMPL(ns, raw); \
  _URX_MATLAB_GROUP_DATA_READER_IMPL(ns, shared)

// NOLINTEND(bugprone-macro-parentheses)

#endif  // #define URX_MATLAB_BINDING_IMPL
