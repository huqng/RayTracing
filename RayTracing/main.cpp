#include"func.h"
#include"camera.h"

#include"sphere.h"
#include"moving_sphere.h"
#include"box.h"
#include"bvh.h"
#include"rectangle.h"
#include"rotate.h"
#include"translate.h"
#include"volumes.h"

#include"material.h"
#include"texture.h"

#include<iostream>
#include<fstream>
#include<ctime>
#include<Windows.h>
using namespace std;

// 图片文件位置
#define FILENAME "D:\\DATA\\0.bmp"
// 线程数
#define NThread 8

typedef struct para_t {
	int index;
	RGBTRIPLE(*bm)[WIDTH];
	camera* cam;
	hitable* world;
}para_t;



vec3 color(const ray& r, hitable* world, int depth);
int render(RGBTRIPLE(*bm)[WIDTH], camera& cam, hitable* world);
DWORD WINAPI render_i(LPVOID para);
// some scenes
void cornell_box(hitable** scene, camera** cam, double aspect);

int main() {
	if ((unsigned int)NThread > 64) {
		cerr << "Too many threads..." << endl;
		return -1;
	}
	// 输出文件的缓冲区
	RGBTRIPLE(*bm)[WIDTH];
	RGBTRIPLE* tmp = new RGBTRIPLE[HEIGHT * WIDTH * sizeof(RGBTRIPLE)];
	bm = (RGBTRIPLE(*)[WIDTH])tmp;
	
	// 场景和摄像机
	hitable* scene;
	camera* cam;
	cornell_box(&scene, &cam, (double)(WIDTH / HEIGHT));
		
	// 开始绘制
	render(bm, *cam, scene);
	// BMP文件头
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
	// 打开输出文件
	fstream fout(FILENAME, ios::out | ios::binary);
	if (!fout.is_open()) {
		cout << "Cannot open file...";
		return -2;
	}
	// 输出到文件
	fout.write((const char*)&bf, sizeof(BITMAPFILEHEADER));
	fout.write((const char*)&bi, sizeof(BITMAPINFOHEADER));
	fout.write((const char*)bm, HEIGHT * WIDTH * sizeof(RGBTRIPLE));
	fout.close();
	return 0;
}

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

			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			col *= 255.99;

			bm[i][j].rgbtRed = col.r() > 255 ? 255 : col.r();
			bm[i][j].rgbtGreen = col.g() > 255 ? 255 : col.g();
			bm[i][j].rgbtBlue = col.b() > 255 ? 255 : col.b();
		}
	}
	return 0;
}

vec3 color(const ray& r, hitable* world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, MAXINT, rec)) {
		ray scattered;
		//vec3 attenuation;
		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		double pdf;
		vec3 albedo;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, albedo, scattered, pdf)) {
			return emitted + albedo * rec.mat_ptr->scattering_pdf(r, rec, scattered) * color(scattered, world, depth + 1) / pdf;
		}
		else {
			return emitted;
		}
	}
	else {
		return vec3(0, 0, 0);
	}
}

int render(RGBTRIPLE(*bm)[WIDTH], camera& cam, hitable* world) {
	cout << "Rendering..." << endl;
	//用于计时
	time_t time_start = time(0);
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
	cout << time(0) - time_start << "s used." << endl;
	return 0;
}



void cornell_box(hitable** scene, camera** cam, double aspect) {
	// set scene
	int i = 0;
	hitable** list = new hitable * [8];
	material* red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material* white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material* green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material* light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	
	
	*scene = new hitable_list(list, i);
	// set camera
	vec3 lookfrom(278, 278, -800);					// 视线起点
	vec3 lookat(278, 278, 0);						// 视线终点
	vec3 vup = vec3(0, 1, 0);					// 图片up方向
	double vfov = 40;							// Field of view
//	double aspect = (double)WIDTH / HEIGHT;		// 长宽比
	double dist_to_focus = 10;					// 焦距 // 用于失焦模糊
	double aperture = 0;						// 光圈 //
	double t0 = 0;								// 起始时间 // 用于动态模糊
	double t1 = 1;								// 终止时间
	*cam = new camera(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus, t0, t1);
}