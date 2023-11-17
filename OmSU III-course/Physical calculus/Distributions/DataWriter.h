#ifndef DATAWRITER_H_
#define DATAWRITER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include "Partition.h"

template<typename T = double>
class DataWriter{
protected:
	const Partition<T> &partition;
public:
	DataWriter(const Partition<T> &partition):
		partition(partition){}
	void operator()(const std::string &path){
		std::ofstream outputFileStream{path};
		outputFileStream << partition.toString();
		outputFileStream.close();
	}
};

#endif /* DATAWRITER_H_ */
