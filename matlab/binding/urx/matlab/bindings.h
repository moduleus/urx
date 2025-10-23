#ifndef URX_MATLAB_BINDING
#define URX_MATLAB_BINDING

// MATLAB parser doesn't support __attribute__((visibility("default"))).
#ifndef UrxMatlabBinding_EXPORTS
#define URX_MATLAB_STATIC_DEFINE
#endif

#include <urx/config.h>
#include <urx/matlab/bindings_decl.h>
#include <urx/matlab/export.h>

// NOLINTBEGIN(readability-identifier-naming)

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>  // IWYU pragma: keep
#include <iosfwd>

URX_MATLAB_EXPORT std::ostream &urxGetLog();
#else
#include <stddef.h>  // IWYU pragma: keep
#include <stdint.h>  // IWYU pragma: keep
#endif

#ifdef __cplusplus
extern "C" {
#endif

URX_MATLAB_EXPORT void urxIncAllocCount();
URX_MATLAB_EXPORT void urxDecAllocCount();

URX_MATLAB_EXPORT void std_string_set(void *this_ptr, const char *v);
URX_MATLAB_EXPORT const char *std_string_get(void *this_ptr);

URX_MATLAB_EXPORT uint64_t get_pointer(void *ptr);

URX_MATLAB_ACQUISITION_DECL(urx);
OBJECT_ACCESSOR_NS_DECL(urx, Acquisition, groups_data);

URX_MATLAB_DATASET_DECL(urx);
URX_MATLAB_ELEMENT_GEOMETRY_DECL(urx);
URX_MATLAB_ELEMENT_DECL(urx);
URX_MATLAB_EVENT_DECL(urx);
URX_MATLAB_EXCITATION_DECL(urx);
URX_MATLAB_GROUP_DATA_DECL(urx);
URX_MATLAB_GROUP_DECL(urx);
URX_MATLAB_IMPULSE_RESPONSE_DECL(urx);
URX_MATLAB_PROBE_DECL(urx);
URX_MATLAB_RECEIVE_SETUP_DECL(urx);
URX_MATLAB_TRANSFORM_DECL(urx);
URX_MATLAB_TRANSMIT_SETUP_DECL(urx);
URX_MATLAB_VECTOR3D_DECL(urx);
URX_MATLAB_VECTOR2D_DECL(urx);
URX_MATLAB_VERSION_DECL(urx);
URX_MATLAB_WAVE_DECL(urx);

OBJECT_NS_RAW_DATA_DECL(urx, RawData);

VECTOR_RAW_DECL_RAW(uint8_t);
VECTOR_RAW_DECL_RAW(uint16_t);
VECTOR_RAW_DECL_RAW(uint32_t);
VECTOR_RAW_DECL_RAW(uint64_t);
VECTOR_RAW_DECL_RAW(int8_t);
VECTOR_RAW_DECL_RAW(int16_t);
VECTOR_RAW_DECL_RAW(int32_t);
VECTOR_RAW_DECL_RAW(int64_t);
VECTOR_RAW_DECL_RAW(float);
VECTOR_RAW_DECL_RAW(double);
VECTOR_RAW_NS_DECL(std, string);

VECTOR_RAW_NS_DECL(urx, Element);
VECTOR_RAW_NS_DECL(urx, Event);
VECTOR_RAW_NS_DECL(urx, GroupData);
VECTOR_RAW_NS_DECL(urx, Vector3D);

VECTOR_2D_RAW_DECL(uint8_t);
VECTOR_2D_RAW_DECL(uint16_t);
VECTOR_2D_RAW_DECL(uint32_t);
VECTOR_2D_RAW_DECL(uint64_t);
VECTOR_2D_RAW_DECL(int8_t);
VECTOR_2D_RAW_DECL(int16_t);
VECTOR_2D_RAW_DECL(int32_t);
VECTOR_2D_RAW_DECL(int64_t);
VECTOR_2D_RAW_DECL(float);
VECTOR_2D_RAW_DECL(double);
VECTOR_2D_RAW_NS_DECL(std, string);

VECTOR_SHARED_NS_DECL(urx, Probe);
VECTOR_SHARED_NS_DECL(urx, Excitation);
VECTOR_SHARED_NS_DECL(urx, Group);
VECTOR_SHARED_NS_DECL(urx, ElementGeometry);
VECTOR_SHARED_NS_DECL(urx, ImpulseResponse);
VECTOR_WEAK_NS_DECL(urx, Excitation);

RAW_DATA_SHARED_NS_DECL(urx, RawData);

#ifdef URX_WITH_HDF5

URX_MATLAB_EXPORT void * /*pointer to shared_ptr<Dataset>*/ urx_load_from_file(
    const char *filename);
URX_MATLAB_EXPORT void * /*pointer to shared_ptr<Dataset>*/ urx_load_from_file_options(
    const char *filename, int raw_data_load_policy);
URX_MATLAB_EXPORT void urx_save_to_file(const char *filename, void *shared_dataset);
URX_MATLAB_EXPORT void urx_save_to_file_options(const char *filename, void *shared_dataset,
                                                bool chunk_group_data, bool clean_unusable_data,
                                                bool check_data);

URX_MATLAB_STREAM_DECL(urx);
URX_MATLAB_GROUP_DATA_STREAM_DECL(urx);
URX_MATLAB_GROUP_DATA_READER_DECL(urx);

#endif

URX_MATLAB_EXPORT void *urx_clone_dataset(void *shared_dataset);

URX_MATLAB_EXPORT bool urx_validate_dataset(void *shared_dataset);

URX_MATLAB_EXPORT void urx_throw(const char *v);
URX_MATLAB_EXPORT void urx_cout(const char *v);
URX_MATLAB_EXPORT void urx_cerr(const char *v);

#ifdef __cplusplus
}
#endif

// NOLINTEND(readability-identifier-naming)

#endif  // #define URX_MATLAB_BINDING
