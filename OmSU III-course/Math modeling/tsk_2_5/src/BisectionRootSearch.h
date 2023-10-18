#ifndef BISECTIONROOTSEARCH_H_
#define BISECTIONROOTSEARCH_H_

class Range{
public:
	double begin;
	double end;
	Range(const double &begin, const double &end):begin(begin), end(end){};
	const double length()const{return end-begin;}
	const double middle()const{return (begin+end)/2;}
	const Range left()const{return Range(begin, middle());}
	const Range right()const{return Range(middle(), end);}
};

class BisectionRootSearch{
public:
	double accuracy;
	const double (*function)(const double&);
	BisectionRootSearch(const double &accuracy, const double (*function)(const double&)):
		accuracy(accuracy), function(function){}
	const double approximate(const Range &range)const;
private:
	bool hasSignChanged(const Range &range)const{return (function(range.begin) * function(range.end) < 0.0);}
};

const double BisectionRootSearch::approximate(const Range &range)const{
	if(range.length() < this->accuracy){
		return range.middle();
	}else{
		Range left = range.left();
		if(hasSignChanged(left)){
			return approximate(left);
		}else return approximate(range.right());
	}
	return 0;
}

#endif /* BISECTIONROOTSEARCH_H_ */
