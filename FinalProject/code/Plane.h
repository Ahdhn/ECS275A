
#ifndef Plane_h
#define Plane_h

#include "Vector.h"
#include "Primitive.h"
#include "AABBox.h"

class HitRecord;
class Point;
class RenderContext;

class Plane : public Primitive {
 public:
  Plane(Material* material, const Vector& n, const Point& point);
  virtual ~Plane();

  virtual void getBounds(BoundingBox& bbox) const;
  virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
  virtual void normal(Vector& normal, const RenderContext& context,
                      const Point & hitpos, const Ray& ray, const HitRecord& hit) const;
  virtual void move(double dist){};

  virtual void getAABBox(AABBox& box) const
  {
	  // Infinite Bounding box for infinite surfaces
	  box = AABBox(Vector(-std::numeric_limits<double>::infinity()),
		           Vector(std::numeric_limits<double>::infinity()),
		           Vector(0));
  }
 private:
  Plane(const Plane&);
  Plane& operator=(const Plane&);

  Vector n;
  double d;
};

#endif

