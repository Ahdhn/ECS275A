#ifndef MetalMaterial_h
#define MetalMaterial_h

#include "Material.h"
#include "Vector3.h"
#include "HitRecord.h"
#include "Primitive.h"
#include "RenderContext.h"
#include "Scene.h"
#include <cmath>

/*class MetalMaterial : public Material
	{
	public:
		MetalMaterial(Color color, int exponent) :
		mColor(color), mExponent(exponent) {  }
		
		virtual Color getColor(const HitRecord& hit, const RenderContext& context,
							   const Ray& ray) const
		{
			Point p = ray.origin + hit.getDist() * ray.direction;
			Vector3 normal = hit.getPrimitive()->getNormal(p);
			double costheta = dot(normal, ray.direction);
			
			if (costheta > 0)
				normal = -normal;
			else
				costheta = -costheta;
			
			Color specLight(0.0);
			
			for (int i = 0; i < context.getScene()->lightCount(); i++)
			{
				Light* currLight = context.getScene()->getLight(i);
				Color Cl = currLight->getColor();
				Vector3 L = currLight->getPosition() - p;
				
				double dist = length(L);
				Vector3 Ln = L / dist;
				
				double cosphi = dot(normal, Ln);
				
				if (cosphi > 0)
				{
					HitRecord lightHit(dist);
					Ray lightRay(p+L*1e-6, Ln);
					context.getScene()->getObject()->intersect(lightHit, context, lightRay);
					
					if (lightHit.getPrimitive() == NULL)
					{
						Vector3 H = (Ln - ray.direction);
						normalize(H);
						double cosalpha = dot(H, normal);
						
						if (cosalpha > 0)
							specLight += Cl * pow(cosalpha, mExponent);
					}
				}
			}
			
			double R0 = 1.0; // Purely reflective
			Color result = specLight * R0 * mColor;
			
			if (ray.depth < context.getScene()->getMaxRayDepth())
			{
				double oneMinusCosTheta = (1 - costheta);
				double oneMinusCosTheta5 = (oneMinusCosTheta * oneMinusCosTheta * oneMinusCosTheta * oneMinusCosTheta *oneMinusCosTheta);
				double Fr = R0 + (1.0 - R0) * oneMinusCosTheta5;
				Vector3 reflect = ray.direction + 2 * costheta * normal;
				HitRecord reflectHit(1e6);
				Ray reflectRay(p+1e-6*reflect, reflect);
				reflectRay.depth = ray.depth + 1;
				context.getScene()->getObject()->intersect(reflectHit, context, reflectRay);
				
				if (reflectHit.getPrimitive())
				{
					result += Fr * mColor * reflectHit.getMaterial()->getColor(reflectHit, context, reflectRay);
				}
				else
				{
					result += (1 - oneMinusCosTheta5) * mColor * context.getScene()->getBackground()->getColor(reflectRay) + 
					oneMinusCosTheta5 * context.getScene()->getBackground()->getColor(reflectRay); 
				}
			}
			
			return result;
		}
		
	private:
		Color mColor;
		int mExponent;
	};*/

#endif
