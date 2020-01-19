#include "Point3D.h"
#include <cmath>
using namespace std;

Point3D::Point3D(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

double Point3D::getX() const
{
	return x;
}

double Point3D::getY() const
{
	return y;
}

double Point3D::getZ() const
{
	return z;
}

double Point3D::cost(Point3D other) const
{
	return fabs(other.getX() - getX()) + fabs(other.getY() - getY()) + fabs(other.getZ() - getZ());
}

double Point3D::distance(Point3D other) const
{
	return sqrt(pow(other.getX() - getX(), 2) + pow(other.getY() - getY(), 2) + pow(other.getZ() - getZ(), 2));
}
