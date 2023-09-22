#include <iostream>
#include <cmath>
#include <vector>

const double f(const double &x);
const double f(const double &x){return (pow(2, x*x)) - 1/x;}

const double abs(const double &x){return (x>=0.0)? x : -x;}

class Range{
public:
	const double begin;
	const double end;
	Range(const double &begin, const double &end);
	const double length()const;
	const double middle()const;
	const Range left()const;
	const Range right()const;
};

Range::Range(const double &begin, const double &end){
	this->begin = begin;
	this->end = end;
}
const double Range::length()const{return end-begin;}
const double Range::middle()const{return (begin+end)/2;}
const Range Range::left()const{return Range(begin, middle());}
const Range Range::right()const{return Range(middle(), end);}

class BisectionRootApproximation{
public:
	const double accuracy;
	const double (*function)(const double&);
	BisectionRootApproximation(const double &accuracy, const double (*function)(const double&));
	const double approximate(const Range &range)const;
private:
	bool hasSignChanged(const Range &range)const;
};

BisectionRootApproximation::BisectionRootApproximation(const double &accuracy, const double (*function)(const double&)){
	this->accuracy = accuracy;
	this->function = function;
}

bool BisectionRootApproximation::hasSignChanged(const Range &range)const{return (function(range.begin) * function(range.end) < 0.0);}

const double BisectionRootApproximation::approximate(const Range &range)const{
	if(range.length() < this->accuracy){
		return range.middle();
	}else{
		Range &left = range.left();
		if(hasSignChanged(left)){
			return approximate(left);
		}else return approximate(range.right());
	}
}

class ParabolicRootApproximation{
public:
	std::vector<double> sequence;
	const double (*function)(const double&);
	ParabolicRootApproximation(const double (*function)(const double&), const std::vector<double> &sequence);
	const double approximate(const double &accuracy);
private:
	bool isSequenceCollapsing()const;
};

ParabolicRootApproximation::ParabolicRootApproximation(const double (*function)(const double&), const std::vector<double> &sequence){this->function = function;}

bool ParabolicRootApproximation::isSequenceCollapsing()const{
	auto end = sequence.end()-1;
	return abs(*end - *(end-1)) < abs(*(end-1) - *(end-2));
}

const double ParabolicRootApproximation::approximate(const double &acuracy){

}

int main() {

	return 0;
}
