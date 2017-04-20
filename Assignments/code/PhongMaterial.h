#ifndef PhongMaterial_h
#define PhongMaterial_h

#include "Material.h"
#include "Color.h"

class PhongMaterial : public Material {
  public:
    PhongMaterial(const Color& color, 
                  double Ka,
				  double Kd,				  
				  double Exponent,
                  double Kr);

    virtual ~PhongMaterial();

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

    Color color;
    double Ka;
    double Kd;    
	double Exponent;
    double Kr;
};

#endif
