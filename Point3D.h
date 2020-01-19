#ifndef POINT3D_H_
#define POINT3D_H_

class Point3D
{
private:
	double x;
	double y;
	double z;
public:
	Point3D(double = 0, double = 0, double = 0);

	double getX() const;
	double getY() const;
	double getZ() const;

	double cost(Point3D) const;
	double distance(Point3D) const;
};

#endif /* POINT3D_H_ */
