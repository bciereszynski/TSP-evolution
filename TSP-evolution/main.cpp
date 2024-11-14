#include "TSP.h"
#include <iostream>

#include "utils.h"

int main() {
	const std::string filename = "data.tsp";
	const int populationSize = 100;
	const int iterations = 100;
	const int k = 3;
	const int q = 5;

	std::vector<Location> locations = utils::loadTSPLIB(filename);

	if (locations.empty()){
		std::cerr << "ERROR: data not loaded" << std::endl;
		return 1;
	}

	TSP tsp(populationSize, locations, iterations, k, q);

	std::vector<int> optPath = utils::loadoptTSPLIB("opt.tsp");

	for (int i = 0; i < tsp.bestPath.size(); i++) {
		std::cout << tsp.bestPath[i] << std::endl;
	}
	std::cout << tsp.compareToOpt(optPath) << std::endl;



	Path test2Opt;
	for (int i = 0; i < locations.size(); ++i) {
		test2Opt.push_back(locations[i].id);
	}
	std::random_device rd;
	std::default_random_engine rng(rd());

	for (int i = 0; i < populationSize; ++i) {
		std::shuffle(test2Opt.begin(), test2Opt.end(), rng);
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