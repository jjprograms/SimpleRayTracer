#ifndef BOUND_H
#define BOUND_H

#include "vec3.h"

class boundingbox
{
	public:
		double xlow;
		double xhigh;
		double ylow;
		double yhigh;
		double zlow;
		double zhigh;
	
		boundingbox(double xlow, double xhigh, double ylow, double yhigh, double zlow, double zhigh);
		
		bool hit(vec3 origin, vec3 target);
		
	private:
		enum plane {x,y,z};

		double intercept(double u1, double v1, double u2, double v2, double interceptpos);
		
		bool checkbound(double val, plane checkplane);
};

#endif