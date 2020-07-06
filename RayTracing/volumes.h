#pragma once
#include"hitable.h"

class constant_medium :public hitable {
public:
	hitable* boundary;
	double density;
	material* phase_function;

	constant_medium(hitable* b, double d, texture* a) :boundary(b), density(d) { phase_function = new isotrophic(a); }
	bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
		hit_record rec1, rec2;
		if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1)) {				// 光线射入
			if (boundary->hit(r, rec1.t + 0.0001, FLT_MAX, rec2)) {		// 且能射出？
				//if (rec1.t < t_min)
				//	rec1.t = t_min;
				//if (rec1.t > t_max)
				//	rec1.t = t_max;
				//if (rec1.t >= rec2.t)
				//	return false;
				//if (rec1.t < 0)
				//	rec1.t = 0;
				double distance_inside_boundary = (rec2.t - rec1.t) * r.direction().length();
				double hit_distance = -(1 / density) * log(rand1());
				if (hit_distance < distance_inside_boundary) {
					rec.t = rec1.t + hit_distance / r.direction().length();
					rec.p = r.point_at_parameter(rec.t);
					rec.normal = vec3(1, 0, 0);		// 不会用到
					rec.mat_ptr = phase_function;
					return true;
				}
			}
		}
		return false;
	}
	virtual bool bounding_box(double t0, double t1, aabb& box)const {
		return boundary->bounding_box(t0, t1, box);
	}
};