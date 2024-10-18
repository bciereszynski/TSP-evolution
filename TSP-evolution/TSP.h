#pragma once

#include "Location.h"
#include "utils.h"

class TSP {

public:
	TSP(int populationSize, std::vector<Location>locations, int iterations, int mutationParam);
	double compareToOpt(const std::vector<int>& permutation);

private:
	std::vector<Location> locations;
	std::vector<std::vector<int>> population;
	std::vector<int> bestPath;
	double maksimumFitness = 0.0;
	int mutationParam;

	std::vector<std::vector<int>> generatePopulation(
		int populationSize,
		const std::vector<Location>& locations,
		std::random_device& rd);
	std::vector<int> createPmxOffspring(
		std::vector<int> parent1, std::vector<int> parent2,
		int crosspoint1, int crosspoint2);
	std::vector<int> selectParent(const std::vector<double>& probabilities, std::random_device& rd);
	void scrambleMutation(std::vector<int>& individual, const int k, std::random_device& rd);
	std::pair<std::vector<int>, std::vector<int>> pmxCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2,
		int crosspoint1, int crosspoint2, std::random_device& rd);
	double calcIndividualFitness(const std::vector<int>& individual);
	double calcIndividualValue(const std::vector<int>& individual);
};