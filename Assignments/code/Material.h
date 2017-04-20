
#ifndef Material_h
#define Material_h

class Color;
class HitRecord;
class Ray;
class RenderContext;

class Material {
 public:
  Material();
  virtual ~Material();

  virtual void preprocess();
  virtual void shade(Color& result, const RenderContext& context, const Ray& ray,
                     const HitRecord& hit, const Color& atten, int depth) const = 0;

 private:
  Material(const Material&);
  Material& operator=(const Material&);
};

#endif

