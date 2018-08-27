#ifndef _VECT2_H
#define _VECT2_H
#define _CLOSERTHAN(POS1, POS2, DISTANCE) (((POS1.X - POS2.X)*(POS1.X - POS2.X)) + ((POS1.Y - POS2.Y)*(POS1.Y - POS2.Y)) < DISTANCE * DISTANCE)

#include "Framework\console.h"

class vect2
{
	public:
		double dX, dY;

		vect2(double dX, double dY);
		vect2(COORD c);
		vect2();
		operator COORD() const {return COORD{SHORT(dX), SHORT(dY)};}
		vect2 operator+(vect2 addend);
		vect2 operator-(vect2 subtrahend);
		vect2 operator*(double factor);
		vect2 operator/(double divisor);
		double dot(vect2 multiplicand);
		double length();
		double lengthsq();
};

vect2 operator*(int factor, vect2 vFactor);


double dot(double dAX, double dAY, double dBX, double dBY);
double angleBetweenVectors(double dAX, double dAY, double dBX, double dBY);
double dot(vect2 vA, vect2 vB);
double angleBetweenVectors(vect2 vA, vect2 vB);
double dist(double dFromX, double dFromY, double dToX, double dToY);
double length(vect2 v);
double lengthsq(vect2 v);
#endif