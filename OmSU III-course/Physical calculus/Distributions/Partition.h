#ifndef PARTITION_H_
#define PARTITION_H_

#include <string>
#include <sstream>
#include <vector>
#include <initializer_list>
#include "Range.h"
#include "StatisticalCell.h"


template<typename T = double>
class Partition{
protected:
	std::vector<StatisticalCell<T>> cells;
public:
	Partition(const std::vector<StatisticalCell<T>> &cells):cells(cells){}
	size_t& size()const{return cells.size();}
	StatisticalCell<T>& search(const T &value,
					const typename std::vector<StatisticalCell<T>>::iterator &begin,
					const typename std::vector<StatisticalCell<T>>::iterator &end)const{
		if(end-begin == static_cast<size_t>(1))
			return *begin;
		else{
			size_t halfSize = static_cast<size_t>((end-begin)/2);
			return ((*std::next(begin, halfSize)).range.toLeft(value))?
					search(value, begin, std::next(begin, halfSize)):
					search(value, std::next(begin, halfSize), end);
		}
	}
	StatisticalCell<T>& operator[](const size_t &index)const{return cells.at(index);}
	Partition<T>& add(const T &value){
		StatisticalCell<T>&  cell = search(value, cells.begin(), cells.end());
		for(StatisticalSumm<T> &summ:cell.summs)
			summ+=value;
		return *this;
	}
	StatisticalCell<T> unite()const{
		StatisticalCell<T> cell{Range<T>{cells.front().range.begin, cells.back().range.end}};
		for(StatisticalSumm<T> summ:cells.front().summs)
			cell.summs.push_back(summ);
		for(size_t summIndex=0; summIndex<cell.summs.size(); ++summIndex)
			for(size_t cellIndex=1; cellIndex<cells.size(); ++cellIndex)
				cell.summs.at(summIndex) += cells.at(cellIndex).summs.at(summIndex);
		return cell;
	}
	std::string toString()const{
		std::stringstream ss;
		for(const StatisticalCell<T> &cell:cells)
			if(cell.summs.at(0).counter > 0)
				ss << cell.toString() << std::endl;
		return ss.str();
	}
	~Partition<T>() = default;
};

template<typename T = double>
std::vector<Range<T>> partUniform(const Range<T> &range, const size_t &partAmount = 10){
	std::vector<Range<T>> ranges(partAmount);
	T partSize = (range.end-range.begin) / static_cast<T>(partAmount);
	for(size_t i=0; i<partAmount-1; ++i)
		if(range.begin + (i+1)*partSize <= range.end)
			ranges[i] = HalfRange<T>{range.begin + static_cast<T>(i)*partSize,
									range.begin + static_cast<T>(i+1)*partSize};
		else break;
	ranges[partAmount-1] = Segment<T>{range.end-partSize, range.end};
	return ranges;
}

template<typename T = double>
std::vector<StatisticalCell<T>> initializeCells(const std::vector<Range<T>> &ranges,
												const std::initializer_list<std::function<T(const T&)>> &functions){
	std::vector<StatisticalCell<T>> cells;
	for(const Range<T> &range:ranges){
		StatisticalCell<T> cell{range};
		for(const std::function<T(const T&)> &function:functions)
			cell.summs.push_back(StatisticalSumm<T>{function});
		cells.push_back(cell);
	}
	return cells;
}


#endif /* PARTITION_H_ */
