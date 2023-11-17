#ifndef DATAREADER_H_
#define DATAREADER_H_

#include <string>
#include <iostream>
#include "Partition.h"

class DataReader{
protected:
	Partition<> &partition;
public:
	DataReader(Partition<> &partition):
		partition(partition){}
	Partition<> read(const std::string &path){
		std::ifstream inputFileStream{path};
		std::string line;
		int index = 0;
		do{
			std::getline(inputFileStream, line);
			if(!line.empty()){
				partition[index] += getStatisticalCell(line);
				//std::cout << partition[index].toString() << std::endl;
				++index;
			}
		}while(!inputFileStream.eof());
		inputFileStream.close();
		return partition;
	}
};


#endif /* DATAREADER_H_ */
