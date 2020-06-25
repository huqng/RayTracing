#pragma once
#include"ray.h"
#include"func.h"

// 图片的尺寸
#define WIDTH 1280
#define HEIGHT 800
//抗锯齿
#define AA 30

class camera {
public:
	vec3 origin, lower_left_corner, horizontal, vertical;
	vec3 u, v, w;
	double lens_radius;

	camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect, double aperture, double focus_dist) {
		// 从lookfrom看向lookat，vup为图片上方向
		// vfov是图片上下的视角
		// aspect指图片长宽比
		lens_radius = aperture / 2;
		double theta = vfov * PI / 180;
		double half_height = tan(theta / 2);
		double half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
		horizontal = 2 * half_width * focus_dist * u;
		vertical = 2 * half_height * focus_dist * v;
	}

	ray get_ray(double s, double t) {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
	}
};