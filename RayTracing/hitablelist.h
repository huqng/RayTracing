#pragma once
#include"hitable.h"
class hitable_list :public hitable {
public:
	hitable** list;
	int list_size;
	hitable_list() {}
	hitable_list(hitable** l, int n) { list = l; list_size = n; }
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec)const;
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
