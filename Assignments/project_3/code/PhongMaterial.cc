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
#include <assert.h>
using namespace std;

PhongMaterial::PhongMaterial(const Color& color,
	double Ka,
	double Kd,
	double Exponent,
	double Kr)
	:color(color), Ka(Ka), Kd(Kd), Exponent(Exponent), Kr(Kr)
{
}
PhongMaterial::PhongMaterial(const Color& color,
	                         bool is_luminous,
	                         bool is_reflective,
	                         double Ka,
	                         double Kd,
	                         double Exponent,
	                         double Kr):
							 color(color), is_luminous(is_luminous), is_reflective(is_reflective), Ka(Ka), Kd(Kd), Exponent(Exponent), Kr(Kr)
{

}

PhongMaterial::~PhongMaterial()
{
}

//Shading for path tracing 
void PhongMaterial::shade(Color& result, const RenderContext& context, const Ray& ray, const HitRecord& hit, const Color& atten, int depth) const
{ 
	const Scene* scene = context.getScene();
	//check if we reached the max depth yet
	if (depth > scene->getMaxRayDepth() * 10){ return; }

	if (is_luminous){
		//just put the color as it is, will work on this later 
		result = color;
		return;
	}

	//get the direct and indirect components 
	//add then toghther and multi by the color 
	Color col1 = getDirect(context, ray, hit);
	Color col2 = getIndirect(context, ray, hit, depth);
	result = (col1 + col2);
	result *= color;

}
Color PhongMaterial::getDirect(const RenderContext& context, const Ray& ray, const HitRecord& hit)const
{
	//get the direct component from this ray
	const Scene*scene = context.getScene();
	const Object*world = scene->getObject();
	Point hitpos = ray.origin() + ray.direction()*hit.minT();
	Vector normal;
	hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);
	if (Dot(normal, ray.direction()) > 0.0)
		normal = -normal;
		
	const vector<Primitive*>& areaLight = scene->getAreaLights();//get all the area light source 

	int lightSource_id = int((areaLight.size() - 1)*(double(rand()) / double(RAND_MAX)));//pick one of them 	
	Primitive&lightSoucre = *areaLight[lightSource_id];
	vector<Vector> lightRays;

	lightSoucre.Samples(lightRays, context, hitpos);
	
	double rr = 0.0;
	for (int i = 0; i < lightRays.size(); i++){
		if (Dot(normal, lightRays[i])>1E-8){
			HitRecord shadow(DBL_MAX);
			Vector dir = lightRays[i];
			dir.normalize();

			Ray shaowray(hitpos, dir);
			world->intersect(shadow, context, shaowray);

			//assert(shadow.getPrimitive() != NULL);

			if (shadow.getPrimitive() == areaLight[lightSource_id]){
				double brdf;
				if (is_reflective){
					//specular 
					brdf = Spec(dir, normal, -ray.direction());
					//printf("\n spec brdf=%f", brdf);

				}
				else{
					//diffusi
					brdf = Kd/M_PI;
					//printf("\nM_PI=%f", M_PI);
				}
				Point light_hitpos = shaowray.origin() + shaowray.direction()*shadow.minT();
				Vector l_norm;
				lightSoucre.normal(l_norm, context, light_hitpos, shaowray, shadow);
				
				double dist = lightRays[i].normalize();
				rr += brdf * Dot(normal, lightRays[i]) * Dot(l_norm, -lightRays[i]) / (dist * dist);
			}
		}
	}
	rr *= lightSoucre.getArea() / double(lightRays.size());
	rr *= double(areaLight.size());
	
	return lightSoucre.getColor()*rr;
}
double PhongMaterial::Spec(Vector v_in, Vector n, Vector v_out)const{
	Vector ss = n*(2.0*Dot(v_in, n)) - v_in;
	ss.normalize();
	double coo = Dot(v_out, ss);
	if (coo > 1.0){ coo = 1.0; }
	return Ka*(Exponent)*pow(coo, Exponent) / (2.0*M_PI); 
}

Color PhongMaterial::getIndirect(const RenderContext& context, const Ray& ray, const HitRecord& hit, int depth)const
{
	//get the indirect component from this ray (recursivly)
	//up to the depth light path length (checked in shade) 
	//kinda inefficient but it gets the right results anyways 

	const Scene*scene = context.getScene();
	const Object*world = scene->getObject();

	Point hitpos = ray.origin() + ray.direction()*hit.minT();
	Vector normal;
	hit.getPrimitive()->normal(normal, context, hitpos, ray, hit);
	if (Dot(normal, ray.direction()) > 0.0){
		normal -= normal;
	}
	
	Color myColor(0.0, 0.0, 0.0);

	double pmax = color.maxComponent();
	if (depth > scene->getMaxRayDepth()){		
		if (double(rand()) / double(RAND_MAX) > pmax){
			return myColor;
		}
	}
	else{
		pmax = 1.0;
	}


	Vector dir;
	if (is_reflective){
		dir = normal*(2.0*Dot(-ray.direction(), normal)) - (-ray.direction()); 
		dir.normalize();
	}
	else{
		dir = SampleHemiSphere(normal);
	}

	Ray rec_ray(hitpos, dir);
	HitRecord rec_hit(DBL_MAX);
	world->intersect(rec_hit, context, rec_ray);
	Color col(0.0, 0.0, 0.0);
	if (rec_hit.getPrimitive() != NULL&&!(rec_hit.getPrimitive()->isLuminous())){
		rec_hit.getMaterial()->shade(col, context, rec_ray, rec_hit, Color(0.0, 0.0, 0.0), depth + 1);
		myColor = col;
	}
	else{
		scene->getBackground()->getBackgroundColor(col, context, rec_ray);
		myColor = col;
	}

	double brdf;
	if (is_reflective){
		//spec 
		brdf = Spec(dir,normal,-ray.direction());
	}
	else{
		//diffuse
		brdf = Kd / M_PI;
	}
	myColor *= brdf;

	if (is_reflective){
		myColor *= Dot(normal, dir);
	}else{
		myColor *= M_PI;
	}
	myColor /= pmax;

	return myColor;


}
Vector PhongMaterial::SampleHemiSphere(Vector n)const
{
	Vector u;
	if (abs(abs(n.x()) - 1.0) < 1E-8 && abs(n.y()) < 1E-8 && abs(n.z()) < 10E-8){
		u = Cross(n, Vector(0.0, 1.0, 0.0));
	}
	else{
		u = Cross(n, Vector(1.0, 0.0, 0.0));
	}
	u.normalize();
	Vector v = Cross(u, n);
	v.normalize();
	double phi = 2.0*M_PI*(double(rand()) / double(RAND_MAX));
	double r2 = (double(rand()) / double(RAND_MAX));
	Point o(0.0, 0.0, 0.0);
	Point sp = o + u * (sqrt(1.0 - r2) * cos(phi)) + v * (sqrt(1.0 - r2) * sin(phi)) + n * sqrt(r2);
	Vector sam = sp - o;
	sam.normalize();
	return sam;

}






///// shading for ray tracing  
/*void PhongMaterial::shade(Color& result, const RenderContext& context, const Ray& ray, const HitRecord& hit, const Color& atten, int depth) const
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
}*/
