#include "../include/date.h"
#include "../include/json.h"
#include "../include/keplerianApprox.h"
#include "../include/planet.h"
#include "../include/util.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


// requests and returns text input from user
std::string getString(const std::string &prompt) {
  std::string input = "";
  std::cout << prompt;
  getline(std::cin, input);

  while (input == "") {
    std::cout << "No characters entered, try again." << std::endl;
    std::cout << prompt;
    getline(std::cin, input);
  }

  return input;
}


// requests date input from user and returns days since J200 epoch
double getDate() {

  bool isFormatted = false;
  Date date;

  do {
    int deliminatorCount = 0;
    std::string numAsString = "0";

    const std::string dateAsString = getString("Enter a date (MM/DD/YYYY): ");

    for (const char character : dateAsString) {
      if (character == '/') {
        deliminatorCount += 1;

        if (deliminatorCount == 1) {
          date.month = stoi(numAsString);
          numAsString = "0";
        } else if (deliminatorCount == 2) {
          date.day = stoi(numAsString);
          numAsString = "0";
        }
      } else {
        numAsString += character;
      }
    }

    date.year = stoi(numAsString);

    if (deliminatorCount == 2 && date.month > 0 && date.month <= 12 &&
        date.day > 0 && date.day <= 31) {
      isFormatted = true;
    } else {
      std::cout << "Date formatted incorrectly, try again" << std::endl;
    }
  } while (!isFormatted);

  return date.calcDaysSinceEpoch();
}


// displays formatted results
void printResults(const std::vector<StateVector> &planets) {
  const Coord earthPos = planets.at(2).pos;
  const Coord sunPos = planets.at(planets.size() - 1).pos;

  for (StateVector p : planets) {
    // if (p.name == "sun")
    //   continue;
    std::cout << "----------------------------------\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(27) << "Name: " << p.name << "\n";
    std::cout << std::setw(27) << "Distance from Sun [AU]: ";
    std::cout << sqrt(p.pos.magSquared(sunPos)) / M_PER_AU << std::endl;
    std::cout << std::setw(27) << "Distance from Earth [AU]: ";
    std::cout << sqrt(p.pos.magSquared(earthPos)) / M_PER_AU << std::endl;
    std::cout << std::setw(27) << "Vel [km/sec]: ";
    std::cout << sqrt(p.vel.magSquared(Coord())) / M_PER_KM << std::endl;
  }
}


// gets answers from solutions.json and display formatted comparison
void printTest(const std::vector<StateVector> &bodies,
               const double daysSinceEpoch) {
  std::vector<StateVector> solutionBodies;
  populateSolutions(solutionBodies, daysSinceEpoch);

  std::cout << "ERROR %\n\n";
  StateVector sun = bodies.at(bodies.size() - 1);
  for (size_t i = 0; i < bodies.size() - 1; i++) {
    std::cout << std::setw(7) << "NAME: " << bodies[i].name << '\n';
    double posObserved = bodies[i].pos.magSquared(sun.pos);
    double posExpected = solutionBodies[i].pos.magSquared(sun.pos);
    double velObserved = bodies[i].vel.magSquared(Coord());
    double velExpected = solutionBodies[i].vel.magSquared(Coord());

    double posError =
        std::abs((posObserved - posExpected) / posExpected * 100.0);
    double velError =
        std::abs((velObserved - velExpected) / velExpected * 100.0);

    std::cout << std::setw(7) << "POS: " << posError << '\n';
    std::cout << std::setw(7) << "VEL: " << velError << "\n\n";
  }
}
