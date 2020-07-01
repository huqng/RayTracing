#pragma once
#include"hitable.h"

class xy_rect;
class xz_rect;
class yz_rect;

class xy_rect :public hitable {
public:
	double x0, x1, y0, y1, k;
	material* mp;

	xy_rect() {}
	xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, material* mat) :x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {}

	virtual bool hit(const ray& r, double t0, double t1, hit_record& rec)const;
	virtual bool bounding_box(double t0, double t1, aabb& box)const {
		box = aabb(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k + 0.0001));
		return true;
	}
};

bool xy_rect::hit(const ray& r, double t0, double t1, hit_record& rec) const {
	double t = (k - r.origin().z()) / r.direction().z();
	if (t < t0 || t > t1)
		return false;
	double x = r.origin().x() + t * r.direction().x();
	double y = r.origin().y() + t * r.direction().y();
	if (x < x0 || x > x1 || y < y0 || y > y1)
		return false;
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;
	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(0, 0, 1);
	return true;
}

class xz_rect :public hitable {
public:
	double x0, x1, z0, z1, k;
	material* mp;

	xz_rect() {}
	xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, material* mat) :x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {}

	virtual bool hit(const ray& r, double t0, double t1, hit_record& rec)const;
	virtual bool bounding_box(double t0, double t1, aabb& box)const {
		box = aabb(vec3(x0, k - 0.0001, z0), vec3(x1, k + 0.0001, z1));
		return true;
	}
};

bool xz_rect::hit(const ray& r, double t0, double t1, hit_record& rec) const {
	double t = (k - r.origin().y()) / r.direction().y();
	if (t < t0 || t > t1)
		return false;
	double x = r.origin().x() + t * r.direction().x();
	double z = r.origin().z() + t * r.direction().z();
	if (x < x0 || x > x1 || z < z0 || z > z1)
		return false;
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(0, 1, 0);
	return true;
}

class yz_rect :public hitable {
public:
	double y0, y1, z0, z1, k;
	material* mp;

	yz_rect() {}
	yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, material* mat) :y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {}

	virtual bool hit(const ray& r, double t0, double t1, hit_record& rec)const;
	virtual bool bounding_boy(double t0, double t1, aabb& boy)const {
		boy = aabb(vec3(k - 0.0001, y0, z0), vec3(k + 0.0001, y1, z1));
		return true;
	}
};

bool yz_rect::hit(const ray& r, double t0, double t1, hit_record& rec) const {
	double t = (k - r.origin().x()) / r.direction().x();
	if (t < t0 || t > t1)
		return false;
	double y = r.origin().y() + t * r.direction().y();
	double z = r.origin().z() + t * r.direction().z();
	if (y < y0 || y > y1 || z < z0 || z > z1)
		return false;
	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(1, 0, 0);
	return true;
}