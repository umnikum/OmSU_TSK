#ifndef INTEGRAL_H_
#define INTEGRAL_H_

#include <functional>
#include <cmath>

double integralMiddleRectangulars(
		const double &fromLimit = 0.0, const double &toLimit = 1.0, const double &epsilon = 0.0001,
		const std::function<double(const double&)> &function = [](const double &x){return x;}
		){
	size_t amount = 16;
	double prevResult = function((toLimit - fromLimit)/2)*(toLimit-fromLimit);
	do{
		amount <<= 1;
		double h = (toLimit - fromLimit)/amount, result = 0.0;
		for(size_t i=1; i<=amount; ++i)
			result += (function(fromLimit + (i - 0.5)*h));
		result *= h;
		if(std::abs(result - prevResult) < epsilon)
			return result;
		else {
			prevResult = result;
		}
	}while(true);
}

double integralTrapezoid(
		const double &fromLimit = 0.0, const double &toLimit = 1.0, const double &epsilon = 0.0001,
		const std::function<double(const double&)> &function = [](const double &x){return x;}
		){
	size_t amount = 16;
	double prevResult = (function(toLimit)+function(fromLimit))*(toLimit-fromLimit)/2;
	do{
		amount <<= 1;
		double h = (toLimit - fromLimit)/amount,
				result = (function(toLimit)+function(fromLimit))/2;
		for(size_t i=1; i<amount; ++i)
			result += (function(fromLimit + i*h));
		result *= h;
		if(std::abs(result - prevResult) < epsilon)
			return result;
		else prevResult = result;
	}while(true);
}

double integralSimpson(
		const double &fromLimit = 0.0, const double &toLimit = 1.0, const double &epsilon = 0.0001,
		const std::function<double(const double&)> &function = [](const double &x){return x;}
		){
	size_t amount = 16;
	double prevResult = (function(toLimit)+function(fromLimit))*(toLimit-fromLimit)/2;
	do{
		amount <<= 2;
		double h = (toLimit - fromLimit)/amount,
				result = (function(toLimit)+function(fromLimit))/2 + 2*function(toLimit - 0.5*h);
		for(size_t i=1; i<amount; ++i)
			result += (function(fromLimit + i*h) + 2*function(fromLimit + (i - 0.5)*h));
		result *= h/3;
		if(std::abs(result - prevResult) < epsilon)
			return result;
		else prevResult = result;
	}while(true);
}

double integralSimpson_3_8(
		const double &fromLimit = 0.0, const double &toLimit = 1.0, const double &epsilon = 0.0001,
		const std::function<double(const double&)> &function = [](const double &x){return x;}
		){
	size_t amount = 3;
	double prevResult = (function(toLimit)
						+3*function((2*fromLimit + toLimit)/3)
						+3*function((fromLimit + 2*toLimit)/3)
						+function(fromLimit))
						*(toLimit-fromLimit)/8;
	do{
		amount *= 2;
		double h = (toLimit - fromLimit)/amount,
				result = 0;
		for(size_t i=0; i<amount/3; ++i)
			result += function(fromLimit + 3*i*h)
					+3*function(fromLimit + (3*i + 1)*h)
					+3*function(fromLimit + (3*i + 2)*h)
					+function(fromLimit + (3*i + 3)*h);
		result *= 0.375*h;
		if(std::abs(result - prevResult) < epsilon){
			return result;
		}else{
			prevResult = result;
		}
	}while(true);
}

#endif /* INTEGRAL_H_ */
