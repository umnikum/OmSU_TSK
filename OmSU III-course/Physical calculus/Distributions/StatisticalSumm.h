#ifndef STATISTICALSUMM_H_
#define STATISTICALSUMM_H_

#include <string>
#include <sstream>
#include <functional>

template<typename T = double>
class StatisticalSumm{
protected:
	std::function<T(const T&)> function;
public:
	T value;
	size_t counter;
	StatisticalSumm(const std::function<T(const T&)> &function = [](const T &x){return x;}):
		function(function), value(0), counter(0){}
	StatisticalSumm(const StatisticalSumm<T> &summ):
		function(summ.function), value(summ.value), counter(summ.counter){}
	StatisticalSumm<T>& operator+=(const T &value){
		this->value += this->function(value);
		this->counter++;
		return *this;
	}
	StatisticalSumm<T>& operator+=(const StatisticalSumm<T> &summ){
		this->value += summ.value;
		this->counter += summ.counter;
		return *this;
	}
	StatisticalSumm<T> operator+(const StatisticalSumm<T> &summ)const{
		StatisticalSumm<T> result(function);
		result.value = value + summ.value;
		result.counter = counter + summ.counter;
		return result;
	}
	T average()const{return value/static_cast<T>(counter);}
	~StatisticalSumm() = default;
};

#endif /* STATISTICALSUMM_H_ */
