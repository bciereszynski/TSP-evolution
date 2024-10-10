#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

struct Location {
    int id;
    double x;
    double y;
};

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

int main() {
    std::string filename = "data.tsp";  // Nazwa pliku TSPLIB
    std::vector<Location> locations = loadTSPLIB(filename);

    if (!locations.empty()) {
        printLocationsAndDistances(locations);
    }
    else {
        std::cerr << "ERROR: data not loaded" << std::endl;
    }

    return 0;
}