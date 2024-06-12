#include <complex>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>

#include <urx/acquisition.h>
#include <urx/config.h>
#include <urx/dataset.h>
#include <urx/detail/raw_data.h>
#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/event.h>
#include <urx/excitation.h>
#include <urx/group.h>
#include <urx/group_data.h>
#include <urx/impulse_response.h>
#include <urx/matlab/bindings.h>
#include <urx/matlab/bindings_impl.h>
#include <urx/probe.h>
#include <urx/receive_setup.h>
#include <urx/transform.h>
#include <urx/transmit_setup.h>
#include <urx/vector.h>
#include <urx/version.h>
#include <urx/wave.h>

#ifdef URX_WITH_HDF5
#include <urx/utils/io/reader.h>
#include <urx/utils/io/writer.h>
#endif

#define xstr(s) str(s)
#define str(s) #s

namespace {
constexpr bool LOG_NEW_DELETE = false;

size_t urx_alloc_count = 0;
}  // namespace

std::ostream &urxGetLog() {
  if constexpr (LOG_NEW_DELETE) {
    static std::ofstream outfile("c:\\temp\\urx.log");
    return outfile;
  } else {
    std::ostream &outfile(std::cout);
    return outfile;
  }
}

void urxIncAllocCount() { urx_alloc_count++; }

void urxDecAllocCount() { urx_alloc_count--; }

#if WIN32
#include <windows.h>

BOOL WINAPI DllMain(HINSTANCE /*hinstDLL*/,  // handle to DLL module
                    DWORD fdwReason,         // reason for calling function
                    LPVOID /*lpReserved*/)   // reserved
{
  // Perform actions based on the reason for calling.
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      // Initialize once for each new process.
      // Return FALSE to fail DLL load.
      break;

    case DLL_THREAD_ATTACH:
      // Do thread-specific initialization.
      break;

    case DLL_THREAD_DETACH:
      // Do thread-specific cleanup.
      break;

    case DLL_PROCESS_DETACH:
      if (urx_alloc_count != 0) {
        urxGetLog() << "Urx Matlab wrapper: " << urx_alloc_count << " memory leaks\n";
      }
      break;
  }
  return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
#else
#endif

void std_string_set(void *this_ptr, const char *v) { *static_cast<std::string *>(this_ptr) = v; }
const char *std_string_get(void *this_ptr) { return static_cast<std::string *>(this_ptr)->c_str(); }

// NOLINTBEGIN(cppcoreguidelines-owning-memory,bugprone-macro-parentheses)

// NOLINTBEGIN(bugprone-unchecked-optional-access)

URX_MATLAB_ACQUISITION_IMPL(urx);
URX_MATLAB_DATASET_IMPL(urx);
URX_MATLAB_ELEMENT_GEOMETRY_IMPL(urx);
URX_MATLAB_ELEMENT_IMPL(urx);
URX_MATLAB_EVENT_IMPL(urx);
URX_MATLAB_EXCITATION_IMPL(urx);
URX_MATLAB_GROUP_DATA_IMPL(urx);
URX_MATLAB_GROUP_IMPL(urx);
URX_MATLAB_IMPULSE_RESPONSE_IMPL(urx);
URX_MATLAB_PROBE_IMPL(urx);
URX_MATLAB_RECEIVE_SETUP_IMPL(urx);
URX_MATLAB_TRANSFORM_IMPL(urx);
URX_MATLAB_TRANSMIT_SETUP_IMPL(urx);
URX_MATLAB_VECTOR3D_IMPL(urx);
URX_MATLAB_VECTOR2D_IMPL(urx);
URX_MATLAB_VERSION_IMPL(urx);
URX_MATLAB_WAVE_IMPL(urx);

// NOLINTEND(bugprone-unchecked-optional-access)

OBJECT_NS_RAW_DATA_IMPL(urx, RawData, int16_t, real, urx::RawDataNoInit<int16_t>);
OBJECT_NS_RAW_DATA_IMPL(urx, RawData, int16_t, complex, urx::RawDataNoInit<std::complex<int16_t>>);
OBJECT_NS_RAW_DATA_IMPL(urx, RawData, int32_t, real, urx::RawDataNoInit<int32_t>);
OBJECT_NS_RAW_DATA_IMPL(urx, RawData, int32_t, complex, urx::RawDataNoInit<std::complex<int32_t>>);
OBJECT_NS_RAW_DATA_IMPL(urx, RawData, float, real, urx::RawDataNoInit<float>);
OBJECT_NS_RAW_DATA_IMPL(urx, RawData, float, complex, urx::RawDataNoInit<std::complex<float>>);
OBJECT_NS_RAW_DATA_IMPL(urx, RawData, double, real, urx::RawDataNoInit<double>);
OBJECT_NS_RAW_DATA_IMPL(urx, RawData, double, complex, urx::RawDataNoInit<std::complex<double>>);

VECTOR_RAW_IMPL_RAW(uint8_t);
VECTOR_RAW_IMPL_RAW(uint16_t);
VECTOR_RAW_IMPL_RAW(uint32_t);
VECTOR_RAW_IMPL_RAW(uint64_t);
VECTOR_RAW_IMPL_RAW(int8_t);
VECTOR_RAW_IMPL_RAW(int16_t);
VECTOR_RAW_IMPL_RAW(int32_t);
VECTOR_RAW_IMPL_RAW(int64_t);
VECTOR_RAW_IMPL_RAW(float);
VECTOR_RAW_IMPL_RAW(double);
VECTOR_RAW_NS_IMPL(std, string);

VECTOR_RAW_NS_IMPL(urx, Element);
VECTOR_RAW_NS_IMPL(urx, Event);
VECTOR_RAW_NS_IMPL(urx, GroupData);
VECTOR_RAW_NS2_IMPL(urx, Vector3D, Vector3D<double>);

VECTOR_2D_RAW_IMPL(uint8_t);
VECTOR_2D_RAW_IMPL(uint16_t);
VECTOR_2D_RAW_IMPL(uint32_t);
VECTOR_2D_RAW_IMPL(uint64_t);
VECTOR_2D_RAW_IMPL(int8_t);
VECTOR_2D_RAW_IMPL(int16_t);
VECTOR_2D_RAW_IMPL(int32_t);
VECTOR_2D_RAW_IMPL(int64_t);
VECTOR_2D_RAW_IMPL(float);
VECTOR_2D_RAW_IMPL(double);
VECTOR_2D_RAW_NS_IMPL(std, string);

VECTOR_SHARED_NS_IMPL(urx, ElementGeometry);
VECTOR_SHARED_NS_IMPL(urx, Excitation);
VECTOR_SHARED_NS_IMPL(urx, Group);
VECTOR_SHARED_NS_IMPL(urx, ImpulseResponse);
VECTOR_SHARED_NS_IMPL(urx, Probe);

VECTOR_WEAK_NS_IMPL(urx, Excitation);

RAW_DATA_SHARED_NS_IMPL(urx, RawData);

uint64_t get_pointer(void *ptr) { return reinterpret_cast<uint64_t>(ptr); }

void *urx_load_from_file(const char *filename) {
#ifdef URX_WITH_HDF5
  return new std::shared_ptr<urx::Dataset>(urx::utils::io::reader::loadFromFile(filename));
#else
  return nullptr;
#endif
}

void urx_save_to_file(const char *filename, void *dataset) {
#ifdef URX_WITH_HDF5
  urx::utils::io::writer::saveToFile(filename,
                                     **static_cast<std::shared_ptr<urx::Dataset> *>(dataset));
#endif
}

// NOLINTEND(cppcoreguidelines-owning-memory,bugprone-macro-parentheses)
