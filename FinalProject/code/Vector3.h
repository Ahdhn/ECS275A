#ifndef Vector_h
#define Vector_h

#include <iostream>
#include <cmath>

#ifdef USE_SINGLE
typedef float Real;
#else
typedef double Real;
#endif

class Vector3
{
public:
	// Constructors
	inline Vector3();
	inline Vector3(const Real& d);
	inline Vector3(const Real& d0, const Real& d1, const Real& d2);
	inline Vector3(const Vector3& v);
	
	// Assignment Methods
	inline void set(Real d);
	inline void set(const Vector3& v);
	inline Vector3& operator=(const Real& d);
	inline Vector3& operator=(const Vector3& v);
	
	// Accessors and Mutators
	inline Real& operator()(unsigned int i);
	inline Real  operator()(unsigned int i) const;
	inline Real& operator[](unsigned int i);
	inline Real  operator[](unsigned int i) const;
	
	inline Real& x();
	inline Real  x() const;
	inline Real& y();
	inline Real  y() const;
	inline Real& z();
	inline Real  z() const;
	
	inline Real& r();
	inline Real  r() const;
	inline Real& g();
	inline Real  g() const;
	inline Real& b();
	inline Real  b() const;
	
	// Inplace Math
	inline Vector3& operator+=(const Real& d);
	inline Vector3& operator-=(const Real& d);
	inline Vector3& operator*=(const Real& d);
	inline Vector3& operator/=(const Real& d);
	
	inline Vector3& operator+=(const Vector3 &v);
	inline Vector3& operator-=(const Vector3 &v);
	inline Vector3& operator*=(const Vector3 &v);
	inline Vector3& operator/=(const Vector3 &v);
	
	// Data
	Real data[3];
};

#pragma mark Unary Operators
inline Vector3 operator-(const Vector3 &a);

#pragma mark Binary Operators (Component-wise ops)
inline Vector3 operator+(const Vector3& a, const Vector3& b);
inline Vector3 operator-(const Vector3& a, const Vector3& b);
inline Vector3 operator*(const Vector3& a, const Vector3& b);
inline Vector3 operator/(const Vector3& a, const Vector3& b);

#pragma mark Binary Operators (Scalar)
inline Vector3 operator+(const Vector3& a, const Real& b);
inline Vector3 operator-(const Vector3& a, const Real& b);
inline Vector3 operator*(const Vector3& a, const Real& b);
inline Vector3 operator/(const Vector3& a, const Real& b);

inline Vector3 operator+(const Real& a, const Vector3& b);
inline Vector3 operator-(const Real& a, const Vector3& b);
inline Vector3 operator*(const Real& a, const Vector3& b);
inline Vector3 operator/(const Real& a, const Vector3& b);

#pragma mark I/O Operators
std::istream& operator>>(std::istream& strm, Vector3& v);
std::ostream& operator<<(std::ostream& strm, const Vector3& v);

#pragma mark Functions
inline Real length(const Vector3& a);
inline Real lengthSq(const Vector3& a);
inline void normalize(Vector3& a);

inline Real dot(const Vector3& a, const Vector3& b);
inline Vector3 cross(const Vector3& a, const Vector3& b);

#pragma mark Vector3 Constructor Implementation

inline Vector3::Vector3()
{
	data[0] = data[1] = data[2] = 0.0f;
}

inline Vector3::Vector3(const Real& d)
{
	data[0] = data[1] = data[2] = d;
}

inline Vector3::Vector3(const Real& d0, const Real& d1, const Real& d2)
{
	data[0] = d0; data[1] = d1; data[2] = d2;
}

inline Vector3::Vector3(const Vector3& v)
{
	data[0] = v.data[0]; data[1] = v.data[1]; data[2] = v.data[2];
}

#pragma mark Assignment Implementation
inline void Vector3::set(Real d)
{
	data[0] = data[1] = data[2] = d;
}

inline void Vector3::set(const Vector3& v)
{
	data[0] = v[0]; data[1] = v[1]; data[2] = v[2];
}

inline Vector3& Vector3::operator=(const Real& d)
{
	data[0] = data[1] = data[2] = d; return (*this);
}

inline Vector3& Vector3::operator=(const Vector3& v)
{
	data[0] = v[0]; data[1] = v[1]; data[2] = v[2]; return (*this);
}

#pragma mark Vector3 Accessor Implementation
inline Real& Vector3::operator()(unsigned int i)		{ return data[i]; }
inline Real  Vector3::operator()(unsigned int i) const	{ return data[i]; }
inline Real& Vector3::operator[](unsigned int i)		{ return data[i]; }
inline Real  Vector3::operator[](unsigned int i) const	{ return data[i]; }

