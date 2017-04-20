
#include "Triangle.h"
#include "BoundingBox.h"
#include "HitRecord.h"
#include "Point.h"
#include "Ray.h"
#include "Vector.h"
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>

Triangle::Triangle(Material* mat, Point& p1, Point& p2, Point& p3)
	             : Primitive(mat), mP1(p1), mP2(p2), mP3(p3) 
{
	mE1 = mP1 - mP3;
	mE2 = mP2 - mP3;
	mNormal = Cross(mE2, mE1);
	mNormal.normalize();
}
Triangle::~Triangle()
{

}

void Triangle::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
	
	Vector r1 = Cross(ray.direction(), mE2);
	double denom = Dot(mE1, r1);

	if (fabs(denom) < 1e-6)
		return;

	double invDenom = 1.0 / denom;

	Vector s = ray.origin() - mP3;
	double b1 = Dot(s, r1) * invDenom;

	if (b1 < 0.0 || b1 > 1.0)
		return;

	Vector r2 = Cross(s, mE1);
	double b2 = Dot(ray.direction(), r2) * invDenom;

	if (b2 < 0.0 || (b1 + b2) > 1.0)
		return;

	double t = Dot(mE2, r2) * invDenom;
	hit.hit(t, this, matl);
}

void Triangle::normal(Vector& normal, const RenderContext& context,
	                  const Point & hitpos, const Ray& ray, const HitRecord& hit) const
{
	normal = mNormal;
}
void Triangle::getBounds(BoundingBox& bbox) const
{
	std::cerr << "Triangle::getBounds() called!" << std::endl;
	exit(1);
}