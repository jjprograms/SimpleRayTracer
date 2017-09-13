#include <iostream>
#include <math.h>
#include <random>
#include <vector>
#include "vec3.h"
#include "bmp.h"
#include "boundingbox.h"
#include "matrix.h"
using namespace std;

const double PI = 3.14159265358979323846;

class sphere;

struct hit
{
    bool wasRecorded;
    vec3 point;
    vec3 normal;
	//sphere * object;
	int sphereindex;
};

hit closesthit(vec3 origin, hit hit1, hit hit2)
{
    double d1 = (hit1.point - origin).length();
    double d2 = (hit2.point - origin).length();

    if (d2 < d1)
    {
        return hit2;
    }
    else
    {
        return hit1;
    }
}

class material
{
    public:
        vec3 diffuse;
        vec3 specular;
        vec3 emission;

		material()
		{
			diffuse  = vec3(0, 0, 0);
			specular = vec3(0, 0, 0);
			emission = vec3(0, 0, 0);
		}
};

/*class diffuse_material: public material
{
    public:
        ray bounce_ray(ray incoming_ray, vec3 normal)
        {
            ray out;
        }
};*/

double cooktorr(vec3 V, vec3 L, vec3 N, double Ri1, double Ri2, double roughness)
{
    vec3 H = unit_vector((V+L)/2);
    
    // D:
    
    /*double a = acos(dot(N, H));
    
    double cos2a = cos(a) * cos(a);*/
    
    double NdotH = dot(N, H);
    
    double cos2a = NdotH * NdotH;
    
    double m2 = roughness * roughness;
    
    double D = exp( (1 - cos2a) / (cos2a * m2) ) / ( PI * m2 * cos2a * cos2a );
    
    // F:
    
    double cosiangle = dot(V, N);
    double siniangle = sqrt(1 - cosiangle * cosiangle);
    
    double radical = sqrt( 1 - pow( siniangle * Ri1 / Ri2, 2) );
    
    double Rs = pow( (Ri1*cosiangle - Ri2*radical) / (Ri1*cosiangle + Ri2*radical), 2 );
    double Rp = pow( (Ri1*radical - Ri2*cosiangle) / (Ri1*radical + Ri2*cosiangle), 2 );
    
    double F = (Rs + Rp) / 2;
    
    //G:
    
    double VdotH = dot(V, H);
    
    double VdotN = dot(V, N);
    
    double LdotN = dot(L, N);
    
    double G = fmin(1, 2 * NdotH * fmin(VdotN, LdotN) / VdotH);
    
    //Cook-Torr
    
    return D * F * G / (4 * VdotN * LdotN);
}

class ray
{
    public:
        vec3 origin;
        vec3 direction;

