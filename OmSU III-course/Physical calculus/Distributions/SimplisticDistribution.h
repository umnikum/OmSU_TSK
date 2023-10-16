#ifndef SIMPLISTICDISTRIBUTION_H_
#define SIMPLISTICDISTRIBUTION_H_

#include <limits>

template <typename T = double, typename P = double>
class SimplisticDistribution{
private:
	static constexpr P step = 1;
	P p, start;
public:
	SimplisticDistribution(const P &param = 0.0):p(param), start(param){}
	void reset(){p = start;}
	T operator()(){
		p += step;
		return static_cast<T>(p-step);
	}
	T min(){return std::numeric_limits<T>::min();}
	T max(){return std::numeric_limits<T>::max();}
	P param(){return p;}
};

template<typename T = double, typename P = double>
bool operator==(const SimplisticDistribution<T, P> &l, const SimplisticDistribution<T, P> &r){return l.param() == r.param();}
template<typename T = double, typename P = double>
bool operator!=(const SimplisticDistribution<T, P> &l, const SimplisticDistribution<T, P> &r){return !(l == r);}

#endif /* SIMPLISTICDISTRIBUTION_H_ */
