#ifndef SQUEAREEQUATION_H_
#define SQUEAREEQUATION_H_

#include "Complex.h"

class SqueareEquation{
public:
	Complex a, b, c;
	SqueareEquation(const double &a, const double &b, const double &c):a(a), b(b),c(c){}
	SqueareEquation(const Complex &a, const Complex &b, const Complex &c):a(a), b(b),c(c){}
	Complex operator()(const Complex &x){return a*x*x + b*x + c;}
	std::pair<Complex, Complex> solve()const{
		if(a != 0.0){
			Complex d=b*b - 4*a*c;
			if(abs(d) == 0.0){
				return {-b/(2*a), -b/(2*a)};
			}else{
				auto roots = sqrt(d);
				return {(roots.first - b)/(2*a), (roots.second - b)/(2*a)};
			}
		}else{
			if(b != 0){
				return {-c/b, -c/b};
			}else{
				return {Complex{}, Complex{}};
			}
		}
	}
	std::string toString()const{
		std::stringstream ss;
		ss << a.toString() << "*x^2 " << b.toString() << "*x " << c.toString() << " = 0";
		return ss.str();
	}
};



#endif /* SQUEAREEQUATION_H_ */
