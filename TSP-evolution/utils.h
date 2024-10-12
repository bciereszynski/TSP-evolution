#pragma once

#include <vector>
#include <string>

#include "Location.h"

namespace utils {

	double calcEuclDistance(const Location& location1, const Location& location2);

	std::vector<Location> loadTSPLIB(const std::string& filename);

	void printLocationsAndDistances(const std::vector<Location>& locations);

	void printPopulation(const std::vector<std::vector<int>>& population);
}  // namespace utils
