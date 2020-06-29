//#include"ray.h"
#include"camera.h"
#include"sphere.h"
#include"moving_sphere.h"
#include"hitablelist.h"
#include"func.h"
#include<iostream>
#include<fstream>
#include<random>
#include<Windows.h>
using namespace std;

// Í¼Æ¬ÎÄ¼þÎ»ÖÃ
#define FILENAME "D:\\DATA\\0.bmp"
#define NThread 64

typedef struct para_t {
	int index;
	RGBTRIPLE(*bm)[WIDTH];
	camera* cam;
	hitable* world;
}para_t;

vec3 color(const ray& r, hitable* world, int depth);

DWORD WINAPI render_i(LPVOID para) {
	para_t s = *(para_t*)para;
	hitable* world = s.world;
	camera* cam = s.cam;

	RGBTRIPLE(*bm)[WIDTH] = s.bm;
	for (int i = HEIGHT * s.index / NThread; i < HEIGHT * (s.index + 1) / NThread; i++) {
		for (int j = 0; j < WIDTH; j++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < AA; s++) {
				double u = double(j + rand1()) / WIDTH;
				double v = double(i + rand1()) / HEIGHT;
				ray r = cam->get_ray(u, v);
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
		return vec3(180, 200, 200);
		//	vec3 unit_direction = unit_vector(r.direction());
		//	double t = 0.5 * (unit_direction.y() + 1);
		//	return (1 - t) * vec3(255.99, 255.99, 255.99) + t * vec3(100, 150, 255.99);
	}
}

int render(RGBTRIPLE(*bm)[WIDTH], camera& cam, hitable* world) {

	HANDLE hThread[NThread];
	para_t paras[NThread];
	for (int i = 0; i < NThread; i++) {
		paras[i].index = i;
		paras[i].cam = &cam;
		paras[i].world = world;
		paras[i].bm = bm;
		hThread[i] = CreateThread(0, 0, render_i, &paras[i], 0, 0);
	}
	WaitForMultipleObjects(NThread, hThread, 1, INFINITE);
	return 0;
}

hitable* random_scene() {
	texture* checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
	int n = 500;
	hitable** list = new hitable * [n];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
	int i = 1;
	for (int a = -10; a < 10; a++) {
		for (int b = -10; b < 10; b++) {
			double choose_mat = rand1();
			vec3 center(a + 0.9 * rand1(), 0.2, b + 0.9 * rand1());
			if ((center - vec3(4, 0.2, 0)).squared_length() > 0.81) {
				if (choose_mat < 0.8) {
					list[i++] = new moving_sphere(center, center + vec3(0, 0.5 * rand1(), 0), 0, 1, 0.2, new lambertian(new constant_texture(vec3(rand1(), rand1(), rand1()))));
				}
				else if (choose_mat < 0.95) {
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + rand1()), 0.5 * (1 + rand1()), 0.5 * (1 + rand1())), 0.5 * rand1()));
				}
				else {
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}
	list[i++] = new sphere(vec3(0, 1, 0), 1, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new sphere(vec3(4, 1, 0), 1, new metal(vec3(0.7, 0.6, 0.5), 0));
	return new hitable_list(list, i);
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

	fstream fout(FILENAME, ios::out | ios::binary);
	if (!fout.is_open()) {
		cout << "Cannot open file...";
		return -1;
	}

	RGBTRIPLE(*bm)[WIDTH];
	RGBTRIPLE* tmp = new RGBTRIPLE[HEIGHT * WIDTH * sizeof(RGBTRIPLE)];
	bm = (RGBTRIPLE(*)[WIDTH])tmp;

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	vec3 vup = vec3(0, 1, 0);
	double vfov = 20;
	double aspect = (double)WIDTH / HEIGHT;
	double dist_to_focus = 10;
	double aperture = 0;
	camera cam(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus, 0, 1);

	render(bm, cam, random_scene());

	fout.write((const char*)&bf, sizeof(BITMAPFILEHEADER));
	fout.write((const char*)&bi, sizeof(BITMAPINFOHEADER));
	fout.write((const char*)bm, HEIGHT * WIDTH * sizeof(RGBTRIPLE));
	fout.close();
	return 0;
}