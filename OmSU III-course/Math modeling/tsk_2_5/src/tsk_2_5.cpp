#include <iostream>
#include <cmath>
#include <vector>
#include <initializer_list>
#include <functional>
#include "Complex.h"
#include "SqueareEquation.h"

Complex f(const Complex &x){return Complex{pow(2, (x * x.conjugate()).re) - 1/x};}

class DividedDifference{
private:
	const std::function<Complex(const Complex&)> &function;
public:
	DividedDifference(const std::function<Complex(const Complex&)> &function):function(function){}
	Complex operator()(const Complex &x_1)const{return function(x_1);}
	Complex operator()(const Complex &x_1, const Complex &x_2)const{
		return (function(x_2)-function(x_1))/(x_2-x_1);
	}
	Complex operator()(const Complex &x_1, const Complex &x_2, const Complex &x_3)const{
		return (operator()(x_2, x_3)-operator()(x_1, x_2))/(x_3-x_1);
	}
};

class ParabolicRootSearh{
private:
	const DividedDifference dDiff;
public:
	std::vector<Complex> sequence;
	ParabolicRootSearh(const std::function<Complex(const Complex&)> &function,
							   const std::initializer_list<Complex> &value={-1.0, 0.0, 1.0}):
							   dDiff(function), sequence(value){}
	Complex search(const double &accuracy);
private:
	bool isSequenceConverging()const;
	std::pair<Complex, Complex> iterateSequence(const Complex &x_n_2, const Complex &x_n_1, const Complex &x_n);
};
bool ParabolicRootSearh::isSequenceConverging()const{
	auto end = sequence.cend()-1;
	return (sequence.size() <= 5)? true : abs(*end - *(end-1)) <= abs(*(end-1) - *(end-2));
}

std::pair<Complex, Complex> ParabolicRootSearh::iterateSequence(const Complex &x_n_2, const Complex &x_n_1, const Complex &x_n){
	SqueareEquation eq(dDiff(x_n_2, x_n_1, x_n), dDiff(x_n_1, x_n)+dDiff(x_n_2, x_n)-dDiff(x_n_2, x_n_1), dDiff(x_n));
	return eq.solve();
}

Complex ParabolicRootSearh::search(const double &accuracy){
	auto end = sequence.cend()-1;
	while((isSequenceConverging())&&(abs(*end - *(end-1)) > accuracy)){
		auto solution = iterateSequence(*(end-2), *(end-1), *end);
		//(abs(solution.first) < abs(solution.second))?
				//sequence.push_back(solution.first + *end):
				sequence.push_back(solution.second + *end);
		end = sequence.cend()-1;
	}
	return sequence.back();
}

int main() {
	ParabolicRootSearh a(f, {0.3, 0.5, 1.0});
	Complex x=a.search(0.00001);
	std::cout << x.toString() << " - root, f("<< x.toString() <<") = " << f(x).toString() << std::endl;
	for(auto x:a.sequence)
		std::cout << "f("<< x.toString() << ") = " << f(x).toString() << "\n";
	return 0;
}
