#ifndef DERIVATIVE_H_
#define DERIVATIVE_H_

#include <initializer_list>
#include <functional>
#include <vector>

double derivative(const std::function<double(const double&)> &function,
		const double &x0, const double &epsilon=0.00001){
	double delta=epsilon, result=0, x=x0;
	result += function(x+delta)/(2*delta);
	result -= function(x-delta)/(2*delta);
	do{
		double newResult=0;
		delta *= 0.1;
		newResult += function(x+delta)/(2*delta);
		newResult -= function(x-delta)/(2*delta);
		if(std::abs(newResult-result) < epsilon){
			return newResult;
		}else result = newResult;
	}while(true);
}

double derivative_2(const std::function<double(const double&)> &function,
		const double &x0, const double &epsilon=0.00001){
	double delta=epsilon, result=0, x=x0;
	result += function(x+delta) + function(x-delta) - 2*function(x);
	result /= delta*delta;
	do{
		double newResult=0;
		delta *= 0.1;
		newResult += function(x+delta) + function(x-delta) - 2*function(x);
		newResult /= delta*delta;
		if(std::abs(newResult-result) < epsilon){
			return newResult;
		}else result = newResult;
	}while(true);
}

double derivative(const std::function<double(const std::vector<double>&)> &function,
				  std::vector<double> x, const size_t &i, const double &epsilon=0.00001){
	double delta = epsilon, result=0;
	x[i] += delta;
	result += function(x)/(2*delta);
	x[i] -= 2*delta;
	result -= function(x)/(2*delta);
	x[i] += delta;
	do{
		double newResult=0;
		delta *= 0.1;
		x[i] += delta;
		newResult += function(x)/(2*delta);
		x[i] -= 2*delta;
		newResult -= function(x)/(2*delta);
		x[i] += delta;
		if(std::abs(newResult-result) < epsilon){
			return newResult;
		}else result = newResult;
	}while(true);
}



#endif /* DERIVATIVE_H_ */
