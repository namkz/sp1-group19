// - CODE BY: Nam Kai Zhe
// - PROTECTED BY COPYLEFT
#include "vect2.h"

vect2::vect2(double dXIn, double dYIn)
{
	dX = dXIn; dY = dYIn;
}

vect2::vect2(COORD c)
{
	dX = c.X; dY = c.Y;
}

vect2::vect2()
{
	dX = 0; dY = 0;
}

double angleBetweenVectors(double dAX, double dAY, double dBX, double dBY)
{
	return acos((dAX * dBX + dAY * dBY)/(sqrt((dAX*dAX + dAY*dAY)*(dBX*dBX + dBY*dBY))));
}

double angleBetweenVectors(vect2 vA, vect2 vB)
{
	return acos(dot(vA, vB) / vA.length() * vB.length());
}

double dot(vect2 vA, vect2 vB)
{
	return vA.dX * vB.dX + vA.dY * vB.dY;
}

vect2 operator*(int factor, vect2 vFactor)
{
	return vFactor * factor;
}

double dot(double dAX, double dAY, double dBX, double dBY)
{
	return (dAX * dBX + dAY * dBY);
}

vect2 vect2::operator+(vect2 addend)
{
	return vect2 {dX + addend.dX, dY+addend.dY};
}

vect2 vect2::operator-(vect2 subtrahend)
{
	return vect2{dX - subtrahend.dX, dY - subtrahend.dY};
}

vect2 vect2::operator*(double factor)
{
	return vect2{dX * factor, dY * factor};
}

vect2 operator*(double factor, vect2 vector)
{
	return vect2{vector.dX * factor, vector.dY * factor};
}

vect2 vect2::operator/(double factor)
{
	return vect2{dX / factor, dY / factor};
}

double vect2::dot(vect2 multiplicand)
{
	return dX * multiplicand.dX + dY * multiplicand.dY;
}

double length(vect2 v)
{
	return sqrt(v.dX * v.dX + v.dY * v.dY);
}

double vect2::length()
{
	return sqrt(dX * dX + dY * dY);
}

double lengthsq(vect2 v)
{
	return v.dX * v.dX + v.dY * v.dY;
}

double vect2::lengthsq()
{
	return dX * dX + dY * dY;
}
