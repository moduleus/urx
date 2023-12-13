#include <cstddef>
#include <memory>
#include <vector>

#include <urx/element.h>
#include <urx/probe.h>
#include <urx/vector.h>
#include <urx_utils/probe_helper.h>

namespace urx::utils {

void updateRcaElementsPositions(Probe& rca, const Vector2D<uint32_t>& nb_elements,
                                const Vector2D<double>& pitch) {
  rca.elements.resize(static_cast<size_t>(nb_elements.x) + nb_elements.y);

  const double xmin = -pitch.x * (nb_elements.x - 1.) / 2.;
  for (uint32_t i = 0; i < nb_elements.x; i++) {
    rca.elements[i] = Element{.transform = {{xmin + i * pitch.x, 0., 0.}, {0., 0., 0.}}};
  }

  const double ymin = -pitch.y * (nb_elements.y - 1.) / 2.;
  for (uint32_t i = nb_elements.y; i < rca.elements.size(); i++) {
    rca.elements[i] =
        Element{.transform = {{0., ymin + (i - nb_elements.y) * pitch.y, 0.}, {0., 0., 0.}}};
  }
}

void updateMatrixElementsPositions(Probe& matrix, const Vector2D<uint32_t>& nb_elements,
                                   const Vector2D<double>& pitch) {
  matrix.elements.resize(static_cast<size_t>(nb_elements.x) * nb_elements.y);

  const double xmin = -pitch.x * (nb_elements.x - 1.) / 2.;
  const double ymin = -pitch.y * (nb_elements.y - 1.) / 2.;
  for (uint32_t i = 0; i < nb_elements.y; i++) {
    for (uint32_t j = 0; j < nb_elements.x; j++) {
      matrix.elements[static_cast<size_t>(j) + static_cast<size_t>(i) * nb_elements.y] =
          Element{.transform = {{xmin + j * pitch.x, ymin + i * pitch.y, 0.}, {}}};
    }
  }
}

void updateLinearElementsPositions(Probe& linear, uint32_t nb_elements, double pitch) {
  linear.elements.resize(nb_elements);
  for (uint32_t i = 0; i < nb_elements; ++i) {
    // element position
    const double xmin = -pitch * static_cast<double>(nb_elements - 1) / 2.;
    linear.elements[i] = Element{.transform = {{xmin + i * pitch, 0., 0.}, {0., 0., 0.}}};
  }
}

}  // namespace urx::utils
