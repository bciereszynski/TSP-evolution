#include "TSP.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "utils.h"
#include "RandomGenerator.h"

int main(int argc, char* argv[]) {
	std::string dataFilename = "data.tsp";
	std::string optFilename = "opt.tsp";
	std::string outFilename = "";
	int populationSize = 100;
	int iterations = 10;
	bool verbose = true;
	double mutationChance = 0.03;
	int mutationParam = 3;
	int selectionParam = 5;
	CrossoverMethod crossoverMethod = Ox;

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "--data" && i + 1 < argc) {
			dataFilename = argv[++i];
		}
		else if (arg == "--opt" && i + 1 < argc) {
			optFilename = argv[++i];
		}
		else if (arg == "--population" && i + 1 < argc) {
			populationSize = std::stoi(argv[++i]);
		}
		else if (arg == "--iterations" && i + 1 < argc) {
			iterations = std::stoi(argv[++i]);
		}
		else if (arg == "--verbose") {
			verbose = true;
		}
		else if (arg == "--mutation" && i + 1 < argc) {
			mutationParam = std::stoi(argv[++i]);
		}
		else if (arg == "--mutation%" && i + 1 < argc) {
			mutationChance = std::stoi(argv[++i]);
		}
		else if (arg == "--crossover" && i + 1 < argc) {
			int crossoverCode = std::stoi(argv[++i]);
			switch (crossoverCode) {
			case 0: crossoverMethod = Ox; break;
			case 1: crossoverMethod = Pmx; break;
			default: break;
			}
		}
		else if (arg == "--selection" && i + 1 < argc) {
			selectionParam = std::stoi(argv[++i]);
		}
		else {
			std::cerr << "Invalid parameter: " << arg << std::endl;
			return 1;
		}
	}

	std::ofstream outputFile(outFilename);
	std::ostream* output = &std::cout;
	if (outputFile.is_open()) {
		output = &outputFile;
	}
	else {
		std::cerr << "Results will be written to standard output - output filename empty or file name could not be opened" << std::endl;
	}

	*output << "Parameters:" << std::endl;
	*output << "  Data file: " << dataFilename << std::endl;
	*output << "  Opt path file: " << optFilename << std::endl;
	*output << "  Population size: " << populationSize << std::endl;
	*output << "  Iterations: " << iterations << std::endl;
	*output << "  Verbose: " << (verbose ? "Y" : "N") << std::endl;
	*output << "  Mutation chance: " << mutationChance << std::endl;
	*output << "  Mutation param: " << mutationParam << std::endl;
	*output << "  Selection param: " << selectionParam << std::endl;
	*output << "  Crossover method: " << crossoverMethod << std::endl;

	std::vector<Location> locations = utils::loadTSPLIB(dataFilename);

	if (locations.empty()){
		std::cerr << "ERROR: data not loaded" << std::endl;
		return 1;
	}

	TSP tsp(locations, populationSize, iterations, mutationChance, mutationParam, selectionParam, crossoverMethod, verbose);

	std::vector<int> optPath = utils::loadoptTSPLIB(optFilename);
	

	for (int i = 0; i < tsp.bestPath.size(); i++) {
		*output << tsp.bestPath[i] << std::endl;
	}
	tsp.compareToOpt(optPath, *output);

	Path test2Opt;
	for (int i = 0; i < locations.size(); ++i) {
		test2Opt.push_back(locations[i].id);
	}

	std::shuffle(test2Opt.begin(), test2Opt.end(), RandomGenerator());
	
	auto distances = utils::calculateDistances(locations);
	while (utils::twoOpt(test2Opt, distances, utils::BestImprovement)) {}
	double distance2Opt = 0.0;
	for (int i = 0; i < test2Opt.size(); i++) {
		distance2Opt += distances[{test2Opt[i], test2Opt[(i + 1) % test2Opt.size()]}];

	}

	*output << "2Opt value:" <<  distance2Opt << std::endl;
	outputFile.close();

	return 0;
}