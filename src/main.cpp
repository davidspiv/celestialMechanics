#include <cmath>
#include <vector>

#include "../include/helpers.h"
#include "../include/io.h"
#include "../include/json.h"
#include "../include/keplerianApprox.h"
#include "../include/nBodySim.h"
#include "../include/picture.h"
#include "../include/planet.h"
#include "../include/timer.h"
#include "../include/util.h"


// N-body model of Jovian planets
void handleJovianBodies(const std::vector<OrbitalElements> &elements,
                        std::vector<CelestialBody> &bodies, double julianDay,
                        const size_t planetDivideIndex);


// One-body approximation for Terrestrial planets
void handleTerrestrialBodies(const std::vector<OrbitalElements> &elements,
                             std::vector<CelestialBody> &bodies,
                             const double julianDay,
                             const size_t planetDivideIndex);


int main() {
  // Parallel vectors to represent planets
  std::vector<OrbitalElements> elements;
  std::vector<CelestialBody> bodies;

  // Initialize system
  const size_t planetDivideIndex = 4;
  static CelestialBody sun = {"sun", Coord(), Coord(), M_SUN};
  populatePlanets(elements, bodies);
  bodies.emplace_back(sun);

  const double julianDay = getDate();

  // Update planets to target date
  handleJovianBodies(elements, bodies, julianDay, planetDivideIndex);
  handleTerrestrialBodies(elements, bodies, julianDay, planetDivideIndex);

  // Handle PNG output
  const size_t picSize = 500;
  const size_t systemSizeAU = approxSystemSize(elements);
  Picture pic(picSize, picSize, 0, 0, 0);
  drawBodies(bodies, pic, systemSizeAU, true);
  pic.save("result.png");

  // Output formatted results
  printTest(bodies, julianDay);
  //   printResults(bodies);
}


// N-body model of Jovian planets
void handleJovianBodies(const std::vector<OrbitalElements> &elements,
                        std::vector<CelestialBody> &bodies, double julianDay,
                        const size_t planetDivideIndex) {

  // Use properties of Keplerian orbit to compute initial state vectors
  for (size_t i = planetDivideIndex; i < bodies.size() - 1; i++) {
    populateStateVectors(elements[i], bodies[i], 0);
  }

  // Isolate bodies to include in model
  std::vector<CelestialBody> jovianBodies(bodies.begin() + planetDivideIndex,
                                          bodies.end());

  // Numerically integrate, using each step to update planet
  const int dt = (julianDay < 0 ? -1 : 1) * SEC_PER_DAY * 7; // one week
  const int steps = round(SEC_PER_DAY * abs(julianDay) / double(abs(dt)));
  for (int i = 0; i < steps; i++) {
    updateBodies(jovianBodies, dt);
  }

  // Merge back updated bodies
  for (size_t i = 0; i < jovianBodies.size(); i++) {
    bodies[i + planetDivideIndex] = jovianBodies[i];
  }
};


// One-body approximation for Terrestrial planets
void handleTerrestrialBodies(const std::vector<OrbitalElements> &elements,
                             std::vector<CelestialBody> &bodies,
                             const double julianDay,
                             const size_t planetDivideIndex) {

  for (size_t i = 0; i < planetDivideIndex; i++) {
    populateStateVectors(elements[i], bodies[i], julianDay);
  }
};
