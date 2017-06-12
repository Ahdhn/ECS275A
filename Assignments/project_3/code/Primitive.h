
#ifndef Primitive_h
#define Primitive_h

#include "Object.h"

class Point;
class Vector;

#include <vector>
#include "Material.h"
#include "Vector.h"
#include "Color.h"

class Primitive : public Object {
 public:
  Primitive(Material* matl);
  Primitive(Material* matl, bool is_luminous, int sampling_freq);
  virtual ~Primitive();

  virtual void preprocess();
  virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const = 0;
  virtual void normal(Vector& normal, const RenderContext& context,
                      const Point& hitpos, const Ray& ray, const HitRecord& hit) const = 0;
  virtual void move(double dist) = 0;
  virtual void computeUVW(Vector& uvw, const RenderContext& context,
                          const Ray& ray, const HitRecord& hit) const;

  
  virtual bool isLuminous() const { return is_luminous; }

  virtual Color getColor()const{
	  return matl->getColor();
  }

  virtual void Samples(std::vector<Vector>&rays, const RenderContext&context, const Point&hitpos)const = 0; 
  virtual double getArea() const {return 0.0; }

 protected:
  Material* matl;
  bool is_luminous;
  int sampling_freq;
 private:
  Primitive(const Primitive&);
  Primitive& operator=(const Primitive&);
};


#endif

