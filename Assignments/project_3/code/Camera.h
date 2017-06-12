
#ifndef Camera_h
#define Camera_h

class Ray;
class RenderContext;
#include <vector>
#include "Point.h"

class Camera {
 public:
  Camera();
  virtual ~Camera();

  virtual void preprocess(double aspect_ratio) = 0;
  virtual void makeRay(Ray& ray, const RenderContext& context, double x, double y) const = 0;
  virtual void makeRay(std::vector<Ray>& rays, const RenderContext& context, const std::vector<Point2D> subpixels) const = 0;
  virtual void makeRay(std::vector<Ray>& rays, const RenderContext& context, double x, double y) const = 0;

 private:
  Camera(const Camera&);
  Camera& operator=(const Camera&);
};

#endif

