#pragma once
#include"vec3.h"
#include<cmath>
#include<ctime>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

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

vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2 * vec3(rand1(), rand1(), 0) - vec3(1, 1, 0);
	} while (dot(p, p) >= 1);
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

double schlick(double cosine, double ref_idx) {
	double r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

void get_sphere_uv(const vec3& p, double& u, double& v) {
	double phi = -atan2(p.y(), p.x());
	double theta = asin(p.z());
	u = 1 - (phi + M_PI) / (2 * M_PI);
	v = (theta + M_PI / 2) / M_PI;
}