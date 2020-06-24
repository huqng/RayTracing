//#include"ray.h"
#include"camera.h"
#include"sphere.h"
#include"hitablelist.h"
#include"func.h"
#include<iostream>
#include<fstream>
#include<random>
#include<Windows.h>
using namespace std;

// Í¼Æ¬ÎÄ¼þÎ»ÖÃ
#define FILENAME "D:\\DATA\\0.bmp"


vec3 color(const ray& r, hitable* world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, MAXINT, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		double t = 0.5 * (unit_direction.y() + 1);
		return (1 - t) * vec3(255.99, 255.99, 255.99) + t * vec3(100, 150, 255.99);
	}
}

int render(RGBTRIPLE(*bm)[WIDTH], camera& cam, hitable* world) {
	for (int i = 0; i < HEIGHT; i++) {
		cout << i << " ";
		for (int j = 0; j < WIDTH; j++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < AA; s++) {
				double u = double(j + rand1()) / WIDTH;
				double v = double(i + rand1()) / HEIGHT;
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2);
				col += color(r, world, 0);
			}
			col /= AA;

			col /= 255.99;
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			col *= 255.99;

			bm[i][j].rgbtRed = col.r();
			bm[i][j].rgbtGreen = col.g();
			bm[i][j].rgbtBlue = col.b();
		}
	}
	return 0;
}

hitable* random_scene() {
	int n = 150;
	hitable** list = new hitable * [n + 1];
	list[0] = new sphere(vec3(0, 0, -1000), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int cnt = 1;
	for (int a = -6; a < 6; a++) {
		for (int b = -6; b < 6; b++) {
			double choose_mat = rand1();
			vec3 center(a + 0.9 * rand1(), b + 0.9 * rand1(), 0.2);
			if (choose_mat < 0.8) {
				list[cnt++] = new sphere(center, 0.2, new lambertian(vec3(rand1(), rand1(), rand1())));
			}
			else if (choose_mat < 0.95) {
				list[cnt++] = new sphere(
					center, 0.2,
					new metal(
						vec3(0.5 * (1 + rand1()), 0.5 * (1 + rand1()), 0.5 * (1 + rand1())), 0.5 * rand1()
					)
				);
			}
			else {
				list[cnt++] = new sphere(center, 0.2, new dielectric(1.5));
			}
		}
	}
	list[cnt++] = new sphere(vec3(0, 0, 1), 1, new dielectric(1.5));
	list[cnt++] = new sphere(vec3(-4, 0, 1), 1, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[cnt++] = new sphere(vec3(4, 0, 1), 1, new metal(vec3(0.7, 0.6, 0.5), 0));
	return new hitable_list(list, cnt);

}

int main() {
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	bf.bfType = 19778;
	bf.bfSize = WIDTH * HEIGHT * 3 + 54;
	bf.bfReserved1 = 0;
	bf.bfReserved2 = 0;
	bf.bfOffBits = 54;
	bi.biWidth = WIDTH;
	bi.biHeight = HEIGHT;
	bi.biSize = 40;
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	fstream of(FILENAME, ios::out | ios::binary);
	if (!of.is_open()) {
		cout << "Cannot open file...";
		return -1;
	}

	RGBTRIPLE(*bm)[WIDTH];
	RGBTRIPLE* tmp = new RGBTRIPLE[HEIGHT * WIDTH * sizeof(RGBTRIPLE)];
	bm = (RGBTRIPLE(*)[WIDTH])tmp;

	hitable* list[2];
	list[0] = new sphere(vec3(5, 0, 0), 5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(-5, 0, 0), 5, new metal(vec3(0.6, 0.6, 0.6), 0.2));
	hitable* world = new hitable_list(list, 2);

	vec3 lookfrom(0, 10, 0);
	vec3 lookat(0, 0, 0);
	vec3 vup = vec3(0, 0, 1);
	double vfov = 40;
	double aspect = (double)WIDTH / HEIGHT;
	double dist_to_focus = (lookfrom - lookat).length();
	double aperture = 0;
	camera cam(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus);;
	
	render(bm, cam, world);

	of.write((const char*)&bf, sizeof(BITMAPFILEHEADER));
	of.write((const char*)&bi, sizeof(BITMAPINFOHEADER));
	of.write((const char*)bm, HEIGHT * WIDTH * sizeof(RGBTRIPLE));
	of.close();
	return 0;
}