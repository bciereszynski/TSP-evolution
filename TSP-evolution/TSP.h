#pragma once

#include "Location.h"
#include "utils.h"

#include <limits>

using Path = std::vector<int>;

class TSP {

public:
	TSP(int populationSize, std::vector<Location>locations, int iterations, int mutationParam, int selectionParam);
	double compareToOpt(const Path& permutation);

private:
	std::vector<Location> locations;
	std::vector<Path> population;
	Path bestPath;
	double bestValue = std::numeric_limits<double>::max();
	int mutationParam;
	int selectionParam;

	std::random_device rd;

	void generatePopulation(int populationSize);
	Path createPmxOffspring(
		Path parent1, Path parent2,
		int crosspoint1, int crosspoint2);
	Path selectParentTournament();
	void scrambleMutation(Path& individual);
	std::pair<Path, Path> pmxCrossover(const Path& parent1, const Path& parent2,
		int crosspoint1, int crosspoint2);
	double calcIndividualValue(const Path& individual);
};