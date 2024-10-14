#include "TSP.h"
#include <iostream>

#include "utils.h"

int main() {
	const std::string filename = "data.tsp";
	const int populationSize = 100;
	const int iterations = 1000;
	const int k = 3;

	std::vector<Location> locations = utils::loadTSPLIB(filename);

	if (locations.empty()){
		std::cerr << "ERROR: data not loaded" << std::endl;
		return 1;
	}

	TSP tsp(populationSize, locations, iterations);

	std::cout << "Minimum distance: " << tsp.minimumDistance << std::endl;

	std::vector<int> optPath = utils::loadoptTSPLIB("opt.tsp");

	std::cout << tsp.compareToOpt(optPath) << std::endl;

	return 0;
}