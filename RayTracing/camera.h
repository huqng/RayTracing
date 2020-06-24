#pragma once
#include"ray.h"

// ͼƬ�ĳߴ�
#define WIDTH 1280			
#define HEIGHT 800
#define AA 100

class camera {
public:
	vec3 origin, lower_left_corner, horizontal, vertical;
	
	camera() {
		// ģ��һ����ά�ռ䣬�������ԭ�㣬����-z��
		// ��Ļ��z=-1������+yΪ�ϣ���Ļ�ĸ߶�Ϊ2��-1 to +1������ȸ���ͼƬ����Ⱦ���
		lower_left_corner = vec3(-((double)WIDTH / HEIGHT), -1, -1);
		horizontal = vec3(((double)WIDTH / HEIGHT) * 2, 0, 0);
		vertical = vec3(0, 2, 0);
		origin = vec3(0, 0, 0);
	}

	ray get_ray(double u, double v) {
		return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}
};