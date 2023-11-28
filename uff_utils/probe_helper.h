#pragma once

#include <cstddef>

#include <uff/probe.h>
#include <uff/vector.h>

namespace uff::utils {
void updateRcaElementsPositions(Probe& rca, const Vector2D<uint32_t>& nb_elements,
                                const Vector2D<double>& pitch);
void updateMatrixElementsPositions(Probe& matrix, const Vector2D<uint32_t>& nb_elements,
                                   const Vector2D<double>& pitch);
void updateLinearElementsPositions(Probe& linear, uint32_t nb_elements, double pitch);
}  // namespace uff::utils
