#include <iostream>
#include <fstream>
#include <initializer_list>
#include <vector>
#include <functional>
#include <cmath>
#include "Range.h"
#include "Polinom.h"
#include "Derivative.h"
#include "Matrix.h"
#include "LinearAlgebra.h"

class Interpolation{
protected:
	std::vector<Polinom> splines;
	const std::vector<Range<>> ranges;
	size_t search(const double &value,
				const size_t &begin,
				const size_t &end)const{
		if(end-begin == static_cast<size_t>(1))
			return begin;
		else{
			size_t halfSize = ((end-begin)/2);
			return ((ranges.at(begin+halfSize)).toLeft(value))?
					search(value, begin, begin+halfSize):
					search(value, begin+halfSize, end);
		}
	}
public:
	Interpolation(const std::vector<Polinom> &splines,
				  const std::vector<Range<>> &ranges):splines(splines), ranges(ranges){}
	double operator()(const double &x)const{return splines.at(search(x, 0, ranges.size()))(x);}
};

class CubicSplineInterpolation{
protected:
	std::vector<std::pair<double, double>> points;
public:
	CubicSplineInterpolation(const std::vector<std::pair<double, double>> &points):points(points){}
	Interpolation interpolate(const double &epsilon=0.00001)const{
		const size_t n = points.size()-1;
		std::vector<double> d(n+1);
		d[0] = 6*(points[1].second-points[0].second)/((points[1].first-points[0].first)*(points[1].first-points[0].first));
		d[n] = -6*(points[n].second-points[n-1].second)/((points[n].first-points[n-1].first)*(points[n].first-points[n-1].first));
		for(size_t i=1; i<n; ++i)
			d[i] = (points[i+1].second-points[i].second)/((points[i+1].first-points[i].first)*(points[i+1].first-points[i-1].first)) -
					(points[i].second-points[i-1].second)/((points[i].first-points[i-1].first)*(points[i+1].first-points[i-1].first));

		Matrix a{n+1, n+1};
		a[0][1] = 1; a[n][n-1] = 1;
		for(size_t i=1; i<n; ++i)
			a[i][i+1] = (points[i+1].first - points[i].first)/(points[i+1].first - points[i-1].first);
		for(size_t i=0; i<n-1; ++i)
			a[i+1][i] = (points[i+1].first - points[i].first)/(points[i+2].first - points[i].first);
		for(size_t i=0; i<n+1; ++i)
			a[i][i] = 2;
		std::vector<double> M = gauss(a, d);

		std::vector<Range<>> ranges(n);
		std::vector<Polinom> splines(n);
		for(size_t i=0; i<n; ++i){
			splines[i] = construct_spline(points[i+1], points[i], M[i+1], M[i]);
			ranges[i] = Range<>{points[i].first, points[i+1].first};
		}
		return Interpolation{splines, ranges};
	}
private:
	Polinom construct_spline(std::pair<double, double> i,
							 std::pair<double, double> i_1,
							 const double &M_i, const double &M_i_1)const{
		double x_i = i.first, x_i_1 = i_1.first,
			   y_i = i.second, y_i_1 = i_1.second,
			   h = x_i-x_i_1;
		std::vector<double> a(4);
		a[0] = (M_i*x_i_1 - M_i_1*x_i)*h/6 + (y_i_1*x_i-y_i*x_i_1)/h + (M_i_1*x_i*x_i*x_i - M_i*x_i_1*x_i_1*x_i_1)/(6*h);
		a[1] = (M_i*x_i_1*x_i_1 - M_i_1*x_i*x_i)/(2*h) + (y_i-y_i_1)/h + (M_i_1 - M_i)*h/6;
		a[2] = (M_i_1*x_i - M_i*x_i_1)/(2*h);
		a[3] = (M_i - M_i_1)/(6*h);
		return Polinom(a);
	}
};


int main() {
	/*std::ofstream fs{"points.in"};
	double q=-1.0*M_PI, dq=2.0*M_PI/10;
	for(int i=0; i<11; ++i){
		fs << q << "\t" << std::sin(q) << "\n";
		q+=dq;
	}
	fs.close();*/
	std::ifstream ifs{"points.in"};
	std::vector<std::pair<double, double>> points;
	do{
		std::pair<double, double> point;
		ifs >> point.first >> point.second;
		points.push_back(point);
	}while(!ifs.eof());
	ifs.close();
	auto interp = CubicSplineInterpolation(points);
	auto result = interp.interpolate();
	std::ofstream ofs{"points.out"};
	double x=-1.0*M_PI, dx = 2.0*M_PI/100;
	for(size_t i=0; i<100; ++i){
		ofs << x << "\t" << result(x) << "\n";
		x+=dx;
	}
	return 0;
}
