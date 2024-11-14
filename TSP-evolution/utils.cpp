#include "utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <unordered_set>

namespace utils {
	double calcEuclDistance(const Location& location1, const Location& location2) {
		return sqrt(pow(location1.x - location2.x, 2) + pow(location1.y - location2.y, 2));
	}

	std::vector<Location> loadTSPLIB(const std::string& filename) {
		std::vector<Location> locations;
		std::ifstream file(filename);
		std::string line;
		bool inNodeCoordSection = false;

		if (!file) {
			std::cerr << "ERROR: can't open file: " << filename << std::endl;
			return locations;
		}

		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string keyword;

			iss >> keyword;

			if (keyword == "NODE_COORD_SECTION") {
				inNodeCoordSection = true;
				continue;
			}

			if (keyword == "EOF") {
				break;
			}

			if (inNodeCoordSection) {
				Location location;
				location.id = std::stoi(keyword);
				iss >> location.x >> location.y;
				locations.push_back(location);
			}
		}

		return locations;
	}

	std::vector<int> loadoptTSPLIB(const std::string& filename) {
		std::vector<int> permutation;
		std::ifstream file(filename);
		std::string line;
		bool inTOUR_SECTION = false;

		if (!file) {
			std::cerr << "ERROR: can't open file: " << filename << std::endl;
			return permutation;
		}

		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string keyword;

			iss >> keyword;

			if (keyword == "TOUR_SECTION") {
				inTOUR_SECTION = true;
				continue;
			}

			if (!inTOUR_SECTION) {
				continue;
			}

			int id = std::stoi(keyword);

			if (id == -1) {
				break;
			}

			if (inTOUR_SECTION) {
				permutation.push_back(id);
			}
		}

		return permutation;
	}

	std::vector<int> getRandomUniquePositions(const int n, const int k, std::random_device& rd) {
		if (k > n / 10) {
			std::vector<int> indexes(n);
			for (int i = 0; i < n; ++i) {
				indexes[i] = i;
			}

			std::default_random_engine rng(rd());
			std::shuffle(indexes.begin(), indexes.end(), rng);

			std::vector<int> result(indexes.begin(), indexes.begin() + k);

			return result;

		}
		else {
			std::vector<int> result;
			result.reserve(k);
			std::default_random_engine rng(rd());
			std::uniform_int_distribution<int> dist(0, n - 1);
			std::unordered_set<int> selected;

			while (result.size() < k) {
				int pos = dist(rng);
				if (selected.insert(pos).second){
					result.push_back(pos);
				}
			}

			return result;
		}	
	}

	void printLocationsAndDistances(const std::vector<Location>& locations) {
		std::cout << "Locations:" << std::endl;
		for (const auto& location : locations) {
			std::cout << "ID: " << location.id << ", x: " << location.x << ", y: " << location.y << std::endl;
		}

		std::cout << "\nDistances:" << std::endl;
		for (size_t i = 0; i < locations.size(); ++i) {
			for (size_t j = i + 1; j < locations.size(); ++j) {
				double dist = calcEuclDistance(locations[i], locations[j]);
				std::cout << "From " << locations[i].id
					<< " to " << locations[j].id << ": " << dist << std::endl;
			}
		}
	}

	void printPopulation(const std::vector<std::vector<int>>& population) {
		std::cout << "Population:" << std::endl;
		for (const auto& individual : population) {
			for (int locationID : individual) {
				std::cout << locationID << " ";
			}
			std::cout << '\n';
		}
	}

	std::map<std::pair<int, int>, double> calculateDistances(const std::vector<Location>locations) {
		std::map<std::pair<int, int>, double> distances;
		for (auto from : locations) {
			for (auto to : locations) {
				distances[{from.id, to.id}] = utils::calcEuclDistance(from, to);
			}
		}
		return distances;
	}

	// 2-opt algorithm with first improvement rule
	void twoOptFI(std::vector<int>& individual, std::map<std::pair<int, int>, double> distances) {
		auto n = individual.size();
		if (n < 3) {
			return;
		}

		for (int i = 0; i < n - 1; i++) {
			for (int j = i + 1; j < n; j++) {
				auto lengthChange =
					distances[{individual[(i - 1) % n], individual[j]}] -
					distances[{individual[(i - 1) % n], individual[i] }] -
					distances[{individual[j], individual[(j + 1) % n]}] +
					distances[{individual[i], individual[(j + 1) % n]}];
				if (lengthChange < 0) {
					std::swap(individual[i], individual[j]);
					return;
				}
			}
		}
	}
}  // namespace utils