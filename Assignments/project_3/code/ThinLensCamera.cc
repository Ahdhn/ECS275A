
#include "ThinLensCamera.h"
#include "Ray.h"
#include "Math.h"
#include "RenderContext.h"
#include "Scene.h"
#include "Point.h"

#include <math.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>

ThinLensCamera::ThinLensCamera(const Point& eye,
	                           const Point& lookat,
	                           const Vector& up,
	                           double hfov,
							   double LenRadius,
							   double FocusDist)
							   :eye(eye), lookat(lookat), up(up), hfov(hfov), LenRadius(LenRadius), FocusDist(FocusDist)
{
	
}


ThinLensCamera::~ThinLensCamera()
{

}

void ThinLensCamera::preprocess(double aspect_ratio)
{
	lookdir = lookat - eye;
	lookdir.normalize();
	u = Cross(lookdir, up);
	u.normalize();
	uLens = u;
	v = Cross(u, lookdir);
	v.normalize();
	vLens = v;
	double ulen = tan(hfov / 2.0 * M_PI / 180.0) * FocusDist;
	double vlen = ulen / aspect_ratio;
	u *= ulen;
	v *= vlen;
}

void ThinLensCamera::makeRay(std::vector<Ray>& rays, const RenderContext& context, double x, double y) const 
{
	rays.clear();
	
	Point target = eye + lookdir*FocusDist + u * x + v * y; 
	Vector direction = target - eye;		
	direction.normalize();

	const int freq = (context.getScene())->getLensSamplingFrequency();
	const double d_a = 2.0 * M_PI / freq;
	const double d_rr = LenRadius * LenRadius / freq;
	if (freq > 0 && LenRadius > 0.0) {
		double s_a = 0.0;
		for (int i = 0; i < freq; ++i) {
			double s_rr = 0.0;
			for (int j = 0; j < freq; ++j) {
				double a = s_a + d_a *(double(rand()) / double(RAND_MAX)); 
				double r = sqrt(s_rr + d_rr * (double(rand()) / double(RAND_MAX)));
				Point sample = eye + uLens * r * cos(a) + vLens * r * sin(a);
				Vector direction = target - sample;
				direction.normalize();
				rays.push_back(Ray(sample, direction));
				s_rr += d_rr;
			}
			s_a += d_a;
		}
	}

	if (rays.empty())  
		rays.push_back(Ray(eye, direction));
}

void ThinLensCamera::makeRay(std::vector<Ray>& rays, const RenderContext& context, const std::vector<Point2D> subpixels) const 
{
	rays.clear();

	std::vector<Point> samples;
			
	int freq = (context.getScene())->getLensSamplingFrequency();//
	double d_a = 2.0 * M_PI / freq;//
	double d_rr = LenRadius * LenRadius / freq;//
	if (freq > 0 && LenRadius > 0.0) {//
		double s_a = 0.0;//
		for (int i = 0; i < freq; ++i) {//
			double s_rr = 0.0;//
			for (int j = 0; j < freq; ++j) {//
				double a = s_a + d_a * (double(rand()) / double(RAND_MAX));//
				double r = sqrt(s_rr + d_rr * (double(rand()) / double(RAND_MAX)));//
				Point s = eye + uLens * r * cos(a) + vLens * r * sin(a);//*
				samples.push_back(s);//*
				s_rr += d_rr;//
			}//
			s_a += d_a;//
		}//
	}//

	if (samples.empty())  
		samples.push_back(eye);

	if (samples.size() == 1 && subpixels.size() > 1) {  
		while (samples.size() < subpixels.size())
			samples.push_back(samples[0]);
	}
	else {
		std::random_shuffle(samples.begin(), samples.end());
	}

	for (size_t i = 0; i < samples.size(); ++i) {
		Point target = eye + lookat * FocusDist + u * subpixels[i].x + v * subpixels[i].y;
		Vector direction = target - samples[i];
		direction.normalize();
		rays.push_back(Ray(samples[i], direction));
	}
}

void ThinLensCamera::makeRay(Ray& ray, const RenderContext& context, double x, double y) const
{
	Vector direction = lookdir + u*x + v*y;
	direction.normalize();
	ray = Ray(eye, direction);
}
