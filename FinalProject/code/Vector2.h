
#ifndef Vector_h 
#define Vector_h


#include "Math.h"
#include <iosfwd>
#include <math.h>
#include <cmath>

typedef double Real;

class Point;
class Vector {

public:
	//constructor 
	inline Vector() {
		data[0] = data[1] = data[2] = 0.0f;
	}
	inline Vector(double x, double y, double z){
		data[0] = x; data[1] = y; data[2] = z;
	}
	inline explicit Vector(const Point& copy){
		data[0] = copy.x(); data[1] = copy.y(); data[2] = copy.z();
	}	
	inline Vector(const Vector& copy){
		for (int i = 0; i<3; i++)
			data[i] = copy.data[i];
	}
	inline Vector(const Real& d){
		data[0] = data[1] = data[2] = d;
	}	
	~Vector() {
	}

	//assignment 
	inline void set(Real d){
		data[0] = data[1] = data[2] = d;
	}
	inline void set(const Vector& v){
		data[0] = v[0]; data[1] = v[1]; data[2] = v[2];
	}
	//operator=
	Vector& operator=(const Vector& copy) {
		for (int i = 0; i<3; i++)
			data[i] = copy.data[i];
		return *this;
	}
	Vector& operator=(const Real& d){
		data[0] = data[1] = data[2] = d; return (*this);
	}

	//accessors and mutators
	inline Real& operator()(unsigned int i)		    { return data[i]; }
	inline Real  operator()(unsigned int i) const	{ return data[i]; }
	inline Real& operator[](unsigned int i)		    { return data[i]; }
	inline Real  operator[](unsigned int i) const	{ return data[i]; }

	inline Real& x()		{ return data[0]; }
	inline Real  x() const	{ return data[0]; }
	inline Real& y()		{ return data[1]; }
	inline Real  y() const	{ return data[1]; }
	inline Real& z()		{ return data[2]; }
	inline Real  z() const	{ return data[2]; }

	inline Real& r()		{ return data[0]; }
	inline Real  r() const	{ return data[0]; }
	inline Real& g()		{ return data[1]; }
	inline Real  g() const	{ return data[1]; }
	inline Real& b()		{ return data[2]; }
	inline Real  b() const	{ return data[2]; }		

	
	//operator+
	Vector operator+(const Vector& v) const {
		return Vector(data[0] + v.data[0], data[1] + v.data[1], data[2] + v.data[2]);
	}
	Vector& operator+=(const Real& d) const {
		data[0] += d;
		data[1] += d;
		data[2] += d;
		return (*this);
	}
	Vector& operator+=(const Vector& v) {
		data[0] += v.data[0]; data[1] += v.data[1]; data[2] += v.data[2];
		return *this;
	}
	

	//operator-
	Vector operator-(const Vector& v) const {
		return Vector(data[0] - v.data[0], data[1] - v.data[1], data[2] - v.data[2]);
	}
	Vector operator-() const {
		return Vector(-data[0], -data[1], -data[2]);
	}
	
	Vector& operator-=(const Vector& v) {
		data[0] -= v.data[0]; data[1] -= v.data[1]; data[2] -= v.data[2];
		return *this;
	}
	Vector& operator-=(const Real& d){
		data[0] -= d; data[1] -= d; data[2] -= d; return (*this);
	}
	Vector operator-(const Vector& a, const Vector& b){
		return Vector3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
	}
	Vector operator-(const Vector& a, const Real& b){
		return Vector3(a[0] - b, a[1] - b, a[2] - b);
	}
	Vector operator-(const Real& a, const Vector& b){
		return Vector3(a - b[0], a - b[1], a - b[2]);
	}

	//operator*
	Vector operator*(const Vector& v) const {
		return Vector(data[0] * v.data[0], data[1] * v.data[1], data[2] * v.data[2]);
	}
	Vector operator*(double s) const {
		return Vector(data[0] * s, data[1] * s, data[2] * s);
	}
	Vector operator*(double s, const Vector& v) {
		return Vector(s*v.x(), s*v.y(), s*v.z());
	}
	Vector& operator*=(const Vector& v) {
		data[0] *= v.data[0]; data[1] *= v.data[1]; data[2] *= v.data[2];
		return *this;
	}
	Vector& operator*=(double s) {
		data[0] *= s; data[1] *= s; data[2] *= s;
		return *this;
	}
	Vector operator*(const Vector& a, const Vector& b){
		return Vector3(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
	}
	Vector operator*(const Vector& a, const Real& b){
		return Vector3(a[0] * b, a[1] * b, a[2] * b);
	}

	
	//operator/
	Vector operator/(double s) const {
		double inv_s = 1. / s;
		return Vector(data[0] * inv_s, data[1] * inv_s, data[2] * inv_s);
	}
	Vector& operator/=(double s) {
		double inv_s = 1. / s;
		data[0] *= inv_s; data[1] *= inv_s; data[2] *= inv_s;
		return *this;
	}
	Vector operator/(const Vector& v) const {
		return Vector(data[0] / v.data[0], data[1] / v.data[1], data[2] / v.data[2]);
	}
	Vector& operator/=(const Vector &v){
		data[0] /= v[0]; data[1] /= v[1]; data[2] /= v[2]; return (*this);
	}
	Vector operator/(const Vector& a, const Vector& b){
		return Vector(a[0] / b[0], a[1] / b[1], a[2] / b[2]);
	}
	Vector operator/(const Vector& a, const Real& b){
		return Vector(a[0] / b, a[1] / b, a[2] / b);
	}
	Vector operator/(const Real& a, const Vector& b){
		return Vector(a / b[0], a / b[1], a / b[2]);
	}
	Vector inverseVector(){
		return Vector(1.0 / data[0], 1.0 / data[1], 1.0 / data[2]);
	}

	double length() const {
		return sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
	}
	double length2() const {
		return data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
	}

	double normalize() {
		double l = length();
		double scale = 1. / l;
		*this *= scale;
		return l;
	}

	double minComponent() const {
		return Min(data[0], data[1], data[2]);
	}
	double maxComponent() const {
		return Max(data[0], data[1], data[2]);
	}

	Vector inverse() const {
		return Vector(1. / data[0], 1. / data[1], 1. / data[2]);
	}
	Vector absoluteValue() const {
		return Vector(Abs(data[0]), Abs(data[1]), Abs(data[2]));
	}

	static Vector zero() {
		return Vector(0, 0, 0);
	}
	static Vector one() {
		return Vector(1, 1, 1);
	}

private:
	double data[3];
};

#include "Point.h"

inline double Dot(const Vector& v1, const Vector& v2)
{
	return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}

inline Vector Cross(const Vector& v1, const Vector& v2)
{
	return Vector(v1.y()*v2.z() - v1.z()*v2.y(),
		v1.z()*v2.x() - v1.x()*v2.z(),
		v1.x()*v2.y() - v1.y()*v2.x());
}

inline Vector Min(const Vector& p1, const Vector& p2)
{
	return Vector(Min(p1.x(), p2.x()), Min(p1.y(), p2.y()), Min(p1.z(), p2.z()));
}

inline Vector Max(const Vector& p1, const Vector& p2)
{
	return Vector(Max(p1.x(), p2.x()), Max(p1.y(), p2.y()), Max(p1.z(), p2.z()));
}

//typedef	Vector Color;
//typedef Vector Point;

std::ostream& operator<<(std::ostream&, const Vector& v);

#endif
