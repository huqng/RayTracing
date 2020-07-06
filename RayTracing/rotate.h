#pragma once
#include"hitable.h"

//class rotate_x;
class rotate_y;
//class rotate_z;

class rotate_y :public hitable {
public:
	hitable* ptr;
	double sin_theta, cos_theta;
	bool hasbox;
	aabb bbox;

	rotate_y(hitable* p, double angle);
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const;
	virtual bool bounding_box(double t0, double t1, aabb& box)const {
		box = bbox;
		return hasbox;
	}
};

rotate_y::rotate_y(hitable* p, double angle) :ptr(p) {
	double radians = (PI / 180) * angle;
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	hasbox = ptr->bounding_box(0, 1, bbox);
	vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {		// 对8个顶点
				double x = i * bbox.max().x() + (1 - i) * bbox.min().x();
				double y = j * bbox.max().y() + (1 - j) * bbox.min().y();
				double z = k * bbox.max().z() + (1 - k) * bbox.min().z();
				double newx = cos_theta * x + sin_theta * z;
				double newz = -sin_theta * x + cos_theta * z;
				vec3 tester(newx, y, newz);
				for (int c = 0; c < 3; c++) {	// 存放min和max的顶点，用于计算aabb
					if (tester[c] > max[c])	
						max[c] = tester[c];
					if (tester[c] < min[c])
						min[c] = tester[c];
				}
			}
		}
	}
	bbox = aabb(min, max);
}

bool rotate_y::hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
	vec3 origin = r.origin();
	vec3 direction = r.direction();
	origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
	origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];
	direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
	direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];
	ray rotated_r(origin, direction, r.time());
	if (ptr->hit(rotated_r, t_min, t_max, rec)) {
		vec3 p = rec.p;
		vec3 normal = rec.normal;
		p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
		p[2] = cos_theta * rec.p[2] - sin_theta * rec.p[0];
		normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
		normal[2] = cos_theta * rec.normal[2] - sin_theta * rec.normal[0];
		rec.p = p;
		rec.normal = normal;
		return true;
	}
	else
		return false;
}
