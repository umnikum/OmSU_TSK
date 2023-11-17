#ifndef SIMPLISTICGENERATOR_H_
#define SIMPLISTICGENERATOR_H_

#include <limits>

template <typename T = int, typename P = size_t>
class SimplisticGenerator{
protected:
	static constexpr P step = 1;
	P param;
public:
	using result_type = T;
	SimplisticGenerator(const P &param = 0):param(param){}
	void seed(const P &value){param = value;}
	void discard(const size_t &amount){param += step*amount;}
	T operator()(){return static_cast<T>((param += step)%(max()-min()));}
	static constexpr T min(){return std::numeric_limits<T>::min();}
	static constexpr T max(){return std::numeric_limits<T>::max();}
	friend bool operator==(const SimplisticGenerator<T, P> &l, const SimplisticGenerator<T, P> &r){return l.param == r.param;}
	friend bool operator!=(const SimplisticGenerator<T, P> &l, const SimplisticGenerator<T, P> &r){return !(l == r);}
};

#endif /* SIMPLISTICGENERATOR_H_ */
