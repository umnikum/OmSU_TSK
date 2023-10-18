#include <iostream>
#include <cmath>
#include <vector>
#include <initializer_list>
#include "Complex.h"
#include "SqueareEquation.h"

Complex f(const Complex &x){return Complex{(pow(2, (x*x).re)) - 1/x};}

class ParabolicRootSearh{
public:
	Complex (*function)(const Complex&);
	std::vector<Complex> sequence;
	ParabolicRootSearh(Complex (*function)(const Complex&),
							   const std::initializer_list<Complex> &value={-1.0, 0.0, 1.0}):
							   function(function), sequence(value){}
	Complex search(const double &accuracy);
private:
	bool isSequenceConverging()const;
	std::pair<Complex, Complex> iterateSequence(const Complex &x_n, const Complex &x_n_1, const Complex &x_n_2);
};
bool ParabolicRootSearh::isSequenceConverging()const{
	auto end = sequence.cend()-1;
	return (sequence.size() <= 5)? true : abs(*end - *(end-1)) <= abs(*(end-1) - *(end-2));
}

std::pair<Complex, Complex> ParabolicRootSearh::iterateSequence(const Complex &x_n, const Complex &x_n_1, const Complex &x_n_2){
	Complex f_1=function(x_n),
			f_2=function(x_n)/(function(x_n)-function(x_n_1)),
			f_3=function(x_n)/(function(x_n)-2*function(x_n_1)+function(x_n_2));
	SqueareEquation eq(f_3, (f_2-(x_n_1 + x_n)*f_3), (f_1 - f_2*x_n + f_3*x_n*x_n_1));
	std::cout << eq.toString();
	return eq.solve();
}

Complex ParabolicRootSearh::search(const double &accuracy){
	auto end = sequence.cend()-1;
	while((isSequenceConverging())&&(abs(*end - *(end-1)) > accuracy)){
		auto solution = iterateSequence(*end, *(end-1), *(end-2));
		sequence.push_back(solution.first + *end);
	}
	return sequence.back();
}

int main() {
	ParabolicRootSearh a(f, {1.8, 1.2, 0.5});
	Complex x=a.search(0.0001);
	std::cout << x.toString() << " - root, f("<< x.toString() <<")= " << f(x).toString() << std::endl;
	for(auto x:a.sequence)
		std::cout << "f("<< x.toString() << ")= " << f(x).toString() << "\n";
	return 0;
}
