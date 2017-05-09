
#ifndef Scene_h
#define Scene_h

#include "Color.h"
#include "Point.h"
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
class Primitive;
//struct Point2D;


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
	void addAreaLight(Primitive*area_light){
		arealights.push_back(area_light);
	}
	const std::vector<Primitive*>&getAreaLights()const{
		return arealights;
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
	int getPixelSamplingFrequency() const{
		return pixel_sam_freq;
	}

	void setLensSamplingFrequency(int val){
		len_sam_freq = val;
	}
	int getLensSamplingFrequency()const{ 
		return len_sam_freq;
	}
	

	void setTimeSamplingFrequency(int val){
		time_sam_freq = val;
	}
	int getTimeSamplingFrequency()const{
		return time_sam_freq;
	}

	void setSamplingFequency(int val){
		sam_freq = val;
	}
	int getSamplingFequency()const{
		return sam_freq;
	}

	void setPathTracingFrequency(int val){
		path_trace_freq = val;
	}
	int getPathTracingFrequency()const{
		return path_trace_freq;
	}

	void setShutter(double val){
		shutter = val;
	}
	double getShutter()const{
		return shutter;
	}

	void preprocess();
	void render();
	double traceRay(Color& result, const RenderContext& context, const Ray& ray, const Color& attenuation, int depth) const;
	double traceRay(Color& result, const RenderContext& context, const Object* obj, const Ray& ray, const Color& attenuation, int depth) const;

private:
	Scene(const Scene&);
	Scene& operator=(const Scene&);
	std::vector<double> TimeSampling(const RenderContext&context);
	std::vector<Point2D> PixelSampling(int x, int y, int xres, int yres, const RenderContext&context);

	Background* background;
	Camera* camera;
	Color ambient;
	Image* image;
	Object* object;
	std::vector<Light*> lights;
	std::vector<Primitive*> arealights;
	int maxRayDepth;
	double minAttenuation, shutter;
	int pixel_sam_freq, len_sam_freq, time_sam_freq, sam_freq, path_trace_freq;
	std::vector< std::vector<Color> >accum;
	std::vector< std::vector<Color> >buffer;
	
};

//struct Point2D {
//	double x;
//	double y;
//	Point2D() : x(0.0), y(0.0) {}
//	Point2D(double x, double y) : x(x), y(y) {}
//};

#endif
