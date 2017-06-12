#ifndef PhongMaterial_h
#define PhongMaterial_h
#include <vector>

#include "Material.h"
#include "Color.h"
#include "Vector.h"

class PhongMaterial : public Material {
  public:
    PhongMaterial(const Color& color, 
                  double Ka,
				  double Kd,				  
				  double Exponent,
                  double Kr);

	PhongMaterial(const Color& color,
		          bool is_luminous,
		          bool is_reflective,
		          double Ka,
		          double Kd,
		          double Exponent,
		          double Kr);

    virtual ~PhongMaterial();
	virtual void preprocess(){};
    virtual void shade(Color& result, 
                       const RenderContext& context, 
                       const Ray& ray, 
                       const HitRecord& hit,
                       const Color& atten,
                       int depth) const;
    virtual Color getColor() const {
      return color;
    }

  private:
    PhongMaterial(const PhongMaterial&);
    PhongMaterial& operator=(const PhongMaterial&);
	Vector SampleHemiSphere(Vector n)const;

	Color getDirect(const RenderContext& context, const Ray& ray, const HitRecord& hit)const;
	Color getIndirect(const RenderContext& context, const Ray& ray, const HitRecord& hit, int depth)const;
	double Spec(Vector v_in, Vector n, Vector v_out)const;

    Color color;
	bool is_luminous;
	bool is_reflective;
    double Ka;
    double Kd;    
	double Exponent;
    double Kr;
};

#endif
