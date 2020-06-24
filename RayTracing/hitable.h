#pragma once
#include"ray.h"
#include"func.h"

struct hit_record;
class material;
class hitable;
class lambertian;
class metal;

struct hit_record {
	double t;
	vec3 p;
	vec3 normal;
	material* mat_ptr;
};

class hitable {
public:
	material* mat;

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const = 0;
};

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const = 0;
};

class lambertian :public material {
public:
	vec3 albedo;	// ·´ÉäÂÊ
	
	lambertian(const vec3& a) :albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
};

class metal :public material {
public:
	vec3 albedo;
	double fuzz;

	metal(const vec3& a, double f) :albedo(a) {
		if (f >= 0 && f < 1)
			fuzz = f;
		else
			fuzz = 1;
	}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
};

class dielectric :public material {
public:
	double ref_idx;
	dielectric(double ri) :ref_idx(ri) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const {
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		double ni_over_nt;
		attenuation = vec3(1, 1, 1);
		vec3 refracted;
		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1 / ref_idx;
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
			scattered = ray(rec.p, refracted);
		}
		else {
			scattered = ray(rec.p, reflected);
			return false;
		}
		return true;
	}
};