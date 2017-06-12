
#ifndef Color_h
#define Color_h

#include "Math.h"
#include <iosfwd>

class Color {
public:
	Color() {
	}

	Color(const Color& copy) {
		for (int i = 0; i < 3; i++)
			data[i] = copy.data[i];
	}

	Color& operator=(const Color& copy) {
		for (int i = 0; i<3; i++)
			data[i] = copy.data[i];
		return *this;
	}

	~Color() {
	}

	double r() const {
		return data[0];
	}
	double g() const {
		return data[1];
	}
	double b() const {
		return data[2];
	}

	Color(float r, float g, float b) {
		data[0] = r;
		data[1] = g;
		data[2] = b;
	}
	Color operator+(const Color& c) const {
		return Color(data[0] + c.data[0], data[1] + c.data[1], data[2] + c.data[2]);
	}
	Color& operator+=(const Color& c) {
		data[0] += c.data[0];
		data[1] += c.data[1];
		data[2] += c.data[2];
		return *this;
	}
	Color operator-(const Color& c) const {
		return Color(data[0] - c.data[0], data[1] - c.data[1], data[2] - c.data[2]);
	}
	Color& operator-=(const Color& c) {
		data[0] -= c.data[0];
		data[1] -= c.data[1];
		data[2] -= c.data[2];
		return *this;
	}
	Color operator*(const Color& c) const {
		return Color(data[0] * c.data[0], data[1] * c.data[1], data[2] * c.data[2]);
	}
	Color& operator*=(const Color& c) {
		data[0] *= c.data[0];
		data[1] *= c.data[1];
		data[2] *= c.data[2];
		return *this;
	}
	Color operator*(double s) const {
		return Color(data[0] * float(s), data[1] * float(s), data[2] * float(s));
	}
	Color& operator*=(double s) {
		data[0] *= float(s);
		data[1] *= float(s);
		data[2] *= float(s);
		return *this;
	}
	Color operator*(float s) const {
		return Color(data[0] * s, data[1] * s, data[2] * s);
	}
	Color& operator*=(float s) {
		data[0] *= s;
		data[1] *= s;
		data[2] *= s;
		return *this;
	}
	Color operator/(double s) const {
		return Color(data[0] / float(s), data[1] / float(s), data[2] / float(s));
	}
	Color& operator/=(double s) {
		data[0] /= float(s);
		data[1] /= float(s);
		data[2] /= float(s);
		return *this;
	}
	Color operator/(float s) const {
		return Color(data[0] / s, data[1] / s, data[2] / s);
	}
	Color& operator/=(float s) {
		data[0] /= s;
		data[1] /= s;
		data[2] /= s;
		return *this;
	}
	void cut(){
		//get the  max(min(data, 1.0), 0.0) 
		//data is the color component 
		data[0] = (data[0] > 1.0f) ? 1.0f : (data[0] < 0.0f) ? 0.0f : data[0];
		data[1] = (data[1] > 1.0f) ? 1.0f : (data[1] < 0.0f) ? 0.0f : data[1];
		data[2] = (data[2] > 1.0f) ? 1.0f : (data[2] < 0.0f) ? 0.0f : data[2];
	}

	float maxComponent() const {
		return Max(data[0], data[1], data[2]);
	}

private:
	float data[3];
};

std::ostream& operator<<(std::ostream&, const Color& c);

#endif
