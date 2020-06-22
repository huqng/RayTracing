#include"ray.h"
#include<iostream>
#include<fstream>
#include<Windows.h>
using namespace std;
// 图片的尺寸
#define WIDTH 1280					
#define HEIGHT 800
// 图片文件位置
#define FILENAME "D:\\DATA\\0.bmp"

double hit_sphere(const vec3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	double a = dot(r.direction(), r.direction());
	double b = 2 * dot(oc, r.direction());
	double c = dot(oc, oc) - radius * radius;
	double d = b * b - 4 * a * c;
	if (d < 0)
		return -1;
	else {
		return (-b - sqrt(d)) / (2 * a);
	}
}

vec3 color(const ray& r) {
	double t = hit_sphere(vec3(0, 0, -1), 0.5, r);
	if (t > 0) {
		vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1));
		return 255.99 / 2 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
	}
	vec3 unit_direction = unit_vector(r.direction());
	t = 0.5 * (unit_direction.y() + 1);
	return (1 - t) * vec3(255.99, 255.99, 255.99) + t * vec3(100, 150, 255.99);
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

	// 模拟一个三维空间，摄像机在原点，看向-z。
	// 屏幕在z=-1处，以+y为上，屏幕的高度为2（-1 to +1），宽度根据图片长宽比决定
	vec3 lower_left_corner(-((double)WIDTH / HEIGHT), -1, -1);	
	vec3 horizontal(((double)WIDTH / HEIGHT) * 2, 0, 0);
	vec3 vertical(0, 2, 0);
	vec3 origin(0, 0, 0);

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			double u = double(j) / WIDTH;
			double v = double(i) / HEIGHT;
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			vec3 col = color(r);
		//	bm[i][j] = *(RGBTRIPLE*)col.e;
			bm[i][j].rgbtRed = col.r();
			bm[i][j].rgbtGreen = col.g();
			bm[i][j].rgbtBlue = col.b();
		}
	}

	of.write((const char*)&bf, sizeof(BITMAPFILEHEADER));
	of.write((const char*)&bi, sizeof(BITMAPINFOHEADER));
	of.write((const char*)bm, HEIGHT * WIDTH * sizeof(RGBTRIPLE));
	of.close();
	return 0;
}