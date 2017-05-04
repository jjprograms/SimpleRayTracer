#include <iostream>
#include <math.h>
#include "vec3.h"
#include "bmp.h"
#include "boundingbox.h"
#include "matrix.h"
using namespace std;

class mat
{
	public:
		double m[9];
		
		vec3 mult(vec3 v)
		{
			vec3 out;
			out.e[0] = v[0]*m[0] + v[1]*m[1] + v[2]*m[2];
			out.e[1] = v[0]*m[3] + v[1]*m[4] + v[2]*m[5];
			out.e[2] = v[0]*m[6] + v[1]*m[7] + v[2]*m[8];
			return out;
		}
	
		static mat fromaa(vec3 axis, double angle)
		{
			double c = cos(angle);
			double s = sin(angle);
			double p = 1 - c;
			double x = axis[0];
			double y = axis[1];
			double z = axis[2];
			
			mat m;
			m.m[0] = x*x*p + c;
			m.m[1] = x*y*p - z*s;
			m.m[2] = x*z*p + y*s;
			
			m.m[3] = y*x*c + z*s;
			m.m[4] = y*y*p + c;
			m.m[5] = y*z*p - x*s;
			
			m.m[6] = z*x*p - y*s;
			m.m[7] = z*y*p + x*s;
			m.m[8] = z*z*p + c;
			return m;
		}
};

struct hit
{
	bool wasRecorded;
	vec3 point;
	vec3 normal;
};

class material
{
	vec3 reflectance;

};

class ray
{
	public:
		vec3 origin;
		vec3 direction;

		ray() 
		{
			origin = *(new vec3(0, 0, 0));
			direction = *(new vec3(1, 0, 0));
		}

		ray(vec3 origin, vec3 direction)
		{
			this->direction = direction;
			this->origin = origin;
		}
};

class sphere
{
	public:
		vec3 position;
		double radius;
		//material material;
		sphere(vec3 position, double radius)
		{
			this->position = position;
			this->radius = radius;
		}

		hit intersection(ray ray)
		{
			vec3 a = ray.origin - position;		//O-C
			float b = dot(a, ray.direction);	//L.(O-C)
			float descriminator = b*b - a.squared_length() + radius*radius;

			hit hit1;

			hit1.wasRecorded = false;
			
			if (descriminator <= 0)
			{
				return hit1;
			}

			/*if (descriminator == 0)
			{
				if (-b > 0)
				{
					hit.hit = true;
					hit.point = ray.direction*(-b) + ray.origin;
					hit.normal = unit_vector(hit.point - position);
					return hit;
				}
				else
				{
					hit.hit = false;
					return hit;
				}
			}*/

			if (descriminator > 0)
			{
				float sqdes = sqrt(descriminator);
				float d1 = -b + sqdes;
				float d2 = -b - sqdes;

				if (-b <= 0)
				{
					if (d1 > 0)
					{
						hit1.wasRecorded = true;
						hit1.point = ray.direction*d1 + ray.origin;
						hit1.normal = hit1.point - position;

						return hit1;
					}

					return hit1;
				}

				if (d2 > 0)
				{
					hit1.wasRecorded = true;
					hit1.point = ray.direction*d2 + ray.origin;
					hit1.normal = hit1.point - position;

					return hit1;
				}

				hit1.wasRecorded = true;
				hit1.point = ray.direction*d1 + ray.origin;
				hit1.normal = hit1.point - position;

				return hit1;
			}
		}
};

vec3 rotatevec(vec3 vector, vec3 rotation, double angle)
{
	mat m = mat::fromaa(rotation, angle);
	return m.mult(vector);
}

int main(int argc, char* argv[])
{
	const double PI = 3.14159265358979323846;

	if(argc > 1)
	{
		//attempt to parse input file
	}
	else
	{
		//run test file
		int width = 300;
		int height = 200;
		
		char * img = new char [width*height*3];
		
		boundingbox * box = new boundingbox(-1,1,-1,1,-1,1);

		sphere sphere1 = *(new sphere(vec3(0, 0, 0), 1));
		
		vec3 origin;
		origin[0] = 0;
		origin[1] = 2;
		origin[2] = -5;
		
		vec3 target;
		target[0] = 0;
		target[1] = 0;
		target[2] = 0;
		
		double xrange = PI/4;
		double yrange = xrange*height/width;
		
		vec3 y;
		y[0] = 0;
		y[1] = 1;
		y[2] = 0;
		
		vec3 camright = unit_vector(cross((target - origin), y));
		vec3 camup = unit_vector(cross((target - origin), camright));
		
		int i, j;
		
		vec3 camray;
		
		int r, g, b;
		
		for (j = 0; j < height; j++)
		{
			matrix tilt = matrix::rotate(camright, (2*yrange*j)/height-yrange);
			
			for (i = 0; i < width; i++)
			{
				camray = target - origin;
				
				matrix pan = matrix::rotate(camup, (2*xrange*i)/width-xrange);
				
				r = g = b = 0;
				
				if (box->hit(origin, camray))
				{
					hit hit;
					hit = sphere1.intersection(*(new ray(origin, unit_vector(pan * tilt * camray))));

					if (hit.wasRecorded)
					{
						//r = g = b = 255;
						camray = unit_vector(camray);
						r = (int)((camray.r() + 1) * 127.5);
						g = (int)((camray.g() + 1) * 127.5);
						b = (int)((camray.b() + 1) * 127.5);
					}
				}
				
				img[(width*j+i)*3] = r;
				img[(width*j+i)*3+1] = g;
				img[(width*j+i)*3+2] = b;
			}
		}
		
		saveimage("test.bmp", img, width, height);
	}
	
	return 0;
}