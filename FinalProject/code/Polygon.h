#ifndef Polygon_h
#define Polygon_h

#include "Primitive.h"
#include "Vector.h"
#include "AABBox.h"
#include <vector>

class Ray;

class Polygon : public Primitive {
public:
	Polygon(Material* material, bool is_luminous, const std::vector<Point>& point_list);
	Polygon(Material* material, bool is_luminous, const std::vector<Point>& point_list, Vector direction, double speed);
	virtual ~Polygon();
		
	virtual void preprocess(){};
	virtual void getBounds(BoundingBox& bbox) const;
	virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	virtual void normal(Vector& normal, const RenderContext& context, const Point& hitpos, const Ray& ray, const HitRecord& hit) const;	
	virtual void move(double dist);
	virtual void getAABBox(AABBox &box)const{
		box = mAABBox;
	}
private:
	Polygon(const Polygon&);
	Polygon& operator=(const Polygon&);
	std::vector<Point> point_list;	
	std::vector<Point> init_point_list;
	Vector direction;
	double speed;
	Vector n;
	double a;
	AABBox mAABBox;

};

#endif
