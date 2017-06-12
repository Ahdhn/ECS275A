
#include "Scene.h"
#include "Background.h"
#include "Camera.h"
#include "HitRecord.h"
#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "Object.h"
#include "Ray.h"
#include "RenderContext.h"
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>


using namespace std;

Scene::Scene()
{
  object = 0;
  background = 0;
  camera = 0;
  ambient = Color(0, 0, 0);
  image = 0;
  minAttenuation = 0;
  
  pixel_sam_freq = 0;
  len_sam_freq = 0;
  time_sam_freq = 0;
  sam_freq = 0;
  
}

Scene::~Scene()
{
  delete object;
  delete background;
  delete camera;
  delete image;
  for(int i=0;i<static_cast<int>(lights.size());i++){
    Light* light = lights[i];
    delete light;
  }
}

void Scene::preprocess()
{
	background->preprocess();
	for (int i = 0; i<static_cast<int>(lights.size()); i++){
		Light* light = lights[i];
		light->preprocess();
	}
	double aspect_ratio = image->aspect_ratio();
	camera->preprocess(aspect_ratio);
	object->preprocess();
	
}

void Scene::render()
{
	if (!object || !background || !camera || !image){
		cerr << "Incomplete scene, cannot render!\n";
		exit(1);
	}
	int xres = image->getXresolution();
	int yres = image->getYresolution();
	RenderContext context(this);
	double dx = 2. / xres;
	double xmin = -1. + dx / 2.;
	double dy = 2. / yres;
	double ymin = -1. + dy / 2.;
	Color atten(1, 1, 1);
	
	double total_pixel = std::max(1, pixel_sam_freq*pixel_sam_freq)*std::max(1, len_sam_freq*len_sam_freq)*std::max(1, time_sam_freq);


	/*for (int j = 0; j < yres; j++){
		for (int i = 0; i < xres; i++){
			Color buffer(0.0, 0.0, 0.0);
			
			vector<Point2D>pl = PixelSampling(i, j, xres, yres, context); 

			for (size_t k = 0; k < pl.size(); k++){
				vector<Ray>rays;
				
				camera->makeRay(rays, context, pl[k].x, pl[k].y);

				for (size_t r = 0; r < rays.size(); r++){
					std::vector <double>tl = TimeSampling(context);
					for (size_t t = 0; t < tl.size(); t++){
						object->move(tl[t]);
						HitRecord hit(DBL_MAX);

						object->intersect(hit, context, rays[r]);
						Color mycolor(0.0, 0.0, 0.0);
						if (hit.getPrimitive()){
							const Material*matl = hit.getMaterial();
							matl->shade(mycolor, context, rays[r], hit, atten, 0);
						}
						else{
							background->getBackgroundColor(mycolor, context, rays[r]);
						}
						buffer += mycolor;
					}
				}
			}

			image->set(i, j, buffer / total_pixel);

			//int pre = -1;
			//int done = (j * xres + i + 1) * 100 / (yres * xres);
			//if (pre != done) {
			//	printf("\r%3d%% pixels have been rendered", done);
			//	fflush(stdout);
			//	pre = done;
			//}
		}
	}*/





	for (int i = 0; i<yres; i++){
		//cerr << "y=" << i << '\n';
		double y = ymin + i*dy;
		for (int j = 0; j<xres; j++){
			double x = xmin + j*dx;
			//cerr << "x=" << j << ", y=" << i << '\n';
			Ray ray;
			camera->makeRay(ray, context, x, y);
			HitRecord hit(DBL_MAX);
			object->intersect(hit, context, ray);
			Color result;
			if (hit.getPrimitive()){
				// Ray hit something...
				const Material* matl = hit.getMaterial();
				matl->shade(result, context, ray, hit, atten, 0);
			}
			else {
				background->getBackgroundColor(result, context, ray);
			}
			image->set(j, i, result);
		}
	}
}

double Scene::traceRay(Color& result, const RenderContext& context, const Ray& ray, const Color& atten, int depth) const
{
  if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
    result = Color(0, 0, 0);
    return 0;
  } else {
    HitRecord hit(DBL_MAX);
    object->intersect(hit, context, ray);
    if(hit.getPrimitive()){
      // Ray hit something...
      const Material* matl = hit.getMaterial();
      matl->shade(result, context, ray, hit, atten, depth);
      return hit.minT();
    } else {
      background->getBackgroundColor(result, context, ray);
      return DBL_MAX;
    }
  }
}

double Scene::traceRay(Color& result, const RenderContext& context, const Object* obj, const Ray& ray, const Color& atten, int depth) const
{
  if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
    result = Color(0, 0, 0);
    return 0;
  } else {
    HitRecord hit(DBL_MAX);
    obj->intersect(hit, context, ray);
    if(hit.getPrimitive()){
      // Ray hit something...
      const Material* matl = hit.getMaterial();
      matl->shade(result, context, ray, hit, atten, depth);
      return hit.minT();
    } else {
      background->getBackgroundColor(result, context, ray);
      return DBL_MAX;
    }
  }
}

std::vector<double> Scene::TimeSampling(const RenderContext&context)
{
	vector<double> ret;
		
	if (time_sam_freq > 0){
		for (int i = 0; i < time_sam_freq; i++){
			ret.push_back(shutter*(i + (double(rand()) / double(RAND_MAX))) / time_sam_freq);
		}
	}
	else{
		ret.push_back(0.0);
	}
	return ret;
}
std::vector<Point2D> Scene::PixelSampling(int x, int y, int xres, int yres, const RenderContext&context)
{
	
	vector<Point2D> ret;
	
	for (int i = 0; i < pixel_sam_freq; i++){
		for (int j = 0; j < pixel_sam_freq; j++){
			double sx = x + (i + (double(rand()) / double(RAND_MAX))) / pixel_sam_freq;
			sx = sx * 2.0 / xres - 1.0;
			double sy = y + (j + (double(rand()) / double(RAND_MAX))) / pixel_sam_freq;
			sy = sy * 2.0 / yres - 1.0;
			ret.push_back(Point2D(sx, sy));
		}
	}

	if (ret.empty()){
		//double sx = (x + 0.5)*0.2 / xres - 1.0;
		//double sy = (y + 0.5)*0.2 / yres - 1.0;
				
		double dx = 2. / xres;
		double xmin = -1. + dx / 2.;
		double dy = 2. / yres;
		double ymin = -1. + dy / 2.;
		double sy = ymin + y*dy;
		double sx = xmin + x*dx;
		ret.push_back(Point2D(sx, sy));
	}
	return ret;

}
