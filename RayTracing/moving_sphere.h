#pragma once
#include"hitable.h"

class moving_sphere :public hitable {
public:
	vec3 center(double time)const;
	vec3 center0, center1;
	double time0, time1;
	double radius;
	
	moving_sphere() {}
	moving_sphere(vec3 cen0, vec3 cen1, double t0, double t1, double r, material* m) :center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r) { this->mat = m; }
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
		vec3 oc = r.origin() - center(r.time());
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
				get_sphere_uv((rec.p - center(r.time())) / radius, rec.u, rec.v);
				rec.normal = (rec.p - center(r.time())) / radius;
				return true;
			}
			t = -b / a - t;
			if (t < t_max && t > t_min) {
				rec.mat_ptr = mat;
				rec.t = t;
				rec.p = r.point_at_parameter(t);
				get_sphere_uv((rec.p - center(r.time())) / radius, rec.u, rec.v);
				rec.normal = (rec.p - center(r.time())) / radius;
				return true;
			}
		}
		return false;
	}
	virtual bool bounding_box(double t0, double t1, aabb& box)const {
		aabb box0 = aabb(center0 - vec3(radius, radius, radius), center0 + vec3(radius, radius, radius));
		aabb box1 = aabb(center1 - vec3(radius, radius, radius), center1 + vec3(radius, radius, radius));
		box = surrounding_box(box0, box1);
		return true;
	}
};

vec3 moving_sphere::center(double time)const {
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}