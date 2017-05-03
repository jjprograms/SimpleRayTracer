#include "boundingbox.h"

boundingbox::boundingbox(double xlow, double xhigh, double ylow, double yhigh, double zlow, double zhigh)
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

bool boundingbox::hit(vec3 origin, vec3 target)
        {
            //xbounds
            if (origin.x() != target.x())
            {
                if ((target.x() - origin.x() > 0) != (origin.x() - xlow > 0))   //xlow must not be behind ray
                {
                    //check for hit on plane at xlow
                    bool yrange = checkbound(intercept(origin.x(), origin.y(), target.x(), target.y(), xlow), y);
                    bool zrange = checkbound(intercept(origin.x(), origin.z(), target.x(), target.z(), xlow), z);

                    if (yrange && zrange)
                    {
                        return true;
                    }
                }

                if ((target.x() - origin.x() > 0) != (origin.x() - xhigh > 0))  //xhigh must not be behind ray
                {
                    //check for hit on plane at xhigh
                    bool yrange = checkbound(intercept(origin.x(), origin.y(), target.x(), target.y(), xhigh), y);
                    bool zrange = checkbound(intercept(origin.x(), origin.z(), target.x(), target.z(), xhigh), z);

                    if (yrange && zrange)
                    {
                        return true;
                    }
                }
            }
            
            
            //ybounds
            if (origin.y() != target.y())
            {
                if ((target.y() - origin.y() > 0) != (origin.y() - ylow > 0))   //ylow must not be behind ray
                {
                    //check for hit on plane at ylow
                    bool xrange = checkbound(intercept(origin.y(), origin.x(), target.y(), target.x(), ylow), x);
                    bool zrange = checkbound(intercept(origin.y(), origin.z(), target.y(), target.z(), ylow), z);

                    if (xrange && zrange)
                    {
                        return true;
                    }
                }

                if ((target.y() - origin.y() > 0) != (origin.y() - yhigh > 0))  //yhigh must not be behind ray
                {
                    //check for hit on plane at yhigh
                    bool xrange = checkbound(intercept(origin.y(), origin.x(), target.y(), target.x(), yhigh), x);
                    bool zrange = checkbound(intercept(origin.y(), origin.z(), target.y(), target.z(), yhigh), z);

                    if (xrange && zrange)
                    {
                        return true;
                    }
                }
            }

            
            //zbounds
            if (origin.z() != target.z())
            {
                if ((target.z() - origin.z() > 0) != (origin.z() - zlow > 0))   //zlow must not be behind ray
                {
                    //check for hit on plane at zlow
                    bool yrange = checkbound(intercept(origin.z(), origin.y(), target.z(), target.y(), zlow), y);
                    bool xrange = checkbound(intercept(origin.z(), origin.x(), target.z(), target.x(), zlow), x);

                    if (yrange && xrange)
                    {
                        return true;
                    }
                }

                if ((target.z() - origin.z() > 0) != (origin.z() - zhigh > 0))  //zhigh must not be behind ray
                {
                    //check for hit on plane at zhigh
                    bool yrange = checkbound(intercept(origin.z(), origin.y(), target.z(), target.y(), zhigh), y);
                    bool xrange = checkbound(intercept(origin.z(), origin.x(), target.z(), target.x(), zhigh), x);

                    if (yrange && xrange)
                    {
                        return true;
                    }
                }
            }


            return false;
        }
        
double boundingbox::intercept(double u1, double v1, double u2, double v2, double interceptpos)
        {
            return ((v1-v2)/(u1-u2)*(interceptpos-u2))+v2;
        }
        
bool boundingbox::checkbound(double val, plane checkplane)
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