        ray() 
        {
            origin = vec3(0, 0, 0);
            direction = vec3(1, 0, 0);
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
        boundingbox bbox = boundingbox(-1,1,-1,1,-1,1);
        vec3 position;
        double radius;
        material material;
        sphere(vec3 position, double radius)
        {
            this->position = position;
            this->radius = radius;

            bbox.xhigh = position.x() + radius;
            bbox.xlow = position.x() - radius;

            bbox.yhigh = position.y() + radius;
            bbox.ylow = position.y() - radius;

            bbox.zhigh = position.z() + radius;
            bbox.zlow = position.z() - radius;
        }

		sphere()
		{
			this->position = vec3(-500, -500, -500);
			this->radius = 70;
		}

        hit intersection(ray ray)
        {
            vec3 a = ray.origin - position;     //O-C
            double b = dot(a, ray.direction);   //L.(O-C)
            double descriminator = b*b - a.squared_length() + radius*radius;

            hit hit1;

			//hit1.object = this;

            hit1.wasRecorded = false;
            
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

            if (descriminator <= 0)
            {
                return hit1;
            }

            else//if (descriminator > 0)
            {
                double sqdes = sqrt(descriminator);
                double d1 = -b + sqdes;
                double d2 = -b - sqdes;

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

double get_random()
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<double> dis(0, 1); // range 0 - 1
    return dis(e);
}

vec3 random_unit_vector()
{
	static std::default_random_engine e;
	static std::normal_distribution<double> generate(0.0, 1.0);
	
	double x = generate(e);
	double y = generate(e);
	double z = generate(e);
	
	return unit_vector(vec3(x, y, z));
}

vec3 trace(const vector<sphere>& scene, ray camera_ray, int maxbounce)
{
    if (maxbounce > 0)
    {
		vector<hit> hitlist;

		sphere sceneitem;

		hit currhit;

		for (int m = 0; m < scene.size(); m++)
		{
			sceneitem = scene[m];
			if (sceneitem.bbox.hit(camera_ray.origin, camera_ray.origin + camera_ray.direction))
			{
				int numhits = hitlist.size();
		
				currhit = sceneitem.intersection(camera_ray);

				if (currhit.wasRecorded)
				{
					currhit.sphereindex = m;
					hitlist.push_back(currhit);
				}
			}
		}

		hit besthit;

		switch (hitlist.size())
		{
			case 0:
				return vec3(.5, .5, .5);
				break;

			case 1:
				besthit = hitlist[0];
				break;

			default:
				besthit = hitlist[0];
				for (int i = 1; i < hitlist.size(); i++)
				{
					besthit = closesthit(camera_ray.origin, besthit, hitlist[i]);
				}
				break;
		}
		
		vec3 random_direction = random_unit_vector();
		
		if (dot(besthit.normal, random_direction) < 0)
		{
			random_direction = -random_direction;
		}
		
		ray newray = ray(besthit.point + (besthit.normal * .001), random_direction);

		vec3 emission = scene[besthit.sphereindex].material.emission;
		vec3 diffuse  = scene[besthit.sphereindex].material.diffuse;

		return emission + (diffuse * trace(scene, newray, maxbounce - 1) * dot(besthit.normal, newray.direction));//+ trace(scene, newray, maxbounce - 1);// 
    }
	
	return vec3(0, 0, 0);
}

vec3 clamp(double lowlimit, double highlimit, vec3 vect)
{
	vect[0] = fmax(vect[0], lowlimit);
	vect[1] = fmax(vect[1], lowlimit);
	vect[2] = fmax(vect[2], lowlimit);

	vect[0] = fmin(vect[0], highlimit);
	vect[1] = fmin(vect[1], highlimit);
	vect[2] = fmin(vect[2], highlimit);

	return vect;
}

int main(int argc, char* argv[])
{
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
		
		vec3 * prescale_render   = new vec3 [width*height];

		int a, b;
		for (b = 0; b < height; b++)
		{
			for (a = 0; a < width; a++)
			{
				prescale_render[width*b + a] = vec3(0, 0, 0);
			}
		}
        
        vector<sphere> scene;

		sphere s1 = sphere(vec3(-4, 0, 0), .5);
		sphere s2 = sphere(vec3(-2, 0, 0), .5);
		sphere s3 = sphere(vec3(0, 0, .5), .5);
		sphere s4 = sphere(vec3(2, 0, 0), .5);

		s1.material.emission = vec3(1,0,0)*10;
		s1.material.diffuse = vec3(1,1,1);

		s2.material.diffuse = vec3(1,1,1);
		s3.material.diffuse = vec3(1,1,1);
		s4.material.emission = vec3(1,1,1)*10;
		s4.material.diffuse = vec3(1,1,1);

        scene.push_back(s1);
        scene.push_back(s2);
        scene.push_back(s3);
        scene.push_back(s4);
		
		vec3 origin = vec3(-5, 0, -1.6);
        
		vec3 target = vec3(0, 0, 0);
        
        double xrange = PI/4;
        double yrange = xrange*height/width;
        
        vec3 y = vec3(0,1,0);
        
        vec3 camright = unit_vector(cross((target - origin), y));
        vec3 camup = unit_vector(cross((target - origin), camright));
        
        int i, j, k, l;
        
        vec3 camray;
        
		int samples_per_pixel, max_depth;
        samples_per_pixel = 100;// 25;
        max_depth = 2;
        
        for (int k = 0; k < samples_per_pixel; k++)
        {
            for (j = 0; j < height; j++)
            {
                matrix tilt = matrix::rotate(camright, (2 * yrange*j) / height - yrange);

                for (i = 0; i < width; i++)
                {
                    camray = target - origin;

                    matrix pan = matrix::rotate(camup, (2 * xrange*i) / width - xrange);

					prescale_render[width*j + i] += trace(scene, ray(origin, unit_vector(pan * tilt * camray)), max_depth);

                }
            }
        }
		
		int p,q;
        
		for (q = 0; q < height; q++)
		{
			for (p = 0; p < width; p++)
			{
				vec3 pixel = prescale_render[width*q + p] / samples_per_pixel;

				vec3 cpixel = clamp(0, 1, pixel) * 255;
				
				img[(width*q + p) * 3]     = (char)cpixel.b();
				img[(width*q + p) * 3 + 1] = (char)cpixel.g();
				img[(width*q + p) * 3 + 2] = (char)cpixel.r();
			}
		}

        saveimage("test.bmp", img, width, height);
    }
    
    return 0;
}


//create each object set it's properties and add it to a list

//create rays acording to camera parameters and settings like samples per pixel

//for each ray test each object to find closest hit if any

//if we hit something keep tracing rays until we reach max depth