inline Real& Vector3::x()		{ return data[0]; }
inline Real  Vector3::x() const	{ return data[0]; }
inline Real& Vector3::y()		{ return data[1]; }
inline Real  Vector3::y() const	{ return data[1]; }
inline Real& Vector3::z()		{ return data[2]; }
inline Real  Vector3::z() const	{ return data[2]; }

inline Real& Vector3::r()		{ return data[0]; }
inline Real  Vector3::r() const	{ return data[0]; }
inline Real& Vector3::g()		{ return data[1]; }
inline Real  Vector3::g() const	{ return data[1]; }
inline Real& Vector3::b()		{ return data[2]; }
inline Real  Vector3::b() const	{ return data[2]; }

#pragma mark Inplace Math Implementation
inline Vector3& Vector3::operator+=(const Real& d)
{
	data[0] += d; data[1] += d; data[2] += d; return (*this);
}

inline Vector3& Vector3::operator-=(const Real& d)
{
	data[0] -= d; data[1] -= d; data[2] -= d; return (*this);
}

inline Vector3& Vector3::operator*=(const Real& d)
{
	data[0] *= d; data[1] *= d; data[2] *= d; return (*this);
}

inline Vector3& Vector3::operator/=(const Real& d)
{
	data[0] /= d; data[1] /= d; data[2] /= d; return (*this);
}

inline Vector3& Vector3::operator+=(const Vector3 &v)
{
	data[0] += v[0]; data[1] += v[1]; data[2] += v[2]; return (*this);
}

inline Vector3& Vector3::operator-=(const Vector3 &v)
{
	data[0] -= v[0]; data[1] -= v[1]; data[2] -= v[2]; return (*this);
}

inline Vector3& Vector3::operator*=(const Vector3 &v)
{
	data[0] *= v[0]; data[1] *= v[1]; data[2] *= v[2]; return (*this);
}

inline Vector3& Vector3::operator/=(const Vector3 &v)
{
	data[0] /= v[0]; data[1] /= v[1]; data[2] /= v[2]; return (*this);
}

#pragma mark Unary Operator Implementation
inline Vector3 operator-(const Vector3 &a) 
{
	return Vector3(-a[0], -a[1], -a[2]); 
}

#pragma mark Binary Operator Implementation (Component-wise ops)
inline Vector3 operator+(const Vector3& a, const Vector3& b)
{
	return Vector3(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}

inline Vector3 operator-(const Vector3& a, const Vector3& b)
{
	return Vector3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

inline Vector3 operator*(const Vector3& a, const Vector3& b)
{
	return Vector3(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

inline Vector3 operator/(const Vector3& a, const Vector3& b)
{
	return Vector3(a[0] / b[0], a[1] / b[1], a[2] / b[2]);
}

#pragma mark Binary Operator Implementation (Scalar)
inline Vector3 operator+(const Vector3& a, const Real& b)
{
	return Vector3(a[0] + b, a[1] + b, a[2] + b);
}

inline Vector3 operator-(const Vector3& a, const Real& b)
{
	return Vector3(a[0] - b, a[1] - b, a[2] - b);
}

inline Vector3 operator*(const Vector3& a, const Real& b)
{
	return Vector3(a[0] * b, a[1] * b, a[2] * b);
}

inline Vector3 operator/(const Vector3& a, const Real& b)
{
	return Vector3(a[0] / b, a[1] / b, a[2] / b);
}

inline Vector3 operator+(const Real& a, const Vector3& b)
{
	return Vector3(a + b[0], a + b[1], a + b[2]);
}

inline Vector3 operator-(const Real& a, const Vector3& b)
{
	return Vector3(a - b[0], a - b[1], a - b[2]);
}

inline Vector3 operator*(const Real& a, const Vector3& b)
{
	return Vector3(a * b[0], a * b[1], a * b[2]);
}

inline Vector3 operator/(const Real& a, const Vector3& b)
{
	return Vector3(a / b[0], a / b[1], a / b[2]);
}

#pragma mark Function Implementation
inline Real length(const Vector3& a)
{
	return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

inline Real lengthSq(const Vector3& a)
{
	return (a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

inline void normalize(Vector3& a)
{
	Real len = length(a);
	if (len != 0)
	{
		Real invLen = 1.0f/len;
		a *= invLen;
	}
}

inline Real dot(const Vector3& a, const Vector3& b)
{
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}

inline Vector3 cross(const Vector3& a, const Vector3& b)
{
	return Vector3(a[1] * b[2] - b[1] * a[2],
				   a[2] * b[0] - b[2] * a[0],
				   a[0] * b[1] - b[0] * a[1]);
}


typedef	Vector3 Color;
typedef Vector3 Point;

#endif
