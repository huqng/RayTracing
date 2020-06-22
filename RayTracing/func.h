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