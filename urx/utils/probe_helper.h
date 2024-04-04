#pragma once

#include <cstdint>

#include <urx/probe.h>
#include <urx/utils/export.h>
#include <urx/vector.h>

namespace urx::utils::probe_helper {
URX_UTILS_EXPORT void updateRcaElementsPositions(Probe& rca, const Vector2D<uint32_t>& nb_elements,
                                                 const Vector2D<double>& pitch);
URX_UTILS_EXPORT void updateMatrixElementsPositions(Probe& matrix,
                                                    const Vector2D<uint32_t>& nb_elements,
                                                    const Vector2D<double>& pitch);
URX_UTILS_EXPORT void updateLinearElementsPositions(Probe& linear, uint32_t nb_elements,
                                                    double pitch);
}  // namespace urx::utils::probe_helper
