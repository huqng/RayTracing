#pragma once
#include"func.h"
#include"hit_record.h"
#include"texture.h"
#include"ray.h"

class material;
class lambertian;
class metal;
class dielectric;

class material {
public:
	virtual bool scatter(const ray& r_in,  const hit_record& rec, vec3& attenuation, ray& scattered)const = 0;
};

class lambertian :public material {
public:
	texture* albedo;	// ·´ÉäÂÊ

	lambertian(texture* a) :albedo(a) {}
	virtual bool scatter(const ray& r_in,  const hit_record& rec, vec3& attenuation, ray& scattered)const {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p, r_in.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
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
	virtual bool scatter(const ray& r_in,  const hit_record& rec, vec3& attenuation, ray& scattered)const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
		return true;
	}
};

class dielectric :public material {
public:
	double ref_idx;
	dielectric(double ri) :ref_idx(ri) {}
	virtual bool scatter(const ray& r_in,  const hit_record& rec, vec3& attenuation, ray& scattered)const {
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

