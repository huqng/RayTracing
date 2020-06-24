#pragma once
#include"vec3.h"

double rand1() {
	return (double)rand() / (RAND_MAX + 1);
}

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2 * vec3(rand1(), rand1(), rand1()) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1);
	return p;
}

vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

bool refract(const vec3& v, const vec3& n, double ni_over_nt, vec3& refracted) {
	vec3 uv = unit_vector(v);
	double dt = dot(uv, n);
	double d = 1 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (d > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(d);
		return true;
	}
	else
		return false;
}