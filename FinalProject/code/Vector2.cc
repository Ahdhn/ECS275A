
#include "Vector.h"
#include <iostream>

std::istream& operator>>(std::istream& strm, const Vector& v)
{
	return strm;
}

std::ostream& operator<<(std::ostream& out, const Vector& v)
{
  out << '[' << v.x() << ", " << v.y() << ", " << v.z() << ']';
  return out;
}
