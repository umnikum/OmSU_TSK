#include <iostream>
#include <initializer_list>
#include <functional>
#include <cmath>
#include <vector>
#include "Matrix.h"
#include "LinearAlgebra.h"

double f1(const std::vector<double> &x){
	return x[0]*x[0] + x[1]*x[1];
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

class NewtonMethod{
public:
	const std::initializer_list<std::function<double(const std::vector<double>&)>> &functions;

	NewtonMethod(const std::initializer_list<std::function<double(const std::vector<double>&)>> &functions):
		functions(functions){}

	std::vector<double> solve(const std::vector<double> &x0, const double &epsilon = 0.00001){
		std::vector<double> x = x0, deltaX;
		do{
			deltaX = std::move(gauss(calcDerivativeMatrix(x, epsilon), calcValues(x)));
			for(size_t i=0; i<x.size(); ++i)
				x[i] += deltaX[i];
		}while(!convergenceCondition(deltaX, epsilon));
		return x;
	}
private:
	bool convergenceCondition(const std::vector<double> &deltaX, const double &epsilon){
		double summ = 0;
		for(auto dx:deltaX)
			summ+=std::abs(dx);
		return summ < epsilon;
	}

	std::vector<double> calcValues(const std::vector<double> &x){
		std::vector<double> result(functions.size());
		for(size_t i=0; i<functions.size(); ++i)
			result[i] = (*std::next(functions.begin(), i))(x);
		return result;
	}

	Matrix calcDerivativeMatrix(const std::vector<double> &x, const double &epsilon){
		size_t rowCount = functions.size(), columnCount = x.size();
		Matrix result{rowCount, columnCount};
		for(size_t rIndex=0; rIndex<rowCount; ++rIndex)
			for(size_t cIndex=0; cIndex<columnCount; ++cIndex)
				result[rIndex][cIndex] = derivative((*std::next(functions.begin(), rIndex)), x, cIndex, epsilon);
		return result;
	}
};

int main(){
	std::vector<double> x{3.0, 4.0};
	std::cout << f1(x) << " and derivative: " <<derivative(f1, x, 1);
	return 0;
}
