#ifndef DATAWRITER_H_
#define DATAWRITER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

template<class T>
class DataWriter{
protected:
	static constexpr size_t inFileAmount = 1000000, inLineAmount=10;
	T &distribution;
	const std::string &dataPath;
public:
	DataWriter(T& distr, const std::string &path):
		distribution(distr), dataPath(path){}
	void operator()(const size_t &amount){
		int fileNumber=0;
		std::ofstream outputFileStream{};
		for(size_t i=0; i<amount; ++i){
			if(i%inFileAmount == 0){
				std::stringstream ss;
				ss << dataPath << "values_" << fileNumber << ".dat";
				outputFileStream.open(ss.str());
				fileNumber++;
			}
			outputFileStream << distribution() << "\t";
			if((i+1)%inLineAmount == 0){
				outputFileStream.seekp(-1, std::ios_base::end);
				outputFileStream << '\n';
			}
			if(i%inFileAmount == inFileAmount-1){
				outputFileStream.flush();
				outputFileStream.close();
			}
		}
		outputFileStream.flush();
		outputFileStream.close();
	}
};

#endif /* DATAWRITER_H_ */
