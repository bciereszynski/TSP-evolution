#pragma once

#include "Location.h"
#include "utils.h"

#include <limits>
#include <map>
#include <utility>

enum CrossoverMethod {
	Ox,
	Pmx
};

using Path = std::vector<int>;

class TSP {

public:
	TSP(std::vector<Location>locations, int populationSize = 10, int iterations = 10, int mutationParam = 3, int selectionParam = 5, bool verbose = false);
	double compareToOpt(const Path& permutation);
	Path bestPath;
	std::pair<Path, Path> crossover(const Path& parent1, const Path& parent2, int crosspoint1, int crosspoint2, CrossoverMethod method);

private:
	std::map<std::pair<int, int>, double> distances;
	std::vector<Path> population;

	double bestValue = std::numeric_limits<double>::max();
	int mutationParam;
	int selectionParam;

	std::random_device rd;

	void generatePopulation(int populationSize, std::vector<Location>locations);
	Path createPmxOffspring(
		Path parent1, Path parent2,
		int crosspoint1, int crosspoint2);
	Path createOxOffspring(
		Path parent1, Path parent2,
		int crosspoint1, int crosspoint2);
	Path selectParentTournament(std::vector<double> values);
	void scrambleMutation(Path& individual);
	double calcIndividualValue(const Path& individual);
};