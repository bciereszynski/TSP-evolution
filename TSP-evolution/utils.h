#pragma once

#include <map>
#include <vector>
#include <string>
#include <random>

#include "Location.h"

namespace utils {

	double calcEuclDistance(const Location& location1, const Location& location2);

	std::vector<Location> loadTSPLIB(const std::string& filename);
	std::vector<int> loadoptTSPLIB(const std::string& filename);

	std::vector<int> getRandomUniquePositions(const int n, const int k, std::random_device& rd);

	void printLocationsAndDistances(const std::vector<Location>& locations);

	void printPopulation(const std::vector<std::vector<int>>& population);

	std::map<std::pair<int, int>, double> calculateDistances(const std::vector<Location>locations);

	void twoOptFI(std::vector<int>& individual, std::map<std::pair<int, int>, double> distances);
}  // namespace utils
