#include "TSP.h"
#include <iostream>

int main() {
	const std::string filename = "data.tsp";
	const int populationSize = 10;
	const int k = 3;

	std::vector<Location> locations = utils::loadTSPLIB(filename);

	if (!locations.empty()) {
		utils::printLocationsAndDistances(locations);
	}
	else {
		std::cerr << "ERROR: data not loaded" << std::endl;
		return 1;
	}

	TSP tsp(populationSize, locations);

	utils::printPopulation(tsp.population);

	return 0;
}