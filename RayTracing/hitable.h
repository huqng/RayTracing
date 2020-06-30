#pragma once
#include"material.h"
#include"aabb.h"

class hitable;


class hitable {
public:
	material* mat;
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const = 0;
	virtual bool bounding_box(double t0, double t1, aabb& box)const = 0;
};

