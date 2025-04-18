#include <algorithm>
#include <cmath>
#include <cstddef>
#include <ios>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <urx/element.h>
#include <urx/element_geometry.h>
#include <urx/enums.h>
#include <urx/impulse_response.h>
#include <urx/probe.h>
#include <urx/transform.h>
#include <urx/utils/probe_helper.h>
#include <urx/vector.h>

namespace urx::utils::probe_helper {

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

void updateCurvilinearElementsPositions(Probe& curvi, uint32_t nb_elements, double pitch,
                                        double curvature) {
  curvi.elements.resize(nb_elements);
  const double probe_width = pitch * (nb_elements - 1);
  const double depth = curvature * (1 - std::cos(0.5 * probe_width / curvature));

  for (uint32_t i = 0; i < nb_elements; ++i) {
    const double alpha = (-0.5 * probe_width + i * pitch) / curvature;
    const double x = curvature * std::sin(alpha);
    const double y = curvature * (std::cos(alpha) - 1) + depth;

    Element el;
    el.transform.translation = {x, 0, y};
    el.transform.rotation = {0, alpha, 0};
    curvi.elements[i] = el;
  }
}

void updateRcaElementsRectGeometry(Probe& probe, const Vector2D<uint32_t>& nb_elements,
                                   const Vector2D<double>& col_size,
                                   const Vector2D<double>& row_size) {
  auto& elements = probe.elements;
  const size_t n_els = elements.size();

  if (nb_elements.x + nb_elements.y != n_els) {
    throw std::runtime_error(
        "updateRcaElementsRectGeometry(): nb_elements.x + nb_elements.y != "
        "probe.elements.size()");
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

void updateCurvilinearElementsRectGeometry(Probe& probe, const Vector2D<double>& size) {
  updateLinearElementsRectGeometry(probe, size);
}

Probe createStandardRca(uint32_t n_x, uint32_t n_y, double pitch_x, double pitch_y,
                        double element_width_x, double element_width_y, double element_height_x,
                        double element_height_y, double delta_z) {
  Probe rca;
  rca.type = urx::ProbeType::RCA;
  rca.description = "Standard RCA probe with " + std::to_string(n_x) + " row elements and " +
                    std::to_string(n_y) + " column elements";

  auto eg_0 = std::make_shared<ElementGeometry>(ElementGeometry{
      std::vector<Vector3D<double>>{{-element_width_x / 2, -element_height_x / 2, 0},
                                    {element_width_x / 2, -element_height_x / 2, 0},
                                    {element_width_x / 2, element_height_x / 2, 0},
                                    {-element_width_x / 2, element_height_x / 2, 0}},
  });

  auto eg_1 = std::make_shared<ElementGeometry>(ElementGeometry{
      std::vector<Vector3D<double>>{{-element_width_y / 2, -element_height_y / 2, 0},
                                    {element_width_y / 2, -element_height_y / 2, 0},
                                    {element_width_y / 2, element_height_y / 2, 0},
                                    {-element_width_y / 2, element_height_y / 2, 0}},
  });
  rca.element_geometries = {eg_0, eg_1};
  rca.impulse_responses = {std::make_shared<ImpulseResponse>()};

  const double x_min = -pitch_x * (n_x - 1.) / 2.0;
  for (uint32_t i = 0; i < n_x; ++i) {
    const double x = x_min + pitch_x * i;
    rca.elements.push_back(Element{Transform{{0, 0, 0}, {x, 0, -delta_z / 2}},
                                   std::weak_ptr<ElementGeometry>(),
                                   std::weak_ptr<ImpulseResponse>()});
    rca.elements.back().element_geometry = rca.element_geometries.front();
    rca.elements.back().impulse_response = rca.impulse_responses.back();
  }

  const double y_min = -pitch_y * (n_y - 1.) / 2.0;
  for (uint32_t i = 0; i < n_y; ++i) {
    double y = y_min + pitch_y * i;
    rca.elements.push_back(Element{Transform{{0, 0, 0}, {0, y, delta_z / 2}},
                                   std::weak_ptr<ElementGeometry>(),
                                   std::weak_ptr<ImpulseResponse>()});
    rca.elements.back().element_geometry = rca.element_geometries.back();
    rca.elements.back().impulse_response = rca.impulse_responses.back();
  }

  return rca;
}

Probe createStandardLinear(uint32_t n, double pitch, double element_width, double element_height) {
  Probe la;
  la.type = urx::ProbeType::LINEAR;
  la.description = "Standard LA probe with " + std::to_string(n) + " elements";

  auto eg = std::make_shared<ElementGeometry>(ElementGeometry{
      std::vector<Vector3D<double>>{{-element_width / 2, -element_height / 2, 0},
                                    {element_width / 2, -element_height / 2, 0},
                                    {element_width / 2, element_height / 2, 0},
                                    {-element_width / 2, element_height / 2, 0}},
  });

  la.element_geometries = {eg};
  la.impulse_responses = {std::make_shared<ImpulseResponse>()};

  const double x_min = -pitch * (n - 1.) / 2.0;
  for (uint32_t i = 0; i < n; ++i) {
    const double x = x_min + pitch * i;
    la.elements.push_back(Element{Transform{{0, 0, 0}, {x, 0, 0}}, std::weak_ptr<ElementGeometry>(),
                                  std::weak_ptr<ImpulseResponse>()});
    la.elements.back().element_geometry = la.element_geometries.front();
    la.elements.back().impulse_response = la.impulse_responses.back();
  }

  return la;
}

Probe createStandardCurviLinear(uint32_t n, double pitch, double element_width,
                                double element_height, double curvature) {
  Probe cla;
  cla.type = urx::ProbeType::CURVILINEAR;
  cla.description = "Standard curvi linear probe with " + std::to_string(n) + " elements";

  auto eg = std::make_shared<ElementGeometry>(ElementGeometry{
      std::vector<Vector3D<double>>{{-element_width / 2, -element_height / 2, 0},
                                    {element_width / 2, -element_height / 2, 0},
                                    {element_width / 2, element_height / 2, 0},
                                    {-element_width / 2, element_height / 2, 0}},
  });

  cla.element_geometries = {eg};
  cla.impulse_responses = {std::make_shared<ImpulseResponse>()};
  ////////////////////////////////////////////////////////////////////////////////////////
  ///
  /// Formulas used to create curvilinear probe element transforms:
  ///
  /// alpha = (-pw/2 + i * p)/R, with  alpha : angular position on the probe
  ///                                     pw : probe width, length of the circular arc between leftmost and rightmost elements
  ///                                      p : pitch
  ///                                      R : curvature
  ///
  /// x = R sin(a), with     x : abscissa of the element
  ///                    alpha : angular position on the probe
  ///                        R : curvature
  ///
  /// y = R (cos(a) - 1) + depth, with         y : ordinate of the element
  ///                                      alpha : angular position on the probe
  ///                                          R : curvature
  ///                                      depth : depth of the probe, distance between the lowest and highest element on y-axis
  ///

  const double probe_width = pitch * (n - 1);
  const double depth = curvature * (1 - std::cos(0.5 * probe_width / curvature));

  for (uint32_t i = 0; i < n; ++i) {
    const double alpha = (-0.5 * probe_width + i * pitch) / curvature;
    const double x = curvature * std::sin(alpha);
    const double y = curvature * (std::cos(alpha) - 1) + depth;
    cla.elements.push_back(
        Element{Transform{Vector3D<double>{0, alpha, 0}, Vector3D<double>{x, 0, y}},
                std::weak_ptr<ElementGeometry>(), std::weak_ptr<ImpulseResponse>()});
    cla.elements.back().element_geometry = cla.element_geometries.back();
    cla.elements.back().impulse_response = cla.impulse_responses.back();
  }
  return cla;
}

Probe createStandardMatrix(uint32_t n_x, uint32_t n_y, double pitch_x, double pitch_y,
                           double element_width, double element_height) {
  Probe ma;
  ma.type = urx::ProbeType::MATRIX;
  ma.description = "Standard Matrix array probe with " + std::to_string(n_x) + " row elements x " +
                   std::to_string(n_y) + " column elements";

  auto eg = std::make_shared<ElementGeometry>(ElementGeometry{
      std::vector<Vector3D<double>>{{-element_width / 2, -element_height / 2, 0},
                                    {element_width / 2, -element_height / 2, 0},
                                    {element_width / 2, element_height / 2, 0},
                                    {-element_width / 2, element_height / 2, 0}},
  });
  ma.element_geometries = {eg};
  ma.impulse_responses = {std::make_shared<ImpulseResponse>()};

  const double x_min = -pitch_x * (n_x - 1.) / 2.0;
  const double y_min = -pitch_y * (n_y - 1.) / 2.0;
  for (uint32_t i = 0; i < n_x; ++i) {
    const double x = x_min + pitch_x * i;
    for (uint32_t j = 0; j < n_y; ++j) {
      const double y = y_min + pitch_y * j;
      ma.elements.push_back(Element{Transform{{0, 0, 0}, {x, y, 0}},
                                    std::weak_ptr<ElementGeometry>(),
                                    std::weak_ptr<ImpulseResponse>()});
      ma.elements.back().element_geometry = ma.element_geometries.back();
      ma.elements.back().impulse_response = ma.impulse_responses.back();
    }
  }

  return ma;
}

}  // namespace urx::utils::probe_helper
