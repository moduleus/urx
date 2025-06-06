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
URX_UTILS_EXPORT void updateCurvilinearElementsPositions(Probe& curvi, uint32_t nb_elements,
                                                         double pitch, double curvature);

URX_UTILS_EXPORT void updateRcaElementsRectGeometry(Probe& probe,
                                                    const Vector2D<uint32_t>& nb_elements,
                                                    const Vector2D<double>& col_size,
                                                    const Vector2D<double>& row_size);
URX_UTILS_EXPORT void updateMatrixElementsRectGeometry(Probe& probe, const Vector2D<double>& size);
URX_UTILS_EXPORT void updateLinearElementsRectGeometry(Probe& probe, const Vector2D<double>& size);
URX_UTILS_EXPORT void updateCurvilinearElementsRectGeometry(Probe& probe,
                                                            const Vector2D<double>& size);

URX_UTILS_EXPORT Probe createStandardRca(uint32_t n_x, uint32_t n_y, double pitch_x, double pitch_y,
                                         double element_width_x, double element_width_y,
                                         double element_height_x, double element_height_y,
                                         double delta_z);

URX_UTILS_EXPORT Probe createStandardLinear(uint32_t n, double pitch, double element_width,
                                            double element_height);

URX_UTILS_EXPORT Probe createStandardCurviLinear(uint32_t n, double pitch, double element_width,
                                                 double element_height, double curvature);

URX_UTILS_EXPORT Probe createStandardMatrix(uint32_t n_x, uint32_t n_y, double pitch_x,
                                            double pitch_y, double element_width,
                                            double element_height);
}  // namespace urx::utils::probe_helper
