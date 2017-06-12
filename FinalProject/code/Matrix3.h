#ifndef _MATRIX3_
#define _MATRIX3_

#include "Vector.h"

class Matrix3
{
public:
	inline Matrix3();
	inline Matrix3(double x);
	inline Matrix3(const Matrix3& m);
	inline Matrix3(const Vector &row0,
		           const Vector &row1, 
		           const Vector &row2);
	inline double& operator()(unsigned int r, unsigned int c);
	inline double & operator()(unsigned int r, unsigned int c) const;
		

private:
	double data[3][3];

};

inline Matrix3::Matrix3()
{
	data[0][0] = 1;
	data[0][1] = 0;
	data[0][2] = 0;

	data[1][0] = 0;
	data[1][1] = 1;
	data[1][2] = 0;

	data[2][0] = 0;
	data[2][1] = 0;
	data[2][2] = 1;
}

inline Matrix3::Matrix3(double x)
{
	data[0][0] = x;
	data[0][1] = x;
	data[0][2] = x;
				 
	data[1][0] = x;
	data[1][1] = x;
	data[1][2] = x;
				 
	data[2][0] = x;
	data[2][1] = x;
	data[2][2] = x;
}


inline Matrix3::Matrix3(const Matrix3&m)
{
	data[0][0] = m.data[0][0];
	data[0][1] = m.data[0][1];
	data[0][2] = m.data[0][2];
				 
	data[1][0] = m.data[1][0];
	data[1][1] = m.data[1][1];
	data[1][2] = m.data[1][2];
				 
	data[2][0] = m.data[2][0];
	data[2][1] = m.data[2][1];
	data[2][2] = m.data[2][2];
}

inline Matrix3::Matrix3(const Vector&row0, const Vector&row1, const Vector&row2)
{
	data[0][0] = row0.x();
	data[0][1] = row0.y();
	data[0][2] = row0.z();

	data[1][0] = row1.x();
	data[1][1] = row1.y();
	data[1][2] = row1.z();

	data[2][0] = row2.x();
	data[2][1] = row2.y();
	data[2][2] = row2.z();
}

inline double&Matrix3::operator()(unsigned int r, unsigned int c){ return data[r][c]; }
inline double&Matrix3::operator()(unsigned int r, unsigned int c)const { return data[r][c]; }

inline Vector operator*(const Matrix3&a, const Vector&b){
	return Vector(b[0] * a(0, 0) + b[1] * a(0, 1) + b[2] * a(0, 2),
		          b[0] * a(1, 0) + b[1] * a(1, 1) + b[2] * a(1, 2), 
				  b[0] * a(2, 0) + b[1] * a(2, 1) + b[2] * a(2, 2));
}

inline Vector operator*(const Vector&a, const Matrix3&b){
	return Vector(a[0] * b(0, 0) + a[1] * b(1, 0) + a[2] * b(2, 0),
		          a[0] * b(0, 1) + a[1] * b(1, 1) + a[2] * b(2, 1),
		          a[0] * b(0, 2) + a[1] * b(1, 2) + a[2] * b(2, 2));
}

inline Matrix3 operator*(const Matrix3&a, const Matrix3&b)
{
	Matrix3 dst;
	for (int i = 0; i < 3; i++){
		dst(i, 0) = a(i, 0) * b(0, 0) + a(i, 1) * b(1, 0) + a(i, 2) * b(2, 0);
		dst(i, 1) = a(i, 0) * b(0, 1) + a(i, 1) * b(1, 1) + a(i, 2) * b(2, 1);
		dst(i, 2) = a(i, 0) * b(0, 2) + a(i, 1) * b(1, 2) + a(i, 2) * b(2, 2);
	}
	return dst;
}

inline Matrix3 inverse(const Matrix3&src)
{
	Matrix3 dst;
	double det = src(0, 0) * src(1, 1) * src(2, 2) +
		         src(1, 0) * src(2, 1) * src(0, 2) +
		         src(2, 0) * src(0, 1) * src(1, 2) -
		         src(0, 0) * src(2, 1) * src(1, 2) -
		         src(2, 0) * src(1, 1) * src(0, 2) -
		         src(1, 0) * src(0, 1) * src(2, 2);

	if (det != 0)
	{
		double invDet = 1.0 / det;
		dst(0, 0) = (src(1, 1) * src(2, 2) - src(1, 2) * src(2, 1)) * invDet;
		dst(0, 1) = (src(0, 2) * src(2, 1) - src(0, 1) * src(2, 2)) * invDet;
		dst(0, 2) = (src(0, 1) * src(1, 2) - src(0, 2) * src(1, 1)) * invDet;

		dst(1, 0) = (src(1, 2) * src(2, 0) - src(1, 0) * src(2, 2)) * invDet;
		dst(1, 1) = (src(0, 0) * src(2, 2) - src(0, 2) * src(2, 0)) * invDet;
		dst(1, 2) = (src(0, 2) * src(1, 0) - src(0, 0) * src(1, 2)) * invDet;

		dst(2, 0) = (src(1, 0) * src(2, 1) - src(1, 1) * src(2, 0)) * invDet;
		dst(2, 1) = (src(0, 1) * src(2, 0) - src(0, 0) * src(2, 1)) * invDet;
		dst(2, 2) = (src(0, 0) * src(1, 1) - src(0, 1) * src(1, 0)) * invDet;
	}

	return dst;
}

inline Matrix3 transpose(const Matrix3&src)
{
	Matrix3 dst;
	for (int i = 0; i < 3; i++){
		dst(i, 0) = src(0, i);
		dst(i, 1) = src(1, i);
		dst(i, 2) = src(2, i);
	}
	return dst;

}
#endif