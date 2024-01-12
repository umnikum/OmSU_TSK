#include <iostream>
#include <map>
#include <filesystem>
#include <sstream>
#include <string>

#include "Config.h"
#include "Lattice.h"
#include "MonteKarloMethod.h"

int main()
{
	std::string path{"./Data"};
	std::filesystem::create_directory(path);
	Config config{};
	for(auto unit_pair:config.units){
		std::string unit_name=unit_pair.first, algorithm_name;
		bool is_eq{false};
		const std::map<std::string, std::string> &parameters{unit_pair.second};

		if(parameters.count("algorithm") > 0){
			algorithm_name = parameters.at("algorithm");
		}else algorithm_name = "Metropolis";
		if(parameters.count("eq") > 0)
			is_eq = std::stoi(parameters.at("eq"));

		MonteKarloMethod algorithm{parameters};
		std::stringstream ss;
		ss << path << "/" << algorithm_name;
		std::filesystem::create_directory(ss.str());
		ss << "/" << unit_name;
		std::filesystem::create_directory(ss.str());
		algorithm.start(algorithm_name, is_eq);
		std::cout << ss.str() << "\n";
		algorithm.collection.save(ss.str());
	}
    return 0;
}
