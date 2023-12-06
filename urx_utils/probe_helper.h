#pragma once

#include <cstdint>

#include <urx/probe.h>
#include <urx/vector.h>

namespace urx::utils {
void updateRcaElementsPositions(Probe& rca, const Vector2D<uint32_t>& nb_elements,
                                const Vector2D<double>& pitch);
void updateMatrixElementsPositions(Probe& matrix, const Vector2D<uint32_t>& nb_elements,
                                   const Vector2D<double>& pitch);
void updateLinearElementsPositions(Probe& linear, uint32_t nb_elements, double pitch);
}  // namespace urx::utils
