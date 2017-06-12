#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <vector_types.h>
#include <stdio.h>
#include <iostream>
#include <curand.h>
#include <time.h>
#include <curand_kernel.h>
#include "cutil_math.h"

#define PI 3.14159265359f  
#define width 1024
#define height 1024
#define pixelSamples 4

//error handling micro, wrap it around function whenever possible
static void HandleError(cudaError_t err, const char *file, int line) {
	if (err != cudaSuccess) {
		printf("\n%s in %s at line %d\n", cudaGetErrorString(err), file, line);
		system("pause");
		//exit(EXIT_FAILURE);
	}
}
#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))

struct Ray{
	float3 o, d;
	__device__ 	Ray(float3 o_, float3 d_) :o(o_), d(d_){}
};
enum Refl_t { DIFF, SPEC, REFR };
struct Sphere
{
	float rad;
	float3 p, e, c;
	Refl_t refl;

	__device__ float intersect(const Ray &r) const { // returns distance, 0 if nohit
		float3 op = p - r.o; // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0

		float t, eps = 1e-4, b = dot(op, r.d), det = b*b - dot(op, op) + rad*rad;
		if (det<0)
			return 0;
		else
			det = sqrtf(det);
		return (t = b - det)>eps ? t : ((t = b + det)>eps ? t : 0);
	}
};

//Scene Spheres 
__constant__ Sphere spheres[] = {
	{ 1e5f, { 1e5f + 1.0f, 40.8f, 81.6f }, { 0.0f, 0.0f, 0.0f }, { 0.75f, 0.25f, 0.25f }, DIFF }, //Left 
	{ 1e5f, { -1e5f + 99.0f, 40.8f, 81.6f }, { 0.0f, 0.0f, 0.0f }, { .25f, .25f, .75f }, DIFF }, //Rght 
	{ 1e5f, { 50.0f, 40.8f, 1e5f }, { 0.0f, 0.0f, 0.0f }, { .75f, .75f, .75f }, DIFF }, //Back 
	{ 1e5f, { 50.0f, 40.8f, -1e5f + 600.0f }, { 0.0f, 0.0f, 0.0f }, { 1.00f, 1.00f, 1.00f }, DIFF }, //Frnt 
	{ 1e5f, { 50.0f, 1e5f, 81.6f }, { 0.0f, 0.0f, 0.0f }, { .75f, .75f, .75f }, DIFF }, //Botm 
	{ 1e5f, { 50.0f, -1e5f + 81.6f, 81.6f }, { 0.0f, 0.0f, 0.0f }, { .75f, .75f, .75f }, DIFF }, //Top 
	{ 16.5f, { 27.0f, 16.5f, 47.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, DIFF }, // small sphere 1
	{ 16.5f, { 73.0f, 16.5f, 78.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, DIFF }, // small sphere 2
	{ 600.0f, { 50.0f, 681.6f - .77f, 81.6f }, { 2.0f, 1.8f, 1.6f }, { 0.0f, 0.0f, 0.0f }, DIFF }  // Light
};







inline float clamp(double x){ return x<0 ? 0 : x>1 ? 1 : x; }
inline int toInt(double x){ return int(pow(clamp(x), 1 / 2.2) * 255 + .5); }


__device__ inline bool intersect(const Ray&r, float&t, int&id)
{
	double n = sizeof(spheres) / sizeof(Sphere), d, inf = t = 1e20;
	for (int i = int(n); i--;) if ((d = spheres[i].intersect(r)) && d<t){ t = d; id = i; }	
	return t<inf;
}


__global__ void initialise_curand_on_kernels(curandState * state, unsigned long seed)
{
	//https://nidclip.wordpress.com/2014/04/02/cuda-random-number-generation/
	int idx = blockIdx.x*blockDim.x + threadIdx.x;
	curand_init(seed, idx, 0, &state[idx]);
}
__device__ float generateRAND(curandState* globalState, int ind)
{
	//https://nidclip.wordpress.com/2014/04/02/cuda-random-number-generation/
	//copy state to local mem
	curandState localState = globalState[ind];
	//apply uniform distribution with calculated random
	float rndval = curand_uniform(&localState);
	//update state
	globalState[ind] = localState;
	//return value
	return rndval;
}

__device__ float3 Radiance(Ray &r, curandState* globalState, int ind)
{
	float3 accucolor = make_float3(0.0f, 0.0f, 0.0f);
	float3 mask = make_float3(1.0f, 1.0f, 1.0f);

	for (int bounces = 0; bounces < 4; bounces++){
		float t;          
		int id = 0;

		if (!intersect(r, t, id))
			return make_float3(0.0f, 0.0f, 0.0f); 
		const Sphere &obj = spheres[id];
		float3 x = r.o + r.d*t;
		float3 n = normalize(x - obj.p);
		float3 nl = dot(n, r.d) < 0 ? n : n * -1;
		accucolor += mask * obj.e;

		float r1 = 2 * PI * generateRAND(globalState, ind);
		float r2 = generateRAND(globalState, ind);
		float r2s = sqrtf(r2);

		float3 w = nl;
		float3 u = normalize(cross((fabs(w.x) > .1 ? make_float3(0, 1, 0) : make_float3(1, 0, 0)), w));
		float3 v = cross(w, u);

		float3 d = normalize(u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrtf(1 - r2));

		r.o = x + nl*0.05f;
		r.d = d;

		mask *= obj.c;
		mask *= dot(d, nl);
		mask *= 2;
	}

	return accucolor;
}
__global__ void pathTracing(float3*buffer_d, curandState* globalState)
{
	unsigned int x = blockIdx.x*blockDim.x + threadIdx.x;
	unsigned int y = blockIdx.y*blockDim.y + threadIdx.y;
	unsigned int i = (height - y - 1)*width + x;

	

	Ray cam(make_float3(50, 52, 295.6), normalize(make_float3(0, -0.042612, -1)));
	float3 cx = make_float3(width * .5135 / height, 0.0f, 0.0f);
	float3 cy = normalize(cross(cx, cam.d)) * .5135;
	float3 r;
	r = make_float3(0.0f);
	for (int s = 0; s < pixelSamples; s++){
		float3 d = cam.d + cx*((.25 + x) / width - .5) + cy*((.25 + y) / height - .5);
		r = r + Radiance(Ray(cam.o + d * 40, normalize(d)), globalState, x)*(1. / pixelSamples);
	}
	buffer_d[i] = make_float3(clamp(r.x, 0.0f, 1.0f), clamp(r.y, 0.0f, 1.0f), clamp(r.z, 0.0f, 1.0f));
}

int main(){
	
	float3* buffer_d;

	HANDLE_ERROR(cudaMalloc(&buffer_d, width*height*sizeof(float3)));

	dim3 threads(8, 8, 1);
	dim3 block(width / threads.x, height / threads.y, 1);

	//rand num
	srand(time(NULL));
	curandState* deviceStates;
	HANDLE_ERROR(cudaMalloc(&deviceStates, 100 * sizeof(curandState)));
	initialise_curand_on_kernels << <100 / 1024 + 1, 1024 >> >(deviceStates, unsigned(time(NULL)));
	cudaDeviceSynchronize();



	cudaEvent_t start, stop;
	HANDLE_ERROR(cudaEventCreate(&start));
	HANDLE_ERROR(cudaEventCreate(&stop));
	HANDLE_ERROR(cudaEventRecord(start, NULL));
	pathTracing << <block, threads >> >(buffer_d, deviceStates);
	cudaDeviceSynchronize();
	HANDLE_ERROR(cudaEventRecord(stop, NULL));
	HANDLE_ERROR(cudaEventSynchronize(stop));
	float time = 0.0f;
	HANDLE_ERROR(cudaEventElapsedTime(&time, start, stop));
	

	float3* buffer_h = new float3[width*height];
	HANDLE_ERROR(cudaMemcpy(buffer_h, buffer_d, width*height*sizeof(float3), cudaMemcpyDeviceToHost));

	cudaFree(buffer_d);
	cudaFree(deviceStates);

	FILE *f = fopen("image.ppm", "w");         // Write image to PPM file.
	fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
	for (int i = 0; i < width*height; i++){
		fprintf(f, "%d %d %d ", toInt(buffer_h[i].x), toInt(buffer_h[i].y), toInt(buffer_h[i].z));
	}
	fclose(f);

	printf("\nRendering time = %f (ms)", time);

	return 0;



}