
#ifndef Plane_h
#define Plane_h

#include "Vector.h"
#include "Primitive.h"
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
  virtual void move(double dist);
  virtual bool isLuminous() const { return false; }

  virtual void Samples(std::vector<Vector>&rays, const RenderContext&context, const Point&hitpos)const{};
  virtual double getArea() const { return 0.0; }
  virtual void computeUVW(Vector& uvw, const RenderContext& context, const Ray& ray, const HitRecord& hit) const{};

 private:
  Plane(const Plane&);
  Plane& operator=(const Plane&);
  bool is_luminous;

  Vector n;
  double d;
};

#endif

