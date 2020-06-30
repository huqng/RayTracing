#pragma once
#include"hitable.h"
class sphere :public hitable {
public:
	vec3 center;
	double radius;

	sphere() {}
	sphere(vec3 cen, double r, material* mat) :center(cen), radius(r) { this->mat = mat; }
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec)const;
	virtual bool bounding_box(double t0, double t1, aabb& box)const;
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
			rec.mat_ptr = mat;
			rec.t = t;		
			rec.p = r.point_at_parameter(t);
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
		t = -b / a - t;
		if (t < t_max && t > t_min) {
			rec.mat_ptr = mat;
			rec.t = t;
			rec.p = r.point_at_parameter(t);
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v); 
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	return false;
}

bool sphere::bounding_box(double t0, double t1, aabb& box)const {
	box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return true;
}