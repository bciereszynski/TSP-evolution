#include "TSP.h"

#include <algorithm> 
#include <cassert>
#include <random>    
#include <iostream>
#include <unordered_map>
#include <unordered_set>

TSP::TSP(int populationSize, std::vector<Location>locations, int iterations, int mutationParam) {
	std::random_device rd;

	this->locations = locations;
	this->mutationParam = mutationParam;

	population =
		generatePopulation(populationSize, locations, rd);
	int iterationsWithoutImprovement = 0;

	for (int generation = 0; generation < iterations; generation++) {

		bool improvement = false;

		for (int i = 0; i < population.size(); i++) {
			auto value = calcIndividualValue(population[i]);
			if (value < bestValue) {
				bestValue = value;
				bestPath = population[i];
				improvement = true;
			}
		}

		if (!improvement) {
			iterationsWithoutImprovement++;
			if (iterationsWithoutImprovement == 1000) {
				break;
			}
		}
		else {
			iterationsWithoutImprovement = 0;
		}

		std::vector<std::vector<int>> newPopulation;
		for (int i = 0; i < population.size(); i += 2) {
			std::vector<int> parent1 = selectParentTournament(population, 5, rd);
			std::vector<int> parent2 = selectParentTournament(population, 5, rd);

			std::vector<int> crossoverPoints = utils::getRandomUniquePositions(parent1.size(), 2, rd);

			auto offspringPair = pmxCrossover(parent1, parent2, crossoverPoints[0], crossoverPoints[1], rd);
			newPopulation.push_back(offspringPair.first);
			newPopulation.push_back(offspringPair.second);
		}

		for (auto& individual : newPopulation) {
			if (std::uniform_real_distribution<double>(0.0, 1.0)(rd) < 0.03) {
				scrambleMutation(individual, mutationParam, rd);
			}
		}
		population = newPopulation;
	}

	for (int i = 0; i < population.size(); i++) {
		auto value = calcIndividualValue(population[i]);
		if (value < bestValue) {
			bestValue = value;
			bestPath = population[i];
		}
	}
}

std::vector<int> TSP::selectParentTournament(const std::vector<std::vector<int>>& population, int q, std::random_device& rd) {
	std::uniform_int_distribution<int> dist(0, population.size() - 1);
	double bestValue = std::numeric_limits<double>::max();
	std::vector<int> bestIndividual;

	std::unordered_set<int> selectedIndices;

	for (int i = 0; i < q; ++i) {
		int randomIndex = dist(rd);
		while (selectedIndices.find(randomIndex) != selectedIndices.end()) {
			randomIndex = (randomIndex + 1) % population.size();
		}

		selectedIndices.insert(randomIndex);

		const auto& candidate = population[randomIndex];
		double candidateValue = calcIndividualValue(candidate);

		if (candidateValue < bestValue) {
			bestValue = candidateValue;
			bestIndividual = candidate;
		}
	}

	return bestIndividual;
}

std::vector<std::vector<int>> TSP::generatePopulation(
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

void TSP::scrambleMutation(std::vector<int>& individual, const int k, std::random_device& rd) {
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

std::pair<std::vector<int>, std::vector<int>> TSP::pmxCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2, int crosspoint1, int crosspoint2, std::random_device& rd) {
	if (crosspoint1 > crosspoint2) {
		std::swap(crosspoint1, crosspoint2);
	}
	return { createPmxOffspring(parent2, parent1, crosspoint1, crosspoint2),
			 createPmxOffspring(parent1, parent2, crosspoint1, crosspoint2) };
}

std::vector<int> TSP::createPmxOffspring(std::vector<int> parent1, std::vector<int> parent2, int crosspoint1, int crosspoint2) {
	int n = parent1.size();
	std::vector<int> offspring(n, -1);

	for (int i = crosspoint1; i <= crosspoint2; ++i) {
		offspring[i] = parent1[i];
	}

	// create mapping for values that was copied from parent1
	std::unordered_map<int, int> mapping;
	for (int i = crosspoint1; i <= crosspoint2; ++i) {
		mapping[parent1[i]] = parent2[i];
	}

	for (int i = 0; i < n; ++i) {
		if (i >= crosspoint1 && i <= crosspoint2) {
			continue;
		}
		int candidate_value = parent2[i];

		while (std::find(offspring.begin() + crosspoint1,
			offspring.begin() + crosspoint2 + 1, candidate_value) != offspring.begin() + crosspoint2 + 1) {
			candidate_value = mapping[candidate_value];	// while to handle situation when mapping value is also in offspring e.g. 1->2, 2->3
		}

		offspring[i] = candidate_value;
	}

	return offspring;
};

double TSP::calcIndividualValue(const std::vector<int>& individual) {
	double result = 0.0;
	for (int i = 1; i < individual.size(); i++) {
		result += utils::calcEuclDistance(locations[individual[i] - 1], locations[individual[i - 1] - 1]);
	}
	return result;
}

double TSP::compareToOpt(const std::vector<int>& permutation) {
	double optResult = calcIndividualValue(permutation);
	double locatResult = calcIndividualValue(bestPath);

	std::cout << "Optimal path: " << optResult << std::endl;
	std::cout << "Out best path: " << locatResult << std::endl;

	return optResult - locatResult;
}
