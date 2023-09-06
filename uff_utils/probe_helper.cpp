#include <uff/linear_array.h>
#include <uff/matrix_array.h>
#include <uff/rca_array.h>
#include <uff/vector.h>

#include <vector>

namespace uff::utils {

void updateElementsPositions(RcaArray& rca) {
  rca.elements.resize(static_cast<size_t>(rca.nb_elements.x) + rca.nb_elements.y);

  const double xmin = -rca.pitch.x * (rca.nb_elements.x - 1.) / 2.;
  for (uint32_t i = 0; i < rca.nb_elements.x; i++) {
    rca.elements[i] = Element{.transform = {Vector3D<double>(xmin + i * rca.pitch.x, 0., 0.),
                                            Vector3D<double>(0., 0., 0.)}};
  }

  const double ymin = -rca.pitch.y * (rca.nb_elements.y - 1.) / 2.;
  for (uint32_t i = rca.nb_elements.y; i < rca.elements.size(); i++) {
    rca.elements[i] = Element{
        .transform = {Vector3D<double>(0., ymin + (i - rca.nb_elements.y) * rca.pitch.y, 0.),
                      Vector3D<double>(0., 0., 0.)}};
  }
}

void updateElementsPositions(MatrixArray& matrix) {
  matrix.elements.resize(static_cast<size_t>(matrix.nb_elements.x) * matrix.nb_elements.y);

  double xmin = -matrix.pitch.x * (matrix.nb_elements.x - 1.f) / 2.f;
  double ymin = -matrix.pitch.y * (matrix.nb_elements.y - 1.f) / 2.f;
  for (uint32_t i = 0; i < matrix.nb_elements.y; i++) {
    for (uint32_t j = 0; j < matrix.nb_elements.x; j++) {
      matrix.elements[static_cast<size_t>(j) + static_cast<size_t>(i) * matrix.nb_elements.y] =
          Element{.transform = {
                      Vector3D<double>(xmin + j * matrix.pitch.x, ymin + i * matrix.pitch.y, 0.f),
                      Vector3D<double>()}};
    }
  }
}

void updateElements(LinearArray& linear) {
  linear.elements.resize(linear.nb_elements);
  for (uint32_t i = 0; i < linear.nb_elements; ++i) {
    // element position
    double xmin = -linear.pitch * static_cast<float>(linear.nb_elements - 1) / 2.f;
    linear.elements[i] = Element{.transform = {Vector3D<double>(xmin + i * linear.pitch, 0., 0.),
                                               Vector3D<double>(0., 0., 0.)}};
  }
}

}  // namespace uff::utils