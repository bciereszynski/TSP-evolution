#include "TSP.h"

#include <algorithm> 
#include <cassert>
#include <random>    
#include <iostream>
#include <unordered_map>

TSP::TSP(int populationSize, std::vector<Location>locations, int iterations, int mutationParam, int selectionParam) {
	this->mutationParam = mutationParam;
	this->selectionParam = selectionParam;

	generatePopulation(populationSize, locations);
	calculateDistances(locations);
	int iterationsWithoutImprovement = 0;
	std::vector<double> values(population.size());

	while (true) {

		bool improvement = false;

		for (int i = 0; i < population.size(); i++) {
			values[i] = calcIndividualValue(population[i]);
			if (values[i] < bestValue) {
				bestValue = values[i];
				bestPath = population[i];
				std::cout << "curr best: " << bestValue << " - iteration without improvement: " << iterationsWithoutImprovement << std::endl;
				improvement = true;
			}
		}

		if (!improvement) {
			iterationsWithoutImprovement++;
			if (iterationsWithoutImprovement == iterations) {
				break;
			}
		}
		else {
			iterationsWithoutImprovement = 0;
		}

		std::vector<Path> newPopulation;
		for (int i = 0; i < population.size(); i += 2) {
			Path parent1 = selectParentTournament(values);
			Path parent2 = selectParentTournament(values);

			std::vector<int> crossoverPoints = utils::getRandomUniquePositions(parent1.size(), 2, rd);

			auto offspringPair = pmxCrossover(parent1, parent2, crossoverPoints[0], crossoverPoints[1]);
			newPopulation.push_back(offspringPair.first);
			newPopulation.push_back(offspringPair.second);
		}

		for (auto& individual : newPopulation) {
			if (std::uniform_real_distribution<double>(0.0, 1.0)(rd) < 0.03) {
				scrambleMutation(individual);
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

Path TSP::selectParentTournament(std::vector<double> values) {
	double bestIndividualValue = std::numeric_limits<double>::max();
	Path bestIndividual = population[0];

	std::vector<int> selectedIndexes =
		utils::getRandomUniquePositions(population.size(), selectionParam, rd);

	for (int i = 0; i < selectionParam; ++i) {
		int index = selectedIndexes[i];

		const auto& candidate = population[index];
		double candidateValue = values[index];

		if (candidateValue < bestIndividualValue) {
			bestIndividualValue = candidateValue;
			bestIndividual = candidate;
		}
	}

	return bestIndividual;
}

void TSP::calculateDistances(std::vector<Location>locations) {
	for (auto from : locations) {
		for (auto to : locations){
			distances[{from.id, to.id}] = utils::calcEuclDistance(from, to);
		}
	}
}

void TSP::generatePopulation(int populationSize, std::vector<Location>locations)
{
	Path locationsIDs;

	for (int i = 0; i < locations.size(); ++i) {
		locationsIDs.push_back(locations[i].id);
	}

	std::default_random_engine rng(rd());

	for (int i = 0; i < populationSize; ++i) {
		std::shuffle(locationsIDs.begin(), locationsIDs.end(), rng);
		population.push_back(locationsIDs);
	}
}

void TSP::scrambleMutation(Path& individual) {
	if (individual.size() < mutationParam) {
		throw std::runtime_error("ERROR: individual size is less than mutation parameter k");
	}
	std::vector<int> randomIndexes = utils::getRandomUniquePositions(
		individual.size(), mutationParam, rd);

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

std::pair<Path, Path> TSP::pmxCrossover(const Path& parent1, const Path& parent2, int crosspoint1, int crosspoint2) {
	if (crosspoint1 > crosspoint2) {
		std::swap(crosspoint1, crosspoint2);
	}
	return { createPmxOffspring(parent2, parent1, crosspoint1, crosspoint2),
			 createPmxOffspring(parent1, parent2, crosspoint1, crosspoint2) };
}

Path TSP::createPmxOffspring(Path parent1, Path parent2, int crosspoint1, int crosspoint2) {
	int n = parent1.size();
	Path offspring(n, -1);

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
	double length = 0.0;
	for (size_t i = 0; i < individual.size(); ++i) {
		int from = individual[i];
		int to = individual[(i + 1) %individual.size()];
		length += distances[{from, to}];
	}
	return length;
}

double TSP::compareToOpt(const Path& permutation) {
	double optResult = calcIndividualValue(permutation);
	double locatResult = calcIndividualValue(bestPath);

	std::cout << "Optimal path: " << optResult << std::endl;
	std::cout << "Out best path: " << locatResult << std::endl;

	return optResult - locatResult;
}
