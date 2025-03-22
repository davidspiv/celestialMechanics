#include <cmath>
#include <vector>

#include "../include/celestialBody.h"
#include "../include/coord.h"
#include "../include/getInitialPlanetState.h"
#include "../include/io.h"
#include "../include/picture.h"
#include "../include/timer.h"
#include "../include/updatePlanetState.h"
#include "../include/util.h"


int main() {
  //   const double julianDay = getDate();
  const double julianDay = 0;
  //   Timer timer;
  //   const int dt = 604800; // 1-week intervals
  //   const int steps = round(SEC_PER_DAY * julianDay / double(dt));

  const int picSideLength = 500;
  Picture pic(picSideLength, picSideLength, 0, 0, 0);

  // parallel vectors to represent planets
  std::vector<OrbitalElements> elements;
  std::vector<CelestialBody> bodies;

  //   populatePlanets(elements, bodies);
  populateSolutions(bodies);
  bodies.push_back({"Sun", Coord(), Coord(), M_SUN});

  //   const size_t systemSize = approxSystemSize(elements);


  //   for (size_t i = 0; i < bodies.size() - 1; i++) {
  //     populateStateVectors(elements[i], bodies[i], julianDay);
  //   }


  //   for (int i = 0; i < steps; i++) {
  //     updateBodies(bodies, dt);

  //     // end up rendering a lot of the same pixels, but checking is
  //     slower? drawBodies(bodies, pic, systemSize);
  //   }

  drawBodies(bodies, pic, 31, true);

  pic.save("result.png");

  //   printTest(bodies);
  printResults(bodies);
}
