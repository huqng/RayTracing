#pragma once
#include"hitable.h"

class translate :public hitable {
public:
	vec3 offset;
	hitable* ptr;

	translate(hitable* p, const vec3& displacement) :ptr(p), offset(displacement) {}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const;
	virtual bool bounding_box(double t0, double t1, aabb& box)const;
};

bool translate::hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
	ray moved_r(r.origin() - offset, r.direction(), r.time());
	if (ptr->hit(moved_r, t_min, t_max, rec)) {
		rec.p += offset;
		return true;
	}
	else
		return false;
}

bool translate::bounding_box(double t0, double t1, aabb& box)const {
	if (ptr->bounding_box(t0, t1, box)) {
		box = aabb(box.min() + offset, box.max() + offset);
		return true;
	}
	else
		return false;
}