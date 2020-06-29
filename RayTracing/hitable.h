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
class image_texture;

struct hit_record {
	double t;
	vec3 p;
	vec3 normal;
	material* mat_ptr;
	double u, v;
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
	constant_texture(vec3 c) : color(c) {}
	virtual vec3 value(double u, double v, const vec3& p)const {
		return color;
	}
};

class checker_texture :public texture {
public:
	texture* odd, * even;
	checker_texture() {}
	checker_texture(texture* t0, texture* t1) :even(t0), odd(t1) {}
	virtual vec3 value(double u, double v, const vec3& p)const {
		double sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
};

class image_texture :public texture {
public:
	unsigned char* data;
	int nx, ny;

	image_texture() {}
	image_texture(unsigned char* pixels, int A, int B) :data(pixels), nx(A), ny(B) {}
	virtual vec3 value(double u, double v, const vec3& p)const {
		// u v 在0~1之间
		int x = u * nx;
		int y = v * ny;
		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;
		if (x > nx - 1)
			x = nx - 1;
		if (y > ny - 1)
			y = ny - 1;
		double b = data[3 * (nx * y + x)] / 255.99;
		double g = data[3 * (nx * y + x) + 1] / 255.99;
		double r = data[3 * (nx * y + x) + 2] / 255.99;

		return vec3(r, g, b);
	}
};

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const = 0;
};

class lambertian :public material {
public:
	texture* albedo;	// 反射率
	
	lambertian(texture* a) :albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const {
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

