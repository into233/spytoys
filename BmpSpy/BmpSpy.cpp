#include "BmpSpy.h"

#define PI 3.14159
#define LENGTH_NAME_BMP 30
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

BITMAPFILEHEADER strHead;
RGBQUAD strPla[256];
BITMAPINFOHEADER strInfo;

void showBmpHead(BITMAPFILEHEADER pBmpHead) {
	cout << "位图文件头：" << endl;
	cout << "文件大小：" << pBmpHead.bfSize << endl;
	cout << "保留字1：" << pBmpHead.bfReserved1 << endl;
	cout << "保留字2：" << pBmpHead.bfReserved2 << endl;
	cout << "实际位图数据的偏移字节数：" << pBmpHead.bfOffBits << endl << endl;
}

void showBmpInfoHead(tagBITMAPINFOHEADER pBmpinfoHead) {
	cout <<"位图信息头:\n"<< endl;
	cout <<"结构体的长度: \n"<< pBmpinfoHead.biSize<< endl;
	cout <<"位图宽: \n"<< pBmpinfoHead.biWidth<< endl;
	cout <<"位图高: \n"<< pBmpinfoHead.biHeight<< endl;
	cout <<"biPlanes平面数: \n"<< pBmpinfoHead.biPlanes<< endl;
	cout <<"biBitCount采用颜色位数: \n"<< pBmpinfoHead.biBitCount<< endl;
	cout <<"压缩方式: \n"<< pBmpinfoHead.biCompression<< endl;
	cout <<"biSizeImage实际位图数据占用的字节数: \n"<< pBmpinfoHead.biSizeImage<< endl;
	cout <<"X方向分辨率: \n"<< pBmpinfoHead.biXPelsPerMeter<< endl;
	cout <<"Y方向分辨率: \n"<< pBmpinfoHead.biYPelsPerMeter<< endl;
	cout <<"使用的颜色数: \n"<< pBmpinfoHead.biClrUsed<< endl;
	cout <<"重要颜色数: \n"<< pBmpinfoHead.biClrImportant<< endl;
}
int main() {
	char strFile[LENGTH_NAME_BMP];
	IMAGEDATA *imagedata = nullptr;
	IMAGEDATA *imagedataRot = nullptr;

	int width, height;

	cout << "请输入所要读取文件名：" << endl;
	cin >> strFile;
	FILE *fpi, *fpw;
	fpi = fopen(strFile, "rb");
	if (fpi != NULL) {
		WORD bftype;
		fread(&bftype, 1, sizeof(WORD), fpi);
		if (0x4d42 != bftype)
		{
			cout << "the file is not a bmp file;" << endl;
			return NULL;
		}
		//read bmp file filehead and infohead
		fread(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpi);
		showBmpHead(strHead);
		fread(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpi);
		showBmpInfoHead(strInfo);

		for (unsigned int nCounti = 0; nCounti < strInfo.biClrUsed; nCounti++) {
			fread((char *)&(strPla[nCounti].rgbBlue), 1, sizeof(BYTE), fpi);
			fread((char *)&(strPla[nCounti].rgbGreen), 1, sizeof(BYTE), fpi);
			fread((char *)&(strPla[nCounti].rgbRed), 1, sizeof(BYTE), fpi);
			fread((char *)&(strPla[nCounti].rgbReserved), 1, sizeof(BYTE), fpi);
		}
		width = strInfo.biWidth;
		height = strInfo.biHeight;

		width = (width * sizeof(IMAGEDATA) + 3) / 4 * 4;
		//imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));
		imagedata = (IMAGEDATA*)malloc(width * height);
		imagedataRot = (IMAGEDATA*)malloc(2 * width * 2 * height * sizeof(IMAGEDATA));
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				(*(imagedata + i * width + j)).blue = 0;
//				(*(imagedata + i * width + j)).green = 0;
//				(*(imagedata + i * width + j)).blue = 0;
			}
		}
		for (int i = 0; i < 2 * height; ++i) {
			for (int j = 0; j < 2 * width; ++j)
			{
				(*(imagedataRot + i * 2 * width + j)).blue = 0;
				(*(imagedataRot + i * 2 * width + j)).red = 0;
				(*(imagedataRot + i * 2 * width + j)).green = 0;
			}
		}
		//fseed(fpi, 54, SEEK_SET);

	}
	
	getchar();
	getchar();
	getchar();
	getchar();
	getchar();
}