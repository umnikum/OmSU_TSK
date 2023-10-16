#ifndef DEBUG_HALFRANGE_H_
#define DEBUG_HALFRANGE_H_

#include <string>
#include <sstream>

template<typename T = double>
class Range{
protected:
	char leftBracket = '(';
	char rightBracket = ')';
public:
	T begin;
	T end;
	Range() = default;
	Range(const T &begin, const T &end){
		if(begin < end){
			this->begin = begin;
			this->end = end;
		}else{
			this->begin = end;
			this->end = begin;
		}
	}
	bool toLeft(const T &value)const{return value <= begin;}
	bool toRight(const T &value)const{return value >= end;}
	std::string toString()const{
		std::stringstream ss;
		ss << leftBracket << begin << ",\t" << end << rightBracket;
		return ss.str();
	}
	~Range() = default;
};

template<typename T = double>
class Segment : public Range<T>{
public:
	using Range<T>::begin;
	using Range<T>::end;
	Segment(const T &begin, const T &end){
		if(begin < end){
			this->begin = begin;
			this->end = end;
		}else{
			this->begin = end;
			this->end = begin;
		}
		Range<T>::rightBracket = ']';
		Range<T>::leftBracket = '[';
	}
	bool toLeft(const T &value)const {return value < this->begin;}
	bool toRight(const T &value)const {return value > this->end;}
};

template<typename T = double>
class HalfRange : public Range<T>{
protected:
	char direction = 'l';
public:
	using Range<T>::begin;
	using Range<T>::end;
	HalfRange(const T &begin, const T &end, const char &direction = 'l'){
		if(begin < end){
			this->begin = begin;
			this->end = end;
		}else{
			this->begin = end;
			this->end = begin;
		}
		switch(direction){
		case 'r':
			Range<T>::rightBracket = ']';
			this->direction = 'r';
			break;
		case 'l':
		default:
			Range<T>::leftBracket = '[';
		}
	}
	bool toLeft(const T &value)const{
		return (direction == 'l')? value < this->begin : value <= this->begin;}
	bool toRight(const T &value)const{
		return (direction == 'l')? value >= this->end : value > this->end;}
};

#endif /* DEBUG_HALFRANGE_H_ */
