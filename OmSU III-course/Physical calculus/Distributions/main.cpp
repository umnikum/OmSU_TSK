#include <iostream>
#include <cinttypes>
#include <chrono>
#include "DataWriter.h"
#include "DataReader.h"
#include "Partition.h"
#include "SimplisticGenerator.h"

template<class Generator>
Partition<> generateData(Generator gen, const double &min, const double &max, const size_t &amount = 1'000'000){
	std::initializer_list<std::function<double(const double&)>> moments{
							[](const double &x){return x;},
							[](const double &x){return x*x;},
							[](const double &x){return x*x*x;},
							[](const double &x){return x*x*x*x;}};
	Partition<> partition{initializeCells(partUniform(Range<>{min, max}), moments)};
	std::uniform_real_distribution<> dist{min, max};
	for(size_t i=0; i<amount; ++i)
		partition.add(dist(gen));
	return partition;
}

template<class Generator>
Partition<> generateDataForIntervals(Generator gen, const double &min, const double &max, const size_t &intervalAmount = 10){
	std::initializer_list<std::function<double(const double&)>> moments{
							[](const double &x){return x;},
							[](const double &x){return x*x;},
							[](const double &x){return x*x*x;},
							[](const double &x){return x*x*x*x;}};
	Partition<> partition{initializeCells(partUniform(Range<>{min, max}, intervalAmount), moments)};
	std::uniform_real_distribution<> dist{min, max};
	for(size_t i=0; i<1'000'000'000; ++i)
		partition.add(dist(gen));
	return partition;
}

template<class Generator>
Partition<> generateExponentialData(Generator gen, const double &lambda=1.0, const double &M=100.0, const size_t &amount=100'000'000){
	std::initializer_list<std::function<double(const double&)>> moments{[](const double &x){return x;}};
	Partition<> partition{initializeCells(partExponential(lambda, M, 100.0), moments)};
	std::exponential_distribution<> dist{lambda};
	for(size_t i=0; i<amount; ++i)
		partition.add(dist(gen));
	return partition;
}

int main(){
	std::string dataPath="/home/hostmaster/Documents/Doc/Projects/Programms/"
			"c\\c++/OmSU III-course/Physical calculus/Distributions/Data/";
	std::ofstream outputFileStream{};
	outputFileStream.open(dataPath+"Analitics/MerceneTwisterExponential_1.dat");
	for(double M = 10.0; M <= 1000.0; M*=10.0){
		const auto t_start = std::chrono::system_clock::now();
		//generateDataForIntervals(std::mt19937{std::random_device{}()}, 0.0, 10.0, scale[i]);
		//generateDataForIntervals(SimplisticGenerator<char>{std::random_device{}()}, 0.0, 10.0, scale[i]);
		Partition<> partition = generateExponentialData(std::mt19937{std::random_device{}()}, 1.0, M);
		const auto t_end = std::chrono::system_clock::now();
		using namespace std::literals::chrono_literals;
		const uint64_t elapsed_time = std::chrono::nanoseconds(t_end - t_start).count();
		outputFileStream << M << "\t" << elapsed_time << "\n";
		outputFileStream.flush();
		std::stringstream ss;
		ss << dataPath << "ProcessedValues/exponential_values_" << M << ".dat";
		DataWriter<> writer{partition};
		writer(ss.str());
	}
	outputFileStream.close();
	return 0;
}


