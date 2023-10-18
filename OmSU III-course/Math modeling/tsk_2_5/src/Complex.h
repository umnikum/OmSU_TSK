#ifndef COMPLEX_H_
#define COMPLEX_H_

#include <cmath>
#include <string>
#include <sstream>

class Complex{
public:
	double re, im;
	Complex(const double &re=0.0, const double &im=0.0):re(re), im(im){}
	Complex paired()const{return Complex{re, -im};}
	friend double abs(const Complex &cnum){return sqrt(cnum.re*cnum.re + cnum.im*cnum.im);}
	friend bool operator==(const Complex &l, const Complex &r){return (l.re==r.re)&&(l.im==r.im);}
	friend bool operator!=(const Complex &l, const Complex &r){return !(l == r);}
	Complex operator+(const double &num)const{return Complex{re+num, im};}
	friend Complex operator+(const double &num, const Complex &cnum){return cnum+num;}
	Complex operator-(const double &num)const{return Complex{re-num, im};}
	friend Complex operator-(const double &num, const Complex &cnum){return cnum-num;}
	friend Complex operator-(const Complex &cnum){return Complex{-cnum.re, -cnum.im};}
	Complex operator*(const double &num)const{return Complex{re*num, im*num};}
	friend Complex operator*(const double &num, const Complex &cnum){return cnum*num;}
	Complex operator/(const double &num)const{return Complex{re/num, im/num};}
	friend Complex operator/(const double &num, const Complex &cnum){return Complex{num}/cnum;}
	Complex operator+(const Complex &cnum)const{return Complex{re+cnum.re, im+cnum.im};}
	Complex operator-(const Complex &cnum)const{return Complex{re-cnum.re, im-cnum.im};}
	Complex operator*(const Complex &cnum)const{
		return Complex{re * cnum.re - im * cnum.im, re * cnum.im + im * cnum.re};}
	Complex operator/(const Complex &cnum)const{
			return (*this)*(cnum.paired())/abs(cnum);}
	friend std::pair<Complex, Complex> sqrt(const Complex &cnum){
		double mod = sqrt(abs(cnum));
		Complex tmp = cnum/abs(cnum);
		double re = sqrt((1.0+tmp.re)/2.0), im;

		if(re == 0.0) im = 1.0;
		else im=tmp.im/(2.0*re);

		return {Complex{re, im}*mod, Complex{-re, -im}*mod};
	}
	std::string toString()const{
		std::stringstream ss;
		if((re != 0.0)||(im == 0.0))
			ss << re;
		if(im > 0.0)
			ss << "+" << im << "i";
		else if(im < 0.0)
			ss << im << "i";
		return ss.str();
	}
	~Complex() = default;
};

#endif /* COMPLEX_H_ */
