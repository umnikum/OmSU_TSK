#ifndef DATAREADER_H_
#define DATAREADER_H_

#include <string>
#include <iostream>

template<class C, typename T = double>
class DataReader{
protected:
	C &partition;
public:
	DataReader(C& partition):
		partition(partition){}
	void read(const std::string &path){
		std::ifstream inputFileStream{path};
		while(!inputFileStream.eof()){
			T value;
			inputFileStream >> value;
		    partition.add(value);
		}
	}
};


#endif /* DATAREADER_H_ */
