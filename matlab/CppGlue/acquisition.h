#ifndef ACQUISITION_H_
#define ACQUISITION_H_

#ifdef __cplusplus
/* #include  <cstdint> */
#else
#endif
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

  uint64_t ptr2val(void *p);
  void* val2ptr(uint64_t v);

  void std_string_set(void *this_ptr, const char *v);
  const char * std_string_get(void *this_ptr);

  int acquisition_sizeof(void);
  void* acquisition_new(void);
  void acquisition_delete(void *this_ptr);

  void* acquisition_authors(void *this_ptr);
  void* acquisition_description(void *this_ptr);
  void* acquisition_sound_speed(void *this_ptr);
  void* acquisition_groups(void *this_ptr);

  int group_sizeof(void);
  void* group_new(void);
  void group_delete(void *this_ptr);

  int group_sampling_type_sizeof(void);
  void* group_sampling_type(void *this_ptr);
  void* group_description(void* this_ptr);

  void* std_vector_uff_group_new(void);
  void std_vector_uff_group_delete(void* this_ptr);
  void* std_vector_uff_group_data(void *this_ptr);
  void std_vector_uff_group_insert(void *this_ptr, int pos, void *val);
  void std_vector_uff_group_erase(void *this_ptr, int pos);
  size_t std_vector_uff_group_size(void *this_ptr);
  void std_vector_uff_group_clear(void *this_ptr);
  void std_vector_uff_group_push_back(void *this_ptr, void *val);
  void std_vector_uff_group_copy(void *this_ptr, void *other_ptr);
#ifdef __cplusplus
}
#endif

#endif // ACQUISITION_H_
