#include "Geometry2D.h"

Coordinates* coordinatesFactory(const double &x, const double &y){
	Coordinates* coor = malloc(sizeof(double)*2);
	coor->x = x;
	coor->y = y;
	return coor;
}

const double distance(const Coordinates &p1, const Coordinates &p2){return (((p1.x-p2.x)*(p1.x-p2.x))+((p1.y - p2.y)*(p1.y - p2.y)));}

Circle* circleFactory(const Coordinates &position, const double &radius){
	Circle* cir = malloc(sizeof(double) + sizeof(Coordinates*));
	cir->radius = radius;
	cir->position = position;
	return cir;
}

bool isColliding(const Circle &c1, const Circle &c2){return distance(*c1.position, *c2.position) < c1.radius + c2.radius;}
