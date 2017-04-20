#include "PhongMaterial.h"
#include "HitRecord.h"
#include "Light.h"
#include "Point.h"
#include "Primitive.h"
#include "Ray.h"
#include "RenderContext.h"
#include "Scene.h"
#include "Vector.h"
#include "Math.h"
#include "ConstantBackground.h"  // for getting background color
#include <cfloat>  // for using DBL_MAX

using namespace std;

PhongMaterial::PhongMaterial(const Color& color, 
                             double Ka,
							 double Kd,							 
							 double Exponent,
                             double Kr)
							 :color(color), Ka(Ka), Kd(Kd), Exponent(Exponent), Kr(Kr)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::shade(Color& result, 
                          const RenderContext& context,
                          const Ray& ray, 
                          const HitRecord& hit, 
                          const Color& atten, 
                          int depth) const
{
    const Scene* scene = context.getScene();
    const vector<Light*>& lights = scene->getLights();
    Point hitpos = ray.origin() + ray.direction() * hit.minT();
    Vector normal;
    hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);
    double costheta = Dot(normal, ray.direction());
    if (costheta > 0) {
        normal = -normal;
    }

    const Object* world = scene->getObject();

    Color light = scene->getAmbient() * Ka;

    Light*const* begin = &lights[0];
    Light*const* end = &lights[0] + lights.size();
    while (begin != end) {
        Color light_color;
        Vector light_direction;  // from hit point to light source
        double dist = (*begin++)->getLight(light_color, light_direction, context, hitpos);
        double cosphi = Dot(normal, light_direction);
        if (cosphi > 0) {
            HitRecord shadowhit(dist);
            Ray shadowray(hitpos, light_direction);
            world->intersect(shadowhit, context, shadowray);
            if (!shadowhit.getPrimitive()) {
                // diffuse
                light += light_color * (Kd * cosphi);
                // specular
                Vector light_reflection = normal * (2.0 * Dot(light_direction, normal)) 
                                          - light_direction;
                double cosalpha = Dot(light_reflection, -(ray.direction()));
                if (cosalpha > 0) {
					light += light_color * (1.0 * pow(cosalpha, Exponent));
                }
            }
        }
    }
    result = light * color;

    // recursive specular reflection
    if (depth < scene->getMaxRayDepth()) {
        Vector reflection_direction = 
          ray.direction() - normal * (2.0 * Dot(ray.direction(), normal));
        HitRecord reflection_hit(DBL_MAX);
        Ray reflection_ray(hitpos, reflection_direction);
        world->intersect(reflection_hit, context, reflection_ray);
        if (reflection_hit.getPrimitive()) {
            Color reflected_color;
            Color atten;  // place holder
            // call the hit material's shade NOT the current material's
            reflection_hit.getMaterial()->shade(reflected_color, 
                                                context, 
                                                reflection_ray, 
                                                reflection_hit, 
                                                atten, 
                                                depth + 1);
            result += reflected_color * Kr;
        }
        else {
            Color reflected_color;  // background color
            scene->getBackground()->getBackgroundColor(reflected_color, context, reflection_ray);
            result += reflected_color * Kr;
        }
    }
}
