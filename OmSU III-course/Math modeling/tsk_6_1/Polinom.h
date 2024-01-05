#ifndef POLINOM_H_
#define POLINOM_H_

#include <vector>

class Polinom{
protected:
	std::vector<double> a;
public:
	Polinom(const std::vector<double> &coefficients={0}):a(coefficients){}
	double operator()(const double &x)const{
		double result{0.0};
		for(size_t i=0; i<a.size(); ++i){
			double x_pow{1.0};
			for(size_t pow=0; pow<i; ++pow)
				x_pow*=x;
			result += a[i]*x_pow;
		}
		return result;
	}
};


#endif /* POLINOM_H_ */
