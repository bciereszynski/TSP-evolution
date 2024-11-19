#include "TSP.h"
#include <iostream>

#include "utils.h"
#include "RandomGenerator.h"

int main() {
	const std::string dataFilename = "data.tsp";
	const std::string optFilename = "opt.tsp";
	const int populationSize = 100;
	const int iterationsWithoutImprovement = 100;
	const bool verbose = true;
	const int mutationParam = 3;
	const int selectionParam = 5;

	std::vector<Location> locations = utils::loadTSPLIB(dataFilename);

	if (locations.empty()){
		std::cerr << "ERROR: data not loaded" << std::endl;
		return 1;
	}

	TSP tsp(locations, populationSize, iterationsWithoutImprovement, mutationParam, selectionParam, verbose);

	std::vector<int> optPath = utils::loadoptTSPLIB(optFilename);
	
	if (verbose)
		for (int i = 0; i < tsp.bestPath.size(); i++) {
			std::cout << tsp.bestPath[i] << std::endl;
		}
	tsp.compareToOpt(optPath);



	Path test2Opt;
	for (int i = 0; i < locations.size(); ++i) {
		test2Opt.push_back(locations[i].id);
	}

	for (int i = 0; i < populationSize; ++i) {
		std::shuffle(test2Opt.begin(), test2Opt.end(), RandomGenerator());
	}
	auto distances = utils::calculateDistances(locations);
	while (utils::twoOpt(test2Opt, distances, utils::BestImprovement)) {}
	double distance2Opt = 0.0;
	for (int i = 0; i < test2Opt.size(); i++) {
		distance2Opt += distances[{test2Opt[i], test2Opt[(i + 1) % test2Opt.size()]}];

	}

	std::cout << "2Opt value:" <<  distance2Opt << std::endl;

	return 0;
}