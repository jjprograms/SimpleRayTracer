#ifndef MATRIX_H
#define MATRIX_H

#include "vec3.h"

class matrix
{
	public:
		matrix();
		double data[16];
		//matrix & operator*=(matrix & m2);
		matrix inverse();
		static matrix translate(double x, double y, double z);
		static matrix rotate(vec3 axis, double angle);
		static matrix scale(double x, double y, double z);
	
};

matrix operator*(const matrix & m1, const matrix & m2);
vec3 operator*(const matrix & m, const vec3 & v);

#endif