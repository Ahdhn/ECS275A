#ifndef Triangle_h
#define Triangle_h

#include "Primitive.h"
#include "Vector.h"



class Triangle : public Primitive { 

public:

	Triangle(Material* mat, Point& p1, Point& p2, Point& p3);
	Triangle(Material* mat, Point& p1, Point& p2, Point& p3, Vector direction, double speed);
	virtual~Triangle();
		
	virtual void preprocess(){};
	virtual void getBounds(BoundingBox& bbox) const;
	virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	virtual void normal(Vector& normal, const RenderContext& context,
		                const Point & hitpos, const Ray& ray, const HitRecord& hit) const;
	virtual void move(double dist);

protected:
	Point mP1, mP2, mP3;
	Point init_mP1, init_mP2, init_mP3;
	Vector mE1;
	Vector mE2;
	Vector mNormal;
	Vector direction;
	double speed;
};

#endif