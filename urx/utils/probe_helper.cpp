#include <algorithm>
#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include <urx/element.h>
#include <urx/probe.h>
#include <urx/transform.h>
#include <urx/utils/probe_helper.h>
#include <urx/vector.h>

namespace urx::utils::probe_helper {

void updateRcaElementsPositions(Probe& rca, const Vector2D<uint32_t>& nb_elements,
                                const Vector2D<double>& pitch) {
  rca.elements.resize(static_cast<size_t>(nb_elements.x) + nb_elements.y);

  const double xmin = -pitch.x * (nb_elements.x - 1.) / 2.;
  for (uint32_t i = 0; i < nb_elements.x; i++) {
    Element el;
    el.transform.translation = {xmin + i * pitch.x, 0., 0.};
    el.transform.rotation = {0., 0., 0.};
    rca.elements[i] = std::move(el);
  }

  const double ymin = -pitch.y * (nb_elements.y - 1.) / 2.;
  for (uint32_t i = nb_elements.y; i < rca.elements.size(); i++) {
    Element el;
    el.transform.translation = {0., ymin + (i - nb_elements.y) * pitch.y, 0.};
    el.transform.rotation = {0., 0., 0.};
    rca.elements[i] = std::move(el);
  }
}

void updateMatrixElementsPositions(Probe& matrix, const Vector2D<uint32_t>& nb_elements,
                                   const Vector2D<double>& pitch) {
  matrix.elements.resize(static_cast<size_t>(nb_elements.x) * nb_elements.y);

  const double xmin = -pitch.x * (nb_elements.x - 1.) / 2.;
  const double ymin = -pitch.y * (nb_elements.y - 1.) / 2.;
  for (uint32_t i = 0; i < nb_elements.y; i++) {
    for (uint32_t j = 0; j < nb_elements.x; j++) {
      Element el;
      el.transform.translation = {xmin + j * pitch.x, ymin + i * pitch.y, 0.};
      el.transform.rotation = {};
      matrix.elements[static_cast<size_t>(j) + static_cast<size_t>(i) * nb_elements.y] =
          std::move(el);
    }
  }
}

void updateLinearElementsPositions(Probe& linear, uint32_t nb_elements, double pitch) {
  linear.elements.resize(nb_elements);
  for (uint32_t i = 0; i < nb_elements; ++i) {
    // element position
    const double xmin = -pitch * static_cast<double>(nb_elements - 1) / 2.;
    Element el;
    el.transform.translation = {xmin + i * pitch, 0., 0.};
    el.transform.rotation = {0., 0., 0.};
    linear.elements[i] = el;
  }
}

void updateRcaElementsRectGeometry(Probe& probe, const Vector2D<uint32_t>& nb_elements,
                                   const Vector2D<double>& col_size,
                                   const Vector2D<double>& row_size) {
  auto& elements = probe.elements;
  const size_t n_els = elements.size();

  if (nb_elements.x + nb_elements.y != n_els) {
    throw std::runtime_error(
        "updateRcaElementsRectGeometry(): nb_elements.x + nb_elements.y != probe.elements.size()");
  }

  if (n_els == 0) {
    std::cout << "[warning] updateRcaElementsRectGeometry(): no probe elements\n";
    return;
  }

  // Setup the geometry shared accross rows and cols elements
  const ElementGeometry col_el_geom{{{-col_size.x / 2, -col_size.y / 2, 0},
                                     {col_size.x / 2, -col_size.y / 2, 0},
                                     {col_size.x / 2, col_size.y / 2, 0},
                                     {-col_size.x / 2, col_size.y / 2, 0}}};
  const ElementGeometry row_el_geom{{{-row_size.x / 2, -row_size.y / 2, 0},
                                     {row_size.x / 2, -row_size.y / 2, 0},
                                     {row_size.x / 2, row_size.y / 2, 0},
                                     {-row_size.x / 2, row_size.y / 2, 0}}};

  const auto sh_col_el_geom = std::make_shared<ElementGeometry>(col_el_geom);
  const auto sh_row_el_geom = std::make_shared<ElementGeometry>(row_el_geom);

  // Register geometries
  probe.element_geometries.clear();
  probe.element_geometries.emplace_back(sh_col_el_geom);
  probe.element_geometries.emplace_back(sh_row_el_geom);

  // Set the probe elements geometry

  for (size_t i = 0; i < nb_elements.x; i++) {
    elements[i].element_geometry = sh_col_el_geom;
  }

  for (size_t i = nb_elements.x; i < n_els; i++) {
    elements[i].element_geometry = sh_row_el_geom;
  }
}

void updateMatrixElementsRectGeometry(Probe& probe, const Vector2D<double>& size) {
  updateLinearElementsRectGeometry(probe, size);
}

void updateLinearElementsRectGeometry(Probe& probe, const Vector2D<double>& size) {
  if (probe.elements.empty()) {
    std::cout << "[warning] updateRcaElementsRectGeometry(): no probe elements\n";
    return;
  }

  // Setup the geometry shared accross all elements
  const ElementGeometry el_geom{{{-size.x / 2, -size.y / 2, 0},
                                 {size.x / 2, -size.y / 2, 0},
                                 {size.x / 2, size.y / 2, 0},
                                 {-size.x / 2, size.y / 2, 0}}};
  const auto sh_el_geom = std::make_shared<ElementGeometry>(el_geom);

  // Register geometries
  probe.element_geometries.clear();
  probe.element_geometries.emplace_back(sh_el_geom);

  // Set the probe elements geometry
  for (auto& element : probe.elements) {
    element.element_geometry = sh_el_geom;
  }
}

}  // namespace urx::utils::probe_helper
