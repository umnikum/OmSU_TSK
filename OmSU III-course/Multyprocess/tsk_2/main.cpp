#include <iostream>
#include <omp.h>
#include <cmath>
#include <vector>
#include <mpi.h>
#include "Integral.h"

#define VLEN 10

double derArctg(const double &x){return 1/(x*x + 1);}

double calcPi(const double &epsilon,
		const std::function<double
		(const double&,const double&,const double&,
		const std::function<double(const double&)>&
		)> &integral)
{return 4*integral(0, 1, epsilon, derArctg);}

std::vector<double> ompPi(){
	constexpr double step = 0.1;
	double epsilon[VLEN];
	std::vector<double> pi(VLEN);
	int i;
	for(i=0; i<VLEN; ++i)
		epsilon[i] = pow(step, i+5);
	#pragma omp parallel for private(i)
		for(i=0; i<VLEN; ++i)
			pi[i]=calcPi(epsilon[i], integralMiddleRectangulars);
	return pi;
}

int main(){
	constexpr double step = 0.1;
	double epsilon[VLEN];
	std::vector<double> pi(VLEN);
	int i;
	for(i=0; i<VLEN; ++i)
		epsilon[i] = pow(step, i+5);
	#pragma omp parallel for private(i)
		for(i=0; i<VLEN; ++i)
			pi[i]=calcPi(epsilon[i], integralMiddleRectangulars);
	return 0;
}

