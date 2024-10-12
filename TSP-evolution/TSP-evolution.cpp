#include <iostream>

#include "Location.h"
#include "utils.h"

#include <algorithm> 
#include <random>    

std::vector<std::vector<int>> generatePopulation(
	int populationSize,
	const std::vector<Location>& locations,
	std::random_device& rd) {
	std::vector<std::vector<int>> population;
	std::vector<int> locationsIDs;

	for (int i = 0; i < locations.size(); ++i) {
		locationsIDs.push_back(locations[i].id);
	}

	std::default_random_engine rng(rd());

	for (int i = 0; i < populationSize; ++i) {
		std::shuffle(locationsIDs.begin(), locationsIDs.end(), rng);
		population.push_back(locationsIDs);
	}

	return population;
}

void scrambleMutation(std::vector<int>& individual, const int k, std::random_device& rd) {
	if (individual.size() < k) {
        throw std::runtime_error("ERROR: individual size is less than mutation parameter k");
	}
	std::vector<int> randomIndexes = utils::getRandomUniquePositions(
		individual.size(), k, rd);

	std::vector<int> choosenLocations;
	for (int i = 0; i < randomIndexes.size(); i++) {
		choosenLocations.push_back(individual[randomIndexes[i]]);
	}
	std::default_random_engine rng(rd());
	std::shuffle(choosenLocations.begin(), choosenLocations.end(), rng);

	for (int i = 0; i < randomIndexes.size(); i++) {
		individual[randomIndexes[i]] = choosenLocations[i];
	}
}

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

	std::random_device rd;

	std::vector<std::vector<int>> population =
		generatePopulation(populationSize, locations, rd);

	utils::printPopulation(population);

	return 0;
}