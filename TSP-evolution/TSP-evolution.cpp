#include "TSP.h"

#include <algorithm> 
#include <random>    
#include <iostream>
#include <unordered_map>


TSP::TSP(int populationSize, std::vector<Location>locations, int iterations, int mutationParam) {
	std::random_device rd;

	this->locations = locations;
	this->mutationParam = mutationParam;

	population =
		generatePopulation(populationSize, locations, rd);

	for (int generation = 0; generation < iterations; generation++) {
		std::vector<double> distances(population.size());

		double max = 0.0;
		for (int i = 0; i < population.size(); i++) {
			distances[i] = calcIndividualFitness(population[i]);
			max = std::max(max, distances[i]);
		}

		double sum = 0.0;
		for (int i = 0; i < population.size(); i++) {
			distances[i] = max - distances[i];
			sum = sum + distances[i];
		}

		std::vector<double> probabilities(population.size());
		for (int i = 0; i < population.size(); i++) {
			auto probability = distances[i] / sum;
			probabilities[i] = (i == 0) ? probability : probability + probabilities[i - 1];
		}

		std::vector<std::vector<int>> newPopulation;
		for (int i = 0; i < population.size(); i += 2) {
			std::vector<int> parent1 = selectParent(probabilities, rd);
			std::vector<int> parent2 = selectParent(probabilities, rd);

			std::vector<int> crossoverPoints = utils::getRandomUniquePositions(parent1.size(), 2, rd);

			auto offspringPair = pmxCrossover(parent1, parent2, crossoverPoints[0], crossoverPoints[1], rd);
			newPopulation.push_back(offspringPair.first);
			newPopulation.push_back(offspringPair.second);
		}

		for (auto& individual : newPopulation) {
			if (std::uniform_real_distribution<double>(0.0, 1.0)(rd) < 0.03) {
				scrambleMutation(individual, mutationParam, rd);  // k = 3, przykład
			}
		}
		population = newPopulation;

		auto minimum = -1;
		for (int i = 0; i < population.size(); i++) {
			distances[i] = calcIndividualFitness(population[i]);
			if (minimum == -1 || distances[i] < minimum) {
				minimum = distances[i];
			}
		}
		minimumDistance = std::min(minimum, minimumDistance);
	}
}

std::vector<int> TSP::selectParent(const std::vector<double>& probabilities, std::random_device& rd) {
	std::default_random_engine rng(rd());
	std::uniform_real_distribution<double> distribution(0.0, 1.0);
	double value = distribution(rng);

	auto it = std::lower_bound(probabilities.begin(), probabilities.end(), value);
	if (it != probabilities.end()) {
		return population[std::distance(probabilities.begin(), it)];
	}
	else {
		throw std::runtime_error("ERROR: lower_bound failed");
	}
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

double TSP::calcIndividualFitness(const std::vector<int>& individual) {
	double result = 0.0;
	for (int i = 1; i < individual.size(); i++) {
		result += utils::calcEuclDistance(locations[individual[i] - 1], locations[individual[i - 1] - 1]);
	}
	return result;
}

double TSP::compareToOpt(const std::vector<int>& permutation) {
	double result = calcIndividualFitness(permutation);

	return result - minimumDistance;
}
