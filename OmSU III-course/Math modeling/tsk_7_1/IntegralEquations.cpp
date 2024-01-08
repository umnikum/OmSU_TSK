#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <cmath>

#include "Matrix.h"
#include "LinearAlgebra.h"

class IntegralEquation{
protected:
	double from, to;
	std::function<double(const double&, const double&)> core;
	std::function<double(const double&)> free_function;
public:
	IntegralEquation(const double &from, const double &to,
					 const std::function<double(const double&, const double&)> &core,
					 const std::function<double(const double&)> &free_function=[](const double &x){return 0;}):
						from{from}, to{to}, core(core), free_function(free_function){}
	std::vector<std::pair<double, double>> solve(const size_t &depth=10)const{
		const size_t size = depth+1;
		std::vector<double> x(size), f(size);
		Matrix system{size, size};
		const double h=(to-from)/depth, half_h=h/2;
		for(size_t i=0; i<size; ++i){
			x[i] = from + h*i;
			f[i] = free_function(x[i]);
		}
		for(size_t i=0; i<size; ++i){
			system[i][i]=1.0;
			for(size_t j=0; j<size; ++j){
				system[i][j] -= ((j==0 or j==depth)?half_h:h)*core(x[i], x[j]);
			}
		}
		std::vector<double> y = gauss(system, f);
		std::vector<std::pair<double, double>> result(size);
		for(size_t i=0; i<size; ++i)
			result[i] = {x[i], y[i]};
		return result;
	}
};


int main() {
	IntegralEquation equation{-M_PI, M_PI,
		[](const double &x, const double &s){
			const double cos = std::cos((x+s)/2);
			return 1/(0.64*cos*cos - 1);},
		[](const double &x){
				const double sin = std::sin(x);
				return 25.0 - 16*sin*sin;
			}};
	std::ofstream ofs{"points.out"};
	for(auto point:equation.solve(30))
		ofs << point.first << "\t" << point.second << "\n";
	return 0;
}
