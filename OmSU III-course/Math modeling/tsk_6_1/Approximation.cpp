#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include "Matrix.h"
#include "LinearAlgebra.h"
#include "Polinom.h"

class LeastSquaresMethod{
protected:
	std::vector<std::pair<double, double>> points;
public:
	LeastSquaresMethod(const std::vector<std::pair<double, double>> &points):points(points){}
	double check(const std::function<double(const double&)> &function)const{
		double result{0.0};
		for(auto point:points){
			double difference = point.second - function(point.first);
			result += difference*difference;
		}
		return result;
	}
};

class LeastSquaresPolinomialApproximation{
protected:
	std::vector<std::pair<double, double>> points;
public:
	LeastSquaresPolinomialApproximation(const std::vector<std::pair<double, double>> &points):points(points){}
	Polinom calculate(const size_t &pow)const{
		Matrix system{pow+1,pow+1};
		std::vector<double> freeCoefficients(pow+1);
		for(auto point:points){
			std::vector<double> x_pow(pow*2);
			for(size_t i=0; i<=2*pow; ++i){
				x_pow[i]=1;
				for(size_t power=0; power<i; ++power)
					x_pow[i]*=point.first;
			}
			for(size_t i=0; i<=pow; ++i){
				freeCoefficients[i] += point.second*x_pow[i];
				for(size_t j=0; j<=pow; ++j)
					system[i][j]+=x_pow[i+j];
			}
		}
		return Polinom{gauss(system, freeCoefficients)};
	}
};

int main() {
	std::ifstream ifs{"points.in"};
	std::vector<std::pair<double, double>> points;
	do{
		std::pair<double, double> point;
		ifs >> point.first >> point.second;
		points.push_back(point);
	}while(!ifs.eof());
	ifs.close();

	LeastSquaresPolinomialApproximation method{points};
	auto result = method.calculate(2);
	std::ofstream ofs{"points.out"};
	double x=points.front().first, dx=(points.back().first-x)/100;
	for(int i=0; i<=100; ++i){
		ofs << x << "\t" << result(x) << "\n";
		x += dx;
	}
	return 0;
}
