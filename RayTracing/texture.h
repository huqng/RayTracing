#pragma once
#include"vec3.h"

class texture;
class constant_texture;
class checker_texture;
class image_texture;

class texture {
public:
	virtual vec3 value(double u, double v, const vec3& p)const = 0;
};

class constant_texture :public texture {
public:
	vec3 color;

	constant_texture() {}
	constant_texture(vec3 c) : color(c) {}
	virtual vec3 value(double u, double v, const vec3& p)const {
		return color;
	}
};

class checker_texture :public texture {
public:
	texture* odd, * even;
	checker_texture() {}
	checker_texture(texture* t0, texture* t1) :even(t0), odd(t1) {}
	virtual vec3 value(double u, double v, const vec3& p)const {
		double sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
};

class image_texture :public texture {
public:
	unsigned char* data;
	int nx, ny;

	image_texture() {}
	image_texture(unsigned char* pixels, int A, int B) :data(pixels), nx(A), ny(B) {}
	virtual vec3 value(double u, double v, const vec3& p)const {
		// u v ÔÚ0~1Ö®¼ä
		int x = u * nx;
		int y = v * ny;
		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;
		if (x > nx - 1)
			x = nx - 1;
		if (y > ny - 1)
			y = ny - 1;
		double b = data[3 * (nx * y + x)] / 255.99;
		double g = data[3 * (nx * y + x) + 1] / 255.99;
		double r = data[3 * (nx * y + x) + 2] / 255.99;

		return vec3(r, g, b);
	}
};

