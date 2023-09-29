#ifndef GEOMETRY2D_H_
#define GEOMETRY2D_H_

#include <malloc.h>
#include <math.h>

typedef struct Coordinates{
public:
	double x;
	double y;
};

Coordinates* coordinatesFactory(const double &x, const double &y);
const double distance(const Coordinates &p1, const Coordinates &p2);

typedef struct Circle{
public:
	Coordinates *position;
	double radius;
};

Circle* circleFactory(const Coordinates &position, const double &radius=1.0);
bool isColliding(const Circle &c1, const Circle &c2);

#endif /* GEOMETRY2D_H_ */
