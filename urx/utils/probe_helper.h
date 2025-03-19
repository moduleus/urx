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

URX_UTILS_EXPORT void updateRcaElementsRectGeometry(Probe& probe,
                                                    const Vector2D<uint32_t>& nb_elements,
                                                    const Vector2D<double>& col_size,
                                                    const Vector2D<double>& row_size);
URX_UTILS_EXPORT void updateMatrixElementsRectGeometry(Probe& probe,
                                                       const Vector2D<uint32_t>& nb_elements,
                                                       const Vector2D<double>& size);
URX_UTILS_EXPORT void updateLinearElementsGeometry(Probe& probe, uint32_t nb_elements,
                                                   const Vector2D<double>& size);
}  // namespace urx::utils::probe_helper
