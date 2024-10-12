#include <iostream>

#include "Location.h"
#include "utils.h"

int main() {
	std::string filename = "data.tsp";
}

	std::vector<Location> locations = utils::loadTSPLIB(filename);

    if (!locations.empty()) {
		utils::printLocationsAndDistances(locations);
    }
    else {
        std::cerr << "ERROR: data not loaded" << std::endl;
    }

    return 0;
}