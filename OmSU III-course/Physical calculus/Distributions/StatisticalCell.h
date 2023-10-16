#ifndef STATISTICALCELL_H_
#define STATISTICALCELL_H_

#include <sstream>
#include <string>
#include <vector>
#include "Range.h"
#include "StatisticalSumm.h"

template<typename T = double>
class StatisticalCell{
public:
	Range<T> range;
	std::vector<StatisticalSumm<T>> summs;
	StatisticalCell(const Range<T> &range):range(range){}
	std::string toString()const{
		std::stringstream ss;
		ss << range.toString() << '\t';
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

#endif /* STATISTICALCELL_H_ */
