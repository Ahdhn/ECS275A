
#ifndef Primitive_h
#define Primitive_h

#include "Object.h"
class Material;
class Point;
class Vector;

class Primitive : public Object {
 public:
  Primitive(Material* matl);
  Primitive(Material* matl, bool is_luminous);
  virtual ~Primitive();

  virtual void preprocess();
  virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const = 0;
  virtual void normal(Vector& normal, const RenderContext& context,
                      const Point& hitpos, const Ray& ray, const HitRecord& hit) const = 0;

  virtual void computeUVW(Vector& uvw, const RenderContext& context,
                          const Ray& ray, const HitRecord& hit) const;
  virtual bool isLuminous() const { return is_luminous; }

 protected:
  Material* matl;
  bool is_luminous;
 private:
  Primitive(const Primitive&);
  Primitive& operator=(const Primitive&);
};


#endif

