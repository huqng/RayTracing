#pragma once
#include"vec3.h"

class material;

struct hit_record {
	double t;
	vec3 p;
	vec3 normal;
	material* mat_ptr;
	double u, v;
};
typedef struct hit_record hit_record;