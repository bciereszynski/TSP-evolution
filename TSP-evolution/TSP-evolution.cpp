#include "TSP.h"

#include <algorithm> 
#include <random>    


TSP::TSP(int populationSize, std::vector<Location>locations) {
	std::random_device rd;

	population =
		generatePopulation(populationSize, locations, rd);
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

std::pair<std::vector<int>, std::vector<int>> TSP::pmxCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2, std::random_device& rd) {
	int n = parent1.size();

	std::vector<int> crossoverPoints = utils::getRandomUniquePositions(n, 2, rd);
	if (crossoverPoints[0] > crossoverPoints[1]) {
		std::swap(crossoverPoints[0], crossoverPoints[1]);
	}

	return { createOffspring(crossoverPoints[0], crossoverPoints[1], parent2, parent1),
			 createOffspring(crossoverPoints[0], crossoverPoints[1], parent1, parent2) };
}

std::vector<int> TSP::createOffspring(int k, int l, std::vector<int> parent1, std::vector<int> parent2) {
	int n = parent1.size();
	std::vector<int> offspring(n, -1);
	for (int i = k; i <= l; ++i) {
		offspring[i] = parent1[i];
	}

	for (int i = k; i <= l; ++i) {
		auto subpartSerarchOffset = n - l - 1;
		// Sprawdź czy dany element rodzica został skopiowany do potomka
		if (std::find(offspring.begin() + k, offspring.end() - subpartSerarchOffset,
			parent2[i]) != offspring.end() - subpartSerarchOffset) {
			continue;
		}

		// El. odpowiadjący offspring2[i] został skopiowany na miejsce el. nieskopiowanego parent2[i]
		// Sprawdz czy element skopiowany znajduje się w zbiorze krzyżowym rodzica 
		int correspondingValue;
		auto correspondingIt = std::find(parent2.begin() + k, parent2.end() - subpartSerarchOffset, offspring[i]);
		if (correspondingIt == parent2.end() - subpartSerarchOffset) {
			// Jeżeli element nie znajduje się w zbiorze, to znaczy że możemy wstawić nieskopiowany element w jego miejsce
			correspondingValue = offspring[i];
		}
		else {
			//W przeciwnym wypadku, sprawdzamy jaka wartość została skopiowana w jego miejsce
			int correspondingIndex = std::distance(parent2.begin(), correspondingIt);
			correspondingValue = offspring[correspondingIndex];
		}
		//Wstawiamy element nieskopiowany w miejsce w którym znajduje się jego element odpowiadający
		auto it = std::find(parent2.begin(), parent2.end(), correspondingValue);
		int index = std::distance(parent2.begin(), it);
		offspring[index] = parent2[i];
	}

	for (int i = 0; i < n; ++i) {
		if (offspring[i] == -1) {
			offspring[i] = parent2[i];
		}

	}
	return offspring;
};