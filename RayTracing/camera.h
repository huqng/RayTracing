#pragma once
#include"ray.h"

// 图片的尺寸
#define WIDTH 1280			
#define HEIGHT 800
#define AA 100

class camera {
public:
	vec3 origin, lower_left_corner, horizontal, vertical;
	
	camera() {
		// 模拟一个三维空间，摄像机在原点，看向-z。
		// 屏幕在z=-1处，以+y为上，屏幕的高度为2（-1 to +1），宽度根据图片长宽比决定
		lower_left_corner = vec3(-((double)WIDTH / HEIGHT), -1, -1);
		horizontal = vec3(((double)WIDTH / HEIGHT) * 2, 0, 0);
		vertical = vec3(0, 2, 0);
		origin = vec3(0, 0, 0);
	}

	ray get_ray(double u, double v) {
		return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}
};