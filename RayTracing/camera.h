#pragma once
#include"ray.h"

// 图片的尺寸
#define WIDTH 1280
#define HEIGHT 800
//抗锯齿
#define AA 30
#define PI 3.14159265359

class camera {
public:
	vec3 origin, lower_left_corner, horizontal, vertical;
	
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect) {
		// 从lookfrom看向lookat，vup为图片上方向
		// vfov是图片上下的视角
		// aspect指图片长宽比
		vec3 u, v, w;
		double theta = vfov * PI / 180;
		double half_height = tan(theta / 2);
		double half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		lower_left_corner = origin - half_width * u - half_height * v - w;
		horizontal = 2 * half_width * u;
		vertical = 2 * half_height * v;
	}

	ray get_ray(double u, double v) {
		return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}
};