
#ifndef Group_h
#define Group_h

#include "Object.h"
#include "HitRecord.h"
#include "AABBox.h"
#include "Vector.h"

#include <vector>
#include <limits>

class Group : public Object {
public:
	Group();
	virtual ~Group();

	
	virtual void preprocess();
	virtual void getBounds(BoundingBox& bbox) const;
	virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
	virtual void move(double);
	virtual void addObject(Object*obj);

	virtual void getAABBox(AABBox&box)const{
		box = AABBox(Vector(-std::numeric_limits<double>::infinity()),
			         Vector(std::numeric_limits<double>::infinity()),
			         Vector(0));

	}

protected:
	std::vector<Object*> objects;

private:
	Group(const Group&);
	Group& operator=(const Group&);
};

#endif

