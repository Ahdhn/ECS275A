#ifndef Triangle_h
#define Triangle_h

#include "Primitive.h"
#include "Vector.h"



class Triangle : public Primitive { 

public:

	Triangle(Material* mat, Point& p1, Point& p2, Point& p3);
	virtual~Triangle();
		
	virtual void getBounds(BoundingBox& bbox) const;
	virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	virtual void normal(Vector& normal, const RenderContext& context,
		                const Point & hitpos, const Ray& ray, const HitRecord& hit) const;


protected:
	Point mP1, mP2, mP3;
	Vector mE1;
	Vector mE2;
	Vector mNormal;
};

#endif