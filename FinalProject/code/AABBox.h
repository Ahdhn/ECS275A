#ifndef _AABBOX_
#define _AABBOX_


#include "Vector.h"
#include "Ray.h"
#include <limits>
#include <algorithm>

class AABBox{
public:
	inline AABBox(){
		mCorners[0].set(std::numeric_limits<double>::infinity());
		mCorners[1].set(-std::numeric_limits<double>::infinity());
		mCenter.set(0);
	}
	inline AABBox(const Vector& minPoint, const Vector& maxPoint){
		mCorners[0] = minPoint;
		mCorners[1] = maxPoint;
		mCenter = mCorners[0] + (mCorners[1] - mCorners[0]) / 2.0;
	}

	inline AABBox(const Vector& minPoint, const Vector& maxPoint, const Vector& center) : mCenter(center){
		mCorners[0] = minPoint;
		mCorners[1] = maxPoint;
	}

	inline AABBox(const AABBox& box){
		mCorners[0] = box.mCorners[0]; mCorners[1] = box.mCorners[1]; mCenter = box.mCenter;
	}

	//operator
	inline AABBox& operator=(const AABBox& box){ 
		mCorners[0] = box.mCorners[0]; mCorners[1] = box.mCorners[1]; mCenter = box.mCenter; return (*this);
	}
	inline AABBox& operator+=(const AABBox& box) { addPoint(box[0]); addPoint(box[1]); return (*this); }
	inline AABBox& operator+=(const Vector& p) { addPoint(p); return (*this); }

	inline void addPoint(const Vector& p){
		mCorners[0].x() = std::min(mCorners[0].x(), p.x());
		mCorners[0].y() = std::min(mCorners[0].y(), p.y());
		mCorners[0].z() = std::min(mCorners[0].z(), p.z());

		mCorners[1].x() = std::max(mCorners[1].x(), p.x());
		mCorners[1].y() = std::max(mCorners[1].y(), p.y());
		mCorners[1].z() = std::max(mCorners[1].z(), p.z());

		mCenter = mCorners[0] + (mCorners[1] - mCorners[0]) / 2.0;
	}


	// Accessors
	inline Vector operator[](unsigned int i) const { return mCorners[i]; }
	inline Vector minPoint() const { return mCorners[0]; }
	inline Vector maxPoint() const { return mCorners[1]; }
	inline Vector center() const { return mCenter; }
	inline Vector corner(unsigned int i) const
	{
		
		Vector p(mCorners[(i & 0x1)].x(),
 			    mCorners[(i & 0x2) >> 1].y(),
			    mCorners[(i & 0x4) >> 2].z());
		return p;
	}

	// Intersect
	inline bool intersect1(const Ray& ray) const
	{
		double tmin, tmax;
		return intersect1(ray, tmin, tmax);
	}

	inline bool intersect1(const Ray& ray, double& tmin, double& tmax) const
	{
		double tymin, tymax, tzmin, tzmax;
				
		tmin = (mCorners[ray.sign[0]].x() - ray.org.x()) * ray.myInv.x();
		tmax = (mCorners[1 - ray.sign[0]].x() - ray.org.x()) * ray.myInv.x();
		tymin = (mCorners[ray.sign[1]].y() - ray.org.y()) * ray.myInv.y();
		tymax = (mCorners[1 - ray.sign[1]].y() - ray.org.y()) * ray.myInv.y();

		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;

		if (tymax < tmax)
			tmax = tymax;

		tzmin = (mCorners[ray.sign[2]].z() - ray.org.z()) * ray.myInv.z();
		tzmax = (mCorners[1 - ray.sign[2]].z() - ray.org.z()) * ray.myInv.z();

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;

		if (tzmax < tmax)
			tmax = tzmax;

		return true;
	}

private:
	Vector mCorners[2];
	Vector mCenter;

};

#endif