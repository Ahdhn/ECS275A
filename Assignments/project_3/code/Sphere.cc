
#include "Sphere.h"
#include "BoundingBox.h"
#include "HitRecord.h"
#include "Point.h"
#include "Ray.h"
#include "Vector.h"
#include <math.h>
#include <cstdlib>

Sphere::Sphere(Material* material, const Point& center, double radius)
              : Primitive(material), center(center), radius(radius)
{
  inv_radius = 1./radius;
  a = 4.0*M_PI*radius*radius;
}
Sphere::Sphere(Material* material, bool is_luminous,int sampling_freq, const Point& center, double radius, Vector direction, double speed)
	: Primitive(material, is_luminous, sampling_freq), is_luminous(is_luminous), ini_center(center), center(center), radius(radius), direction(direction), speed(speed)
{
	inv_radius = 1. / radius;
	a = 4.0*M_PI*radius*radius;
}

Sphere::~Sphere()
{
}

void Sphere::getBounds(BoundingBox& bbox) const
{
  Vector diag(radius, radius, radius);
  bbox.extend(center+diag);
  bbox.extend(center-diag);
}

void Sphere::intersect(HitRecord& hit, const RenderContext&, const Ray& ray) const
{
  Vector O(ray.origin()-center);
  const Vector& V(ray.direction());
  double b = Dot(O, V);
  double c = Dot(O, O)-radius*radius;
  double disc = b*b-c;
  if(disc > 0){
    double sdisc = sqrt(disc);
    double root1 = (-b - sdisc);
    if(!hit.hit(root1, this, matl)){
      double root2 = (-b + sdisc);
      hit.hit(root2, this, matl);
    }
  }
}

void Sphere::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                    const Ray& ray, const HitRecord& hit) const
{
  normal = (hitpos-center)*inv_radius;
}

void Sphere::move(double dist)
{
	center = ini_center + direction * speed * dist;
}
void Sphere::Samples(std::vector<Vector>&rays, const RenderContext&context, const Point&hitpos)const
{
	rays.clear();
	for (int i = 0; i < sampling_freq*sampling_freq; i++){
		double phi = 2.0*M_PI*(double(rand()) / double(RAND_MAX));
		double cos_theta = 1.0 - 2.0*(double(rand()) / double(RAND_MAX));
		double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
		Vector dir(sin_theta*cos(phi), sin_theta*sin(phi), cos_theta);
		Point p = center + dir*radius;
		if (Dot(hitpos - center, p - center) < 0.0){
			p = center + (-dir)*radius;
		}
		rays.push_back(p - hitpos);

	}

}
double Sphere::getArea()const
{
	return 0.5*a;
}