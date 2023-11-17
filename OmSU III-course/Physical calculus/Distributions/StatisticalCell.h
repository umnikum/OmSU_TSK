#ifndef STATISTICALCELL_H_
#define STATISTICALCELL_H_

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "Range.h"
#include "StatisticalSumm.h"

#include <iostream>

template<typename T = double>
class StatisticalCell{
public:
	Range<T> range;
	std::vector<StatisticalSumm<T>> summs;
	StatisticalCell(const Range<T> &range):range(range){}
	StatisticalCell<T>& operator+=(const StatisticalCell<T>& cell){
		for(size_t i=0; i<summs.size(); ++i)
			summs[i] += cell.summs.at(i);
		return *this;
	}
	std::string toString()const{
		std::stringstream ss;
		ss << range.toString() << '\t' << summs.at(0).counter << '\t';
		for(auto summ:summs)
			ss << summ.value << '\t';
		return ss.str();
	}
	T average()const{return summs.at(0).average();}
	T dispertion()const{
		T average = summs.at(0).average();
		return summs.at(1).average() - average*average;
	}
	T assimetry()const{
		T average = summs.at(0).average(),
		  average2 = summs.at(1).average(),
		  average3 = summs.at(2).average(),
		  sigma = sqrt(dispertion());
		return (average3 - 3*average2*average + 2*average*average*average)
				/(sigma*sigma*sigma);
	}
	T excess()const{
		T average = summs.at(0).average(),
		  average2 = summs.at(1).average(),
		  average3 = summs.at(2).average(),
		  average4 = summs.at(3).average(),
		  disp = dispertion();
		return (average4 - 4*average3*average + 6*average2*average*average -3*average*average*average*average)
				/(disp*disp);
	}
};

StatisticalCell<> getStatisticalCell(const std::string &data){
	auto possibleIt1 = std::find(data.begin(), data.end(), '('),
		 possibleIt2 = std::find(data.begin(), data.end(), '[');
	auto leftBracket = (possibleIt1 < possibleIt2) ? possibleIt1 : possibleIt2;
	possibleIt1 = std::find(data.begin(), data.end(), ')');
	possibleIt2 = std::find(data.begin(), data.end(), ']');
	auto rightBracket = (possibleIt1 < possibleIt2) ? possibleIt1 : possibleIt2;
	StatisticalCell<> result{getRange(data.substr(leftBracket-data.begin(), rightBracket-leftBracket+1))};

	auto prevIt = rightBracket+1, nextIt = std::find(prevIt+1, data.end(), '\t');
	size_t counter = std::stoull(data.substr(prevIt-data.begin(), nextIt-prevIt));
	do{
		prevIt = nextIt;
		nextIt = std::find(prevIt+1, data.end(), '\t');
		if(nextIt - prevIt > 1){
			double value = std::stod(data.substr(prevIt-data.begin()+1, nextIt-prevIt-1));
			result.summs.push_back(StatisticalSumm<>{});
			auto summ = result.summs.end()-1;
			summ->counter = counter;
			summ->value= value;
		}
	}while(nextIt < data.end());
	return result;
}

#endif /* STATISTICALCELL_H_ */
