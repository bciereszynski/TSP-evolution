#pragma once

#include <map>
#include <vector>
#include <string>
#include <random>

#include "Location.h"

namespace utils {

	enum OptMethod {
		FirstImprovement,
		BestImprovement
	};

	double calcEuclDistance(const Location& location1, const Location& location2);

	std::vector<Location> loadTSPLIB(const std::string& filename);
	std::vector<int> loadoptTSPLIB(const std::string& filename);

	std::vector<int> getRandomUniquePositions(const int n, const int k);

	void printLocationsAndDistances(const std::vector<Location>& locations);

	void printPopulation(const std::vector<std::vector<int>>& population);

	std::map<std::pair<int, int>, double> calculateDistances(const std::vector<Location>locations);

	bool twoOpt(std::vector<int>& individual, std::map<std::pair<int, int>, double> distances, OptMethod method = FirstImprovement);
}  // namespace utils
