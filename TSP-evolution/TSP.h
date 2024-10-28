#pragma once

#include "Location.h"
#include "utils.h"

#include <limits>

using Path = std::vector<int>;

class TSP {

public:
	TSP(int populationSize, std::vector<Location>locations, int iterations, int mutationParam);
	double compareToOpt(const Path& permutation);

private:
	std::vector<Location> locations;
	std::vector<Path> population;
	Path bestPath;
	double bestValue = std::numeric_limits<double>::max();
	int mutationParam;

	std::vector<Path> generatePopulation(
		int populationSize,
		const std::vector<Location>& locations,
		std::random_device& rd);
	Path createPmxOffspring(
		Path parent1, Path parent2,
		int crosspoint1, int crosspoint2);
	Path selectParentTournament(const std::vector<Path>& population, int q, std::random_device& rd);
	void scrambleMutation(Path& individual, const int k, std::random_device& rd);
	std::pair<Path, Path> pmxCrossover(const Path& parent1, const Path& parent2,
		int crosspoint1, int crosspoint2, std::random_device& rd);
	double calcIndividualValue(const Path& individual);
};