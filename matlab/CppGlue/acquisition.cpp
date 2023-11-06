#include "acquisition.h"

#include <iostream>
#include <string>
#include <vector>

#include <uff/acquisition.h>

uint64_t ptr2val(void *p) {return (uint64_t)p; }
void* val2ptr(uint64_t v) {return (void*)v; }

void std_string_set(void *this_ptr, const char *v) {*(std::string*)this_ptr = v;}
const char * std_string_get(void *this_ptr) { return ((std::string*)this_ptr)->c_str(); }

int acquisition_sizeof(void) { return sizeof (uff::Acquisition); }
void* acquisition_new(void) {return new uff::Acquisition; }
void acquisition_delete(void *this_ptr) { delete (uff::Acquisition*)this_ptr; }

void* acquisition_authors(void *this_ptr) { return &((uff::Acquisition*)this_ptr)->authors; }
void* acquisition_description(void *this_ptr) { return &((uff::Acquisition*)this_ptr)->description; }
void* acquisition_local_time(void *this_ptr) { return &((uff::Acquisition*)this_ptr)->local_time; }
void* acquisition_country_code(void *this_ptr) { return &((uff::Acquisition*)this_ptr)->country_code; }
void* acquisition_system(void *this_ptr) { return &((uff::Acquisition*)this_ptr)->system; }
void* acquisition_sound_speed(void *this_ptr) { return &((uff::Acquisition*)this_ptr)->sound_speed; }
void* acquisition_groups(void *this_ptr) { return &((uff::Acquisition*)this_ptr)->groups; }

int group_sizeof(void) { return sizeof (uff::Group); }
void* group_new(void) {return new uff::Group; }
void group_delete(void *this_ptr) { delete (uff::Group*)this_ptr; }

int group_sampling_type_sizeof(void) { return sizeof(uff::Group::SamplingType); }
void* group_sampling_type(void *this_ptr) { return &((uff::Group*)this_ptr)->sampling_type; }
void* group_sequence(void *this_ptr) { return &((uff::Group*)this_ptr)->sequence; }
void* group_description(void *this_ptr) { return &((uff::Group*)this_ptr)->description; }

void* std_vector_uff_group_new(void) { return new std::vector<uff::Group>; }
void std_vector_uff_group_delete(void* this_ptr) { delete (std::vector<uff::Group>*)this_ptr; }
void* std_vector_uff_group_data(void *this_ptr) { return ((std::vector<uff::Group>*)this_ptr)->data(); }
size_t std_vector_uff_group_size(void *this_ptr) { return ((std::vector<uff::Group>*)this_ptr)->size(); }
void std_vector_uff_group_clear(void *this_ptr) { ((std::vector<uff::Group>*)this_ptr)->clear(); }
void std_vector_uff_group_push_back(void *this_ptr, void *val) {
  ((std::vector<uff::Group>*)this_ptr)->push_back(*(uff::Group*)val);
}
void std_vector_uff_group_copy(void *this_ptr, void *other_ptr) {
  *(std::vector<uff::Group>*)this_ptr = *(std::vector<uff::Group>*)other_ptr;
}

int event_sizeof(void) { return sizeof (uff::Event); }
void* event_new(void) {return new uff::Event; }
void event_delete(void *this_ptr) { delete (uff::Event*)this_ptr; }

void* event_transmit_setup(void *this_ptr) { return &((uff::Event*)this_ptr)->transmit_setup; }
void* event_receive_setup(void *this_ptr) { return &((uff::Event*)this_ptr)->receive_setup; }

void* std_vector_uff_event_new(void) { return new std::vector<uff::Event>; }
void std_vector_uff_event_delete(void* this_ptr) { delete (std::vector<uff::Event>*)this_ptr; }
void* std_vector_uff_event_data(void *this_ptr) { return ((std::vector<uff::Event>*)this_ptr)->data(); }
size_t std_vector_uff_event_size(void *this_ptr) { return ((std::vector<uff::Event>*)this_ptr)->size(); }
void std_vector_uff_event_clear(void *this_ptr) { ((std::vector<uff::Event>*)this_ptr)->clear(); }
void std_vector_uff_event_push_back(void *this_ptr, void *val) {
  ((std::vector<uff::Event>*)this_ptr)->push_back(*(uff::Event*)val);
}
void std_vector_uff_event_copy(void *this_ptr, void *other_ptr) {
  *(std::vector<uff::Event>*)this_ptr = *(std::vector<uff::Event>*)other_ptr;
}
