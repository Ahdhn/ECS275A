#ifndef _BOX_
#define _BOX_
#include "Primitive.h"
#include "HitRecord.h"
#include "AABBox.h"

class Box :public Primitive
{
public:
	Box(Material*mat, Point corner1, Point corner2) :
		Primitive(mat){
		Vector v1(corner1), v2(corner2);
		mAABBox += v1;
		mAABBox += v2;
	}
	

	virtual void preporcess(){}
	virtual void intersect(HitRecord&hit, const RenderContext&context, const Ray&ray)const
	{
		double tmin, tmax;
		
		bool something = mAABBox.intersect1(ray, tmin, tmax);

		if (something){
			hit.hit(tmin, this, matl);
			hit.hit(tmax, this, matl);
		}		
	}
	virtual void normal(Vector& normal, const RenderContext& context, const Point& hitpos, const Ray& ray, const HitRecord& hit) const
	{
		
	}
	virtual void move(double dist){};
	virtual void getBounds(BoundingBox& bbox) const{};

	virtual void getAABBox(AABBox& box) const
	{
		box = mAABBox;
	}

private:
	AABBox mAABBox;
};

#endif