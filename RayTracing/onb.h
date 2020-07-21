#pragma once
#include"vec3.h"

// An Ortho-normal basis(ONB) is a collection of 3 mutually orthogonal unit vectors

class onb {
public:
	vec3 axis[3];	

	onb() {}
	inline vec3 operator[](int i) { return axis[i]; }
	vec3 u()const { return axis[0]; }
	vec3 v()const { return axis[1]; }
	vec3 w()const { return axis[2]; }
	vec3 local(double a, double b, double c)const {
		return a * axis[0] + b * axis[1] + c * axis[2];
	}
	vec3 local(const vec3& a)const {
		return a[0] * axis[0] + a[1] * axis[1] + a[2] * axis[2];
	}
	void build_from_w(const vec3& n) {
		axis[2] = unit_vector(n);
		vec3(a);
		if (fabs(n[0]) > 0.9)
			a = vec3(0, 1, 1);
		else
			a = vec3(1, 0, 0);
		axis[1] = unit_vector(cross(n, a));
		axis[0] = cross(axis[2], axis[1]);
	}
};