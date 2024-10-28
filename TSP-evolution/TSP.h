#pragma once

#include "Location.h"
#include "utils.h"

#include <limits>
#include <map>
#include <utility>

using Path = std::vector<int>;

class TSP {

public:
	TSP(int populationSize, std::vector<Location>locations, int iterations, int mutationParam, int selectionParam);
	double compareToOpt(const Path& permutation);
	Path bestPath;

private:
	std::map<std::pair<int, int>, double> distances;
	std::vector<Path> population;

	double bestValue = std::numeric_limits<double>::max();
	int mutationParam;
	int selectionParam;

	std::random_device rd;

	void calculateDistances(std::vector<Location>locations);
	void generatePopulation(int populationSize, std::vector<Location>locations);
	Path createPmxOffspring(
		Path parent1, Path parent2,
		int crosspoint1, int crosspoint2);
	Path selectParentTournament();
	void scrambleMutation(Path& individual);
	std::pair<Path, Path> pmxCrossover(const Path& parent1, const Path& parent2,
		int crosspoint1, int crosspoint2);
	double calcIndividualValue(const Path& individual);
};