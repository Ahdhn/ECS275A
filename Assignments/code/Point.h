

#include "Vector.h"

#ifndef Point_h
#define Point_h

#include <iosfwd>

class Point {
 public:
  Point() {
  }

  Point(double x, double y, double z) {
    data[0] = x; data[1] = y; data[2] = z;
  }

  Point(const Point& copy) {
    for(int i=0;i<3;i++)
      data[i] = copy.data[i];
  }
  explicit Point(const Vector& copy) {
    data[0] = copy.x(); data[1] = copy.y(); data[2] = copy.z();
  }

  Point& operator=(const Point& copy) {
    for(int i=0;i<3;i++)
      data[i] = copy.data[i];
    return *this;
  }

  ~Point() {
  }

  double x() const {
    return data[0];
  }
  double y() const {
    return data[1];
  }
  double z() const {
    return data[2];
  }

  Vector operator-(const Point& p) const {
    return Vector(data[0]-p.data[0], data[1]-p.data[1], data[2]-p.data[2]);
  }
  Point operator+(const Vector& v) const {
    return Point(data[0]+v.x(), data[1]+v.y(), data[2]+v.z());
  }
  Point operator-(const Vector& v) const {
    return Point(data[0]-v.x(), data[1]-v.y(), data[2]-v.z());
  }

  Point& operator+=(const Vector& v) {
    data[0]+=v.x();
    data[1]+=v.y();
    data[2]+=v.z();
    return *this;
  }

  Point& operator-=(const Vector& v) {
    data[0]-=v.x();
    data[1]-=v.y();
    data[2]-=v.z();
    return *this;
  }
 private:
  double data[3];
};

inline Point operator+(const Vector& v, const Point& p)
{
  return Point(v.x() + p.x(), v.y() + p.y(), v.z() + p.z());
}

inline Point Interpolate(double s2, const Point& p1, const Point& p2)
{
  double s1 = 1-s2;
  return Point(s1*p1.x() + s2*p2.x(), s1*p1.y() + s2*p2.y(),
               s1*p1.z() + s2*p2.z());
}

inline Point AffineCombination(double s1, const Point& p1,
                               double s2, const Point& p2)
{
  return Point(s1*p1.x() + s2*p2.x(), s1*p1.y() + s2*p2.y(),
               s1*p1.z() + s2*p2.z());
}

inline Point AffineCombination(double s1, const Point& p1,
                               double s2, const Point& p2,
                               double s3, const Point& p3)
{
  return Point(s1*p1.x() + s2*p2.x() + s3*p3.x(),
               s1*p1.y() + s2*p2.y() + s3*p3.y(),
               s1*p1.z() + s2*p2.z() + s3*p3.z());
}

inline double Dot(const Point& v1, const Vector& v2)
{
  return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}

inline double Dot(const Vector& v1, const Point& v2)
{
  return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}

inline Point Min(const Point& p1, const Point& p2)
{
  return Point(Min(p1.x(), p2.x()), Min(p1.y(), p2.y()), Min(p1.z(), p2.z()));
}

inline Point Max(const Point& p1, const Point& p2)
{
  return Point(Max(p1.x(), p2.x()), Max(p1.y(), p2.y()), Max(p1.z(), p2.z()));
}

std::ostream& operator<<(std::ostream&, const Point& v1);

#endif

