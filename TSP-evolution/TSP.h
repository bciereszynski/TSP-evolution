#pragma once

#include "Location.h"
#include "utils.h"

class TSP {

public:
	std::vector<std::vector<int>> population;
	TSP(int populationSize, std::vector<Location>locations);
	std::pair<std::vector<int>, std::vector<int>> pmxCrossover(
			const std::vector<int>& parent1, const std::vector<int>& parent2,
			int crosspoint1, int crosspoint2,
			std::random_device& rd);

private:
	std::vector<std::vector<int>> generatePopulation(
		int populationSize,
		const std::vector<Location>& locations,
		std::random_device& rd);
	std::vector<int> createPmxOffspring(
		std::vector<int> parent1, std::vector<int> parent2,
		int crosspoint1, int crosspoint2);
	void scrambleMutation(std::vector<int>& individual, const int k, std::random_device& rd);
};