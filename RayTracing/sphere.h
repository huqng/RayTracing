#pragma once
#include"hitable.h"
class sphere :public hitable {
public:
	vec3 center;
	double radius;

	sphere() {}
	sphere(vec3 cen, double r) :center(cen), radius(r) {}
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec)const;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
	vec3 oc = r.origin() - center;
	double a = dot(r.direction(), r.direction());
	double b = 2 * dot(oc, r.direction());
	double c = dot(oc, oc) - radius * radius;
	double d = b * b - 4 * a * c;
	if (d > 0) {
		double t = (-b - sqrt(d)) / (2 * a);
		if (t < t_max && t > t_min) {
			rec.t = t;		
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
		t = -b / a - t;
		if (t < t_max && t > t_min) {
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	return false;
}
