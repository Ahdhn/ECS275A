#include "Vector3.h"

std::istream& operator>>(std::istream& strm, Vector3& v)
{
	return strm;
}

std::ostream& operator<<(std::ostream& strm, const Vector3& v)
{
	strm << '[' << v[0] << ',' << v[1] << ',' << v[2] << ']';
	return strm;
}
