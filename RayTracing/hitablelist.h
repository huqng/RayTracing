#pragma once
#include"hitable.h"
class hitable_list :public hitable {
public:
	hitable** list;
	int list_size;
	hitable_list() {}
	hitable_list(hitable** l, int n) { list = l; list_size = n; }
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec)const;
	virtual bool bounding_box(double t0, double t1, aabb& box)const;
};

bool hitable_list::hit(const ray& r, double tmin, double tmax, hit_record& rec)const {
	hit_record tmp_record;
	bool hit_anything = false;
	double closest_so_far = tmax;
	for (int i = 0; i < list_size; i++) {
		if (list[i]->hit(r, tmin, closest_so_far, tmp_record)) {
			hit_anything = true;
			closest_so_far = tmp_record.t;
			rec = tmp_record;
		}
	}
	return hit_anything;
}

bool hitable_list::bounding_box(double t0, double t1, aabb& box)const {
	// 用来计算boundingbox的
	// 不存在boundingbox的情况
	if (list_size < 1)
		return false;
	aabb temp_box;
	bool first_true = list[0]->bounding_box(t0, t1, temp_box);
	if (!first_true)
		return false;
	else
		box = temp_box;
	for (int i = 1; i < list_size; i++) {
		if (list[i]->bounding_box(t0, t1, temp_box))
			box = surrounding_box(box, temp_box);
		else
			return false;
	}
	return true;
}