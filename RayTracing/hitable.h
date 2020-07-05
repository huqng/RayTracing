#pragma once
#include"material.h"
#include"aabb.h"

class hitable;
class flip_normals;

class hitable {
public:
	material* mat;
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const = 0;
	virtual bool bounding_box(double t0, double t1, aabb& box)const = 0;
};

class flip_normals :public hitable {
public:
	hitable* ptr;
	flip_normals(hitable* p) :ptr(p) {}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
		if (ptr->hit(r, t_min, t_max, rec)) {
			rec.normal = -rec.normal;
			return true;
		}
		else
			return false;
	}
	virtual bool bounding_box(double t0, double t1, aabb& box)const {
		return ptr->bounding_box(t0, t1, box);
	}
};