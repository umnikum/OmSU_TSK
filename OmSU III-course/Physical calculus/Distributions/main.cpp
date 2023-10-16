#include <iostream>

#include "DataWriter.h"
#include "DataReader.h"
#include "SimplisticDistribution.h"
#include "Partition.h"

int main(){
	std::string dataPath="/home/hostmaster/Documents/Doc/Projects/Programms/"
			"c\\c++/OmSU III-course/Physical calculus/Distributions"
			"/Data/SimplisticDistribution/";
	/*SimplisticDistribution<> distr{};
	DataWriter<SimplisticDistribution<>> writer{distr, dataPath};
	writer(2000);*/
	std::initializer_list<std::function<double(const double&)>> moments{
				[](const double &x){return x;},
				[](const double &x){return x*x;}};
	Partition<> a{initializeCells(partUniform(Range<double>{-5.0, 5.0}), moments)};
	DataReader<Partition<>, double> reader{a};
	std::stringstream ss;
	ss << dataPath << "values_0.dat";
	reader.read(ss.str());
	std::cout << a.unite().average();
	return 0;
}


