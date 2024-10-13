#include "utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

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
		std::vector<int> indexes(n);
		for (int i = 0; i < n; ++i) {
			indexes[i] = i;
		}

		std::default_random_engine rng(rd());
		std::shuffle(indexes.begin(), indexes.end(), rng);

		std::vector<int> result(indexes.begin(), indexes.begin() + k);

		return result;
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
}  // namespace utils