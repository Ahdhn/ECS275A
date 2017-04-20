
#ifndef Scene_h
#define Scene_h

#include "Color.h"
#include <string>
#include <vector>

class Background;
class Camera;
class Color;
class Image;
class Light;
class Object;
class RenderContext;
class Ray;

class Scene {
public:
	Scene();
	virtual ~Scene();

	inline Object* getObject() const {
		return object;
	}
	void setObject(Object* obj) {
		object = obj;
	}

	inline Background* getBackground() const {
		return background;
	}
	void setBackground(Background* bg) {
		background = bg;
	}

	inline Camera* getCamera() const {
		return camera;
	}
	void setCamera(Camera* cam) {
		camera = cam;
	}

	inline Image* getImage() const {
		return image;
	}
	void setImage(Image* im) {
		image = im;
	}

	void addLight(Light* light) {
		lights.push_back(light);
	}
	const std::vector<Light*>& getLights() const {
		return lights;
	}

	Color getAmbient() const {
		return ambient;
	}
	void setAmbient(const Color& amb) {
		ambient = amb;
	}

	int getMaxRayDepth() const {
		return maxRayDepth;
	}
	void setMaxRayDepth(int rd) {
		maxRayDepth = rd;
	}
	double getMinAttenuation() const {
		return minAttenuation;
	}
	void setMinAttenuation(double atten) {
		minAttenuation = atten;
	}
	void setPixelSamplingFrequency(int val){
		pixel_sam_freq = val;
	}	
	void setLensSamplingFrequency(int val){
		len_sam_freq = val;
	}
	void setTimeSamplingFrequency(int val){
		time_sam_freq = val;
	}
	void setSamplingFequency(int val){
		sam_freq = val;
	}
	void setPathTracingFrequency(int val){
		path_trace_freq = val;
	}
	void setShutter(double val){
		shutter = val;
	}






	void preprocess();
	void render();
	double traceRay(Color& result, const RenderContext& context, const Ray& ray, const Color& attenuation, int depth) const;
	double traceRay(Color& result, const RenderContext& context, const Object* obj, const Ray& ray, const Color& attenuation, int depth) const;

private:
	Scene(const Scene&);
	Scene& operator=(const Scene&);

	Background* background;
	Camera* camera;
	Color ambient;
	Image* image;
	Object* object;
	std::vector<Light*> lights;
	int maxRayDepth;
	double minAttenuation, shutter;
	int pixel_sam_freq, len_sam_freq, time_sam_freq, sam_freq, path_trace_freq;
};

#endif
