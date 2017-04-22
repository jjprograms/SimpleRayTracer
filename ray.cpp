#include <iostream>
#include <math.h>
#include "vec3.h"
#include "bmp.h"
using namespace std;

class boundingbox
{
	public:
		double xlow;
		double xhigh;
		double ylow;
		double yhigh;
		double zlow;
		double zhigh;
	
		boundingbox(double xlow, double xhigh, double ylow, double yhigh, double zlow, double zhigh)
		{
			if (xlow <= xhigh)
			{
				this->xlow  = xlow;
				this->xhigh = xhigh;
			}
			else
			{
				this->xlow  = xhigh;
				this->xhigh = xlow;
			}
			
			if (ylow <= yhigh)
			{
				this->ylow  = ylow;
				this->yhigh = yhigh;
			}
			else
			{
				this->ylow  = yhigh;
				this->yhigh = ylow;
			}
			
			if (zlow <= zhigh)
			{
				this->zlow  = zlow;
				this->zhigh = zhigh;
			}
			else
			{
				this->zlow  = zhigh;
				this->zhigh = zlow;
			}
		}
		
		bool hit(vec3 origin, vec3 target)
		{
			//xbounds
			if (origin[0] != target[0])
			{
				//check for hit on plane at xlow
				bool yrange = checkbound(intercept(origin[0], origin[1], target[0], target[1], xlow), y);
				bool zrange = checkbound(intercept(origin[0], origin[2], target[0], target[2], xlow), z);
				
				if (yrange & zrange)
				{
					return true;
				}
			}
			
			
			//ybounds
			
			
			
			//zbounds
			
			return false;
		}
		
	private:
		enum plane {x,y,z};
		
		/*bool planeintercept(vec3 origin, vec3 target, plane iplane, double planecoord, vec3 * out)
		{
			
		}*/

		double intercept(double u1, double v1, double u2, double v2, double interceptpos)
		{
			return ((v1-v2)/(u1-u2)*(interceptpos-u2))+v2;
		}
		
		bool checkbound(double val, plane checkplane)
		{
			switch (checkplane)
			{
				case x:
					if (val >= xlow & val < xhigh) {return true;}
					else {return false;}
					
				case y:
					if (val >= ylow & val < yhigh) {return true;}
					else {return false;}
					
				case z:
					if (val >= zlow & val < zhigh) {return true;}
					else {return false;}
					
			}
		}
};

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
		int width = 200;
		int height = 200;
		
		char * img = new char [width*height*3];
		
		boundingbox * box = new boundingbox(-1,1,-1,1,-1,1);
		
		vec3 origin;
		origin[0] = 0;
		origin[1] = 0;
		origin[2] = -5;
		
		vec3 target;
		target[0] = 0;
		target[1] = 0;
		target[2] = 0;
		
		double xrange = PI/4;
		double yrange = height/width*xrange;
		
		vec3 y;
		y[0] = 0;
		y[1] = 1;
		y[2] = 0;
		
		vec3 camright = unit_vector(cross((target - origin), y));
		vec3 camup = unit_vector(cross((target - origin), camright));
		
		int i, j;
		
		vec3 ray;
		
		int r, g, b;
		
		for (j = 0; j < height; j++)
		{
			for (i = 0; i < width; i++)
			{
				ray = target - origin;
				ray = rotatevec(ray, camup, (2*xrange*i)/width-xrange);
				ray = rotatevec(ray, camright, (2*yrange*j)/height-yrange);
				
				r = g = b = 0;
				
				if (box->hit(origin, ray))
				{
					r = g = b = 255;
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