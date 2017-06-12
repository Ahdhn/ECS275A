#ifndef Triangle_h
#define Triangle_h

#include "Primitive.h"
#include "Vector.h"
#include "AABBox.h"


class Triangle : public Primitive { 

public:

	Triangle(Material* mat, Point& p1, Point& p2, Point& p3);
	Triangle(Material* mat, Point& p1, Point& p2, Point& p3, Vector direction, double speed);

	Triangle(Material* mat, Point p1, Point p2, Point p3, Point n1, Point n2, Point n3) :
		Primitive(mat), mP1(p1), mP2(p2), mP3(p3), mN1(n1), mN2(n2), mN3(n3), mVertNormals(true) {  }
	virtual~Triangle();
		
	virtual void preprocess(){};
	virtual void getBounds(BoundingBox& bbox) const;
	virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	virtual void normal(Vector& normal, const RenderContext& context,
		                const Point & hitpos, const Ray& ray, const HitRecord& hit) const;
	virtual void move(double dist);
	virtual void getAABBox(AABBox& box) const
	{
		box = mAABBox;
	}
protected:
	Point mP1, mP2, mP3;
	Point init_mP1, init_mP2, init_mP3;
	Point mN1, mN2, mN3;//they should be vectors 
	Vector mE1, mE2, mNormal;
	bool mVertNormals;
	Vector direction;
	AABBox mAABBox;
	double speed;
};

#endif