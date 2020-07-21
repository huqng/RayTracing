#pragma once
#include"func.h"
#include"hit_record.h"
#include"texture.h"
#include"ray.h"
#include"onb.h"

class material;
class lambertian;
class metal;
class dielectric;
class diffuse_light;
class isotrophic;

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, double& pdf)const = 0;
	virtual double scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered)const {
		return false;
	}
	virtual vec3 emitted(const ray& r_in, const hit_record& rec, double u, double v, const vec3& p)const {
		
		return vec3(0, 0, 0);
	}
};

class lambertian :public material {
public:
	texture* albedo;	// ∑¥…‰¬ 

	lambertian(texture* a) :albedo(a) {}
	double scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered)const {
		double cosine = dot(rec.normal, unit_vector(scattered.direction()));
		if (cosine < 0)cosine = 0;
		return cosine / M_PI;;
	}
	bool scatter(const ray& r_in,  const hit_record& rec, vec3& attenuation, ray& scattered, double& pdf)const {
		onb uvw;
		uvw.build_from_w(rec.normal);
		vec3 direction = uvw.local(random_cosine_direction());
		scattered = ray(rec.p, direction, r_in.time());
		//scattered = ray(rec.p, unit_vector(direction), r_in.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		pdf = dot(uvw.w(), scattered.direction()) / M_PI;

		
		
		//vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		//scattered = ray(rec.p, unit_vector(target - rec.p), r_in.time());
		//attenuation = albedo->value(rec.u, rec.v, rec.p);
		//pdf = dot(rec.normal, scattered.direction()) / M_PI;
		/*vec3 direction;
		do {
			direction = random_in_unit_sphere();
		} while (dot(direction, rec.normal) < 0);
		scattered = ray(rec.p, unit_vector(direction), r_in.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		pdf = 0.5 / M_PI;*/
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
	bool scatter(const ray& r_in,  const hit_record& rec, vec3& attenuation, ray& scattered, double& pdf)const {
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
	bool scatter(const ray& r_in,  const hit_record& rec, vec3& attenuation, ray& scattered, double& pdf)const {
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

class diffuse_light :public material {
public:
	texture* emit;

	diffuse_light(texture* a) :emit(a) {}
	bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, double& pdf)const{ return false; }
	vec3 emitted(const ray& r_in, const hit_record& rec, double u, double v, const vec3& p)const{
		//return emit->value(u, v, p);

		if (dot(rec.normal, r_in.direction()) < 0)
			return emit->value(u, v, p);
		else
			return vec3(0, 0, 0); 
	}
};

class isotrophic :public material {
public:
	texture* albedo;

	isotrophic(texture* a) :albedo(a) {}
	bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, double& pdf)const {
		scattered = ray(rec.p, random_in_unit_sphere());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}
};
