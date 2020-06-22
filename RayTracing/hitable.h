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

	metal(const vec3& a) :albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
};