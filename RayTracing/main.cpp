#include"ray.h"
#include<iostream>
#include<fstream>
#include<Windows.h>
using namespace std;

#define WIDTH 1280
#define HEIGHT 800
#define FILENAME "D:\\DATA\\0.bmp"

vec3 color(const ray& r) {
	vec3 unit_direction = unit_vector(r.direction());
	double t = 0.5 * (unit_direction.y() + 1);
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

	vec3 lower_left_corner(-2, -1, -1);
	vec3 horizontal(4, 0, 0);
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