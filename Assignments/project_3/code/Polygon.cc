#include "Polygon.h"

#include <vector>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <algorithm>

#include "Ray.h"
#include "HitRecord.h"
#include "RenderContext.h"
#include "BoundingBox.h"

using namespace std;

Polygon::Polygon(Material* material, const vector<Point>& point_list)
	             : Primitive(material),
	               point_list(point_list){
	assert(point_list.size() >= 3);

	// normal
	Vector v1 = point_list[2] - point_list[1];
	Vector v2 = point_list[0] - point_list[1];
	n = Cross(v1, v2);
	n.normalize();
	// area
	Point o(0.0, 0.0, 0.0);
	a = 0.0;
	int num = point_list.size();
	for (int i = 0; i < num; ++i) {
		Vector t = Cross(point_list[i] - o, point_list[(i + 1) % num] - o);
		a += Dot(n, t);
	}
	a = abs(a) * 0.5;
	assert(a > -10E-8);

}
Polygon::Polygon(Material* material, bool is_luminous, int sampling_freq, const std::vector<Point>& point_list, Vector direction, double speed)
	            :Primitive(material, is_luminous, sampling_freq),
				sampling_freq(sampling_freq),
				is_luminous(is_luminous),
                point_list(point_list),
				init_point_list(point_list),
				direction(direction),
				speed(speed)
{
	assert(point_list.size() >= 3);
	// normal
	Vector v1 = point_list[2] - point_list[1];
	Vector v2 = point_list[0] - point_list[1];
	n = Cross(v1, v2);
	n.normalize();
	// area
	Point o(0.0, 0.0, 0.0);
	a = 0.0;
	int num = point_list.size();
	for (int i = 0; i < num; ++i) {
		Vector t = Cross(point_list[i] - o, point_list[(i + 1) % num] - o);
		a += Dot(n, t);
	}
	a = abs(a) * 0.5;
	assert(a > -10E-8);
}
Polygon::~Polygon()
{

}

void Polygon::getBounds(BoundingBox& bbox) const 
{
	
	for (size_t i = 0; i < point_list.size(); ++i) {
		bbox.extend(point_list[i]);		
	}	
}

void Polygon::intersect(HitRecord& hit, const RenderContext& context,  const Ray& ray) const 
{
  if (Dot(ray.direction(), n) > -1e-10)  // >= 0
    return;
  double t = Dot(point_list[0] - ray.origin(), n) / Dot(ray.direction(), n);
  if (t < 0.0)
    return;
  // get the hit poing on the plane containing the polygon
  Point hitpos = ray.origin() + ray.direction() * t;
  bool inside = true;
  for (size_t i = 0; i < point_list.size(); ++i) {
    Vector v1 = point_list[(i + 1) % point_list.size()] - point_list[i];
    Vector v2 = hitpos - point_list[i];
    if (abs(v2.length2()) < 1e-20)  // ==0, on vertex
      break;
    Vector v = Cross(v1, v2);
    if (abs(v.length2()) < 1e-20)  // ==0, on edge
      break;
    if (Dot(v, n) < 0.0) {
      inside = false;
      break;
    }
  }
  if (inside)
    hit.hit(t, this, matl);
}

void Polygon::normal(Vector& normal, const RenderContext& context,
                     const Point& hitpos, const Ray& ray,
                     const HitRecord& hit) const 
{
  normal = n;
}
void Polygon::move(double dist) 
{
	for (size_t i = 0; i < point_list.size(); i++){
		point_list[i] = init_point_list[i] + direction * speed * dist; 
	}
}

void Polygon::Samples(std::vector<Vector>&rays, const RenderContext&context, const Point&hitpos)const
{
	rays.clear();
	//can not work anything but quads 
	assert(point_list.size() == 4);

	if (point_list.size() == 4){
		Vector u = point_list[0] - point_list[1];
		Vector v = point_list[2] - point_list[1];

		if (((point_list[1] + (u + v)) - point_list[3]).length2() < 1e-20){
			Vector du = u / double(sampling_freq);
			Vector dv = v / double(sampling_freq);
			for (int i = 0; i < sampling_freq; i++){
				for (int j = 0; j < sampling_freq; j++){
					Point s = point_list[1] + du*(double(i) + (double(rand()) / double(RAND_MAX))) + dv*(double(j) + (double(rand()) / double(RAND_MAX)));
					rays.push_back(s - hitpos);
				}
			}			
		}
	}

}
double Polygon::getArea() const
{
	return a;
}