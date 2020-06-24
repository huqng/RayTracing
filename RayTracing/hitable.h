#pragma once
#include"ray.h"
#include"func.h"

struct hit_record;
class material;
class hitable;
class lambertian;
class metal;
class texture;
class constant_texture;

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

class texture {
public:
	virtual vec3 value(double u, double v, const vec3& p)const = 0;
};

class constant_texture :public texture {
public:
	vec3 color;

	constant_texture() {}
	constant_texture(vec3 c) :color(c) {}
	virtual vec3 value(double u, double v, const vec3& p)const {
		return color;
	}
};

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const = 0;
};

class lambertian :public material {
public:
	texture* albedo;	// ������
	
	lambertian(texture* a) :albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo->value(0, 0, rec.p);
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
		double reflect_prob;
		double cosine;
		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
			reflect_prob = schlick(cosine, ref_idx);
		}
		else {
			scattered = ray(rec.p, reflected);
			reflect_prob = 1;
		}
		if (rand1() < reflect_prob) {
			scattered = ray(rec.p, reflected);
		}
		else {
			scattered = ray(rec.p, refracted);
		}

		return true;
	}
};

