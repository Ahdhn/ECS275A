#ifndef Ray_h
#define Ray_h

#define ray_bias 1e-6

#include "Point.h"
#include "Vector.h"

class Ray {
public:
	Ray(){ 		
	}
	Ray(const Point& origin, const Vector& direction)
		: org(origin), dir(direction) {		
		init();
	}

	~Ray() { }

	Ray(const Ray& copy) : org(copy.org), dir(copy.dir) {
		init();
	}

	Ray& operator=(const Ray& copy) {
		org = copy.org;
		dir = copy.dir;
		init();
		return *this;
	}

	void init(){
		myInv = dir.inverseVector();
		sign[0] = (myInv.x() < 0);
		sign[1] = (myInv.y() < 0);
		sign[2] = (myInv.z() < 0);
	}

	const Point& origin() const {
		return org;
	}
	const Vector& direction() const {
		return dir;
	}

	const Vector& invDirection() const {		
		
		return myInv;
	}

	void bias() { 	
		org += ray_bias * dir;
	}
	

	int sign[3];
	Vector myInv;
	Point org;
	Vector dir;
private:
	
	
	
};

#endif
