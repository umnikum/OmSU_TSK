#include <iostream>
#include <functional>
#include <cmath>
#include "Derivative.h"
#include "Integral.h"

class NewtonsRootSearch{
protected:
	const std::function<double(const double&)> &function;
public:
	NewtonsRootSearch(const std::function<double(const double&)> &function)
		:function(function){}
	double search(const double &x0, const double &epsilon = 0.0001){
		double x=x0, delta=0.0;
		do{
			delta = calcDeltaX(x, epsilon);
			x -= delta;
		}while(std::abs(delta) > epsilon);
		return x;
	}
private:
	double calcDeltaX(const double &x, const double &epsilon){
		double h = function(x)/derivative(function, x, epsilon);
		return 8*function(x)/
		(derivative(function, x, epsilon) + 3*derivative(function, x-h/3, epsilon) +
		 3*derivative(function, x-2*h/3, epsilon) + derivative(function, x-h, epsilon));
	}
};


int main() {
	std::cout << integralSimpson_3_8(0.0, 1.0, 0.0001, [](const double &x){return cosh(x*x);});
	std::cout << std::endl << integralSimpson_3_8(0.0, 1.0, 0.0001, [](const double &x){return x*x;});
	return 0;
}
