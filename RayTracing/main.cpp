#include<iostream>
#include<fstream>
#include<Windows.h>
using namespace std;

#define WIDTH 1280
#define HEIGHT 800
#define FILENAME "D:\\DATA\\0.bmp"

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

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			bm[i][j].rgbtRed = (255 * i / HEIGHT);
			bm[i][j].rgbtGreen = (255 * j / WIDTH);
			bm[i][j].rgbtBlue = 51;
		}
	}

	of.write((const char*)&bf, sizeof(BITMAPFILEHEADER));
	of.write((const char*)&bi, sizeof(BITMAPINFOHEADER));
	of.write((const char*)bm, HEIGHT * WIDTH * sizeof(RGBTRIPLE));
	of.close();
	return 0;
}