
#ifndef ThinLensCamera_h
#define ThinLensCamera_h

#include "Camera.h"
#include "Point.h"
#include "Vector.h"
#include "Scene.h"
#include <vector>
class ThinLensCamera : public Camera {
 public:
	 ThinLensCamera(const Point& eye,
		            const Point& lookat,
					const Vector& up,
					double hfov,
					double LenRadius,
					double FocusDist);

  virtual ~ThinLensCamera();
  
  virtual void preprocess(double aspect_ratio);

  virtual void makeRay(std::vector<Ray>& rays, const RenderContext& context, double x, double y) const;//uselss 

  virtual void makeRay(Ray& ray, const RenderContext& context, double x, double y) const;
  virtual void makeRay(std::vector<Ray>& rays, const RenderContext& context, const std::vector<Point2D> subpixels) const;

 private:
	 ThinLensCamera(const ThinLensCamera&);
	 ThinLensCamera& operator=(const ThinLensCamera&);

  Point eye;
  Point lookat;
  Vector up;
  double hfov;
  double LenRadius;
  double FocusDist;

  Vector u;
  Vector v;
  Vector lookdir;
  Vector uLens;
  Vector vLens;
};

#endif

