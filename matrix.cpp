#include "matrix.h"
#include <math.h>

matrix::matrix()
{
	int i;
	for (i = 0; i < 16; i++)
	{
		data[i] = 0;
	}
}

matrix operator*(const matrix & m1, const matrix & m2)
{
    matrix m3;
    int i;
    int j;
    
    for (j = 0; j < 4; j++)
    {
        for (i = 0; i < 4; i++)
        {
            m3.data[j*4+i] += m1.data[j*4]*m2.data[i];
            m3.data[j*4+i] += m1.data[j*4+1]*m2.data[4+i];
            m3.data[j*4+i] += m1.data[j*4+2]*m2.data[8+i];
            m3.data[j*4+i] += m1.data[j*4+3]*m2.data[12+i];
        }
    }
    return m3;
}

vec3 operator*(const matrix & m, const vec3 & v)
{
	vec3 out;
	out.e[0] = v[0]*m.data[0]  + v[1]*m.data[1]  + v[2]*m.data[2]  + v[3]*m.data[3];
	out.e[1] = v[0]*m.data[4]  + v[1]*m.data[5]  + v[2]*m.data[6]  + v[3]*m.data[7];
	out.e[2] = v[0]*m.data[8]  + v[1]*m.data[9]  + v[2]*m.data[10] + v[3]*m.data[11];
	out.e[3] = v[0]*m.data[12] + v[1]*m.data[13] + v[2]*m.data[14] + v[3]*m.data[15];
	return out;
}

matrix matrix::inverse()
{
	// copied from pov ray
	
	double d00, d01, d02, d03;
	double d10, d11, d12, d13;
	double d20, d21, d22, d23;
	double d30, d31, d32, d33;
	double m00, m01, m02, m03;
	double m10, m11, m12, m13;
	double m20, m21, m22, m23;
	double m30, m31, m32, m33;
	double D;

	m00 = this->data[ 0];  m01 = this->data[ 1];  m02 = this->data[ 2];  m03 = this->data[ 3];
	m10 = this->data[ 4];  m11 = this->data[ 5];  m12 = this->data[ 6];  m13 = this->data[ 7];
	m20 = this->data[ 8];  m21 = this->data[ 9];  m22 = this->data[10];  m23 = this->data[11];
	m30 = this->data[12];  m31 = this->data[13];  m32 = this->data[14];  m33 = this->data[15];

	d00 = m11*m22*m33 + m12*m23*m31 + m13*m21*m32 - m31*m22*m13 - m32*m23*m11 - m33*m21*m12;
	d01 = m10*m22*m33 + m12*m23*m30 + m13*m20*m32 - m30*m22*m13 - m32*m23*m10 - m33*m20*m12;
	d02 = m10*m21*m33 + m11*m23*m30 + m13*m20*m31 - m30*m21*m13 - m31*m23*m10 - m33*m20*m11;
	d03 = m10*m21*m32 + m11*m22*m30 + m12*m20*m31 - m30*m21*m12 - m31*m22*m10 - m32*m20*m11;

	d10 = m01*m22*m33 + m02*m23*m31 + m03*m21*m32 - m31*m22*m03 - m32*m23*m01 - m33*m21*m02;
	d11 = m00*m22*m33 + m02*m23*m30 + m03*m20*m32 - m30*m22*m03 - m32*m23*m00 - m33*m20*m02;
	d12 = m00*m21*m33 + m01*m23*m30 + m03*m20*m31 - m30*m21*m03 - m31*m23*m00 - m33*m20*m01;
	d13 = m00*m21*m32 + m01*m22*m30 + m02*m20*m31 - m30*m21*m02 - m31*m22*m00 - m32*m20*m01;

	d20 = m01*m12*m33 + m02*m13*m31 + m03*m11*m32 - m31*m12*m03 - m32*m13*m01 - m33*m11*m02;
	d21 = m00*m12*m33 + m02*m13*m30 + m03*m10*m32 - m30*m12*m03 - m32*m13*m00 - m33*m10*m02;
	d22 = m00*m11*m33 + m01*m13*m30 + m03*m10*m31 - m30*m11*m03 - m31*m13*m00 - m33*m10*m01;
	d23 = m00*m11*m32 + m01*m12*m30 + m02*m10*m31 - m30*m11*m02 - m31*m12*m00 - m32*m10*m01;

	d30 = m01*m12*m23 + m02*m13*m21 + m03*m11*m22 - m21*m12*m03 - m22*m13*m01 - m23*m11*m02;
	d31 = m00*m12*m23 + m02*m13*m20 + m03*m10*m22 - m20*m12*m03 - m22*m13*m00 - m23*m10*m02;
	d32 = m00*m11*m23 + m01*m13*m20 + m03*m10*m21 - m20*m11*m03 - m21*m13*m00 - m23*m10*m01;
	d33 = m00*m11*m22 + m01*m12*m20 + m02*m10*m21 - m20*m11*m02 - m21*m12*m00 - m22*m10*m01;

	D = m00*d00 - m01*d01 + m02*d02 - m03*d03;

	matrix r;

	r.data[ 0] =  d00/D; r.data[ 1] = -d10/D;  r.data[ 2] =  d20/D; r.data[ 3] = -d30/D;
	r.data[ 4] = -d01/D; r.data[ 5] =  d11/D;  r.data[ 6] = -d21/D; r.data[ 7] =  d31/D;
	r.data[ 8] =  d02/D; r.data[ 9] = -d12/D;  r.data[10] =  d22/D; r.data[11] = -d32/D;
	r.data[12] = -d03/D; r.data[13] =  d13/D;  r.data[14] = -d23/D; r.data[15] =  d33/D;
	
	return r;
}

matrix matrix::translate(double x, double y, double z)
{
    matrix m;
    
    m.data[3] = x;
    m.data[7] = y;
    m.data[11] = z;
	m.data[0] = 1;
	m.data[5] = 1;
	m.data[10] = 1;
	m.data[15] = 1;
    
    return m;
}

matrix matrix::rotate(vec3 axis, double angle)
{
	double c = cos(angle);
	double s = sin(angle);
	double p = 1 - c;
	double x = axis[0];
	double y = axis[1];
	double z = axis[2];
	
	matrix m;
	m.data[0] = x*x*p + c;
	m.data[1] = x*y*p - z*s;
	m.data[2] = x*z*p + y*s;
	
	m.data[4] = y*x*c + z*s;
	m.data[5] = y*y*p + c;
	m.data[6] = y*z*p - x*s;
	
	m.data[8] = z*x*p - y*s;
	m.data[9] = z*y*p + x*s;
	m.data[10] = z*z*p + c;
	
	m.data[15] = 1;
	return m;
}

matrix matrix::scale(double x, double y, double z)
{
	matrix m;
	m.data[0] = x;
	m.data[5] = y;
	m.data[10] = z;
	m.data[15] = 1;
	return m;
}
