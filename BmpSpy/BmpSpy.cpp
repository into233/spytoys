#include "BmpSpy.h"

#define PI 3.14159
#define LENGTH_NAME_BMP 30
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

BITMAPFILEHEADER strHead;
RGBQUAD strPla[256];
BITMAPINFOHEADER strInfo;

void showBmpHead(BITMAPFILEHEADER pBmpHead) {
	cout << "λͼ�ļ�ͷ��" << endl;
	cout << "�ļ���С��" << pBmpHead.bfSize << endl;
	cout << "������1��" << pBmpHead.bfReserved1 << endl;
	cout << "������2��" << pBmpHead.bfReserved2 << endl;
	cout << "ʵ��λͼ���ݵ�ƫ���ֽ�����" << pBmpHead.bfOffBits << endl << endl;
}

void showBmpInfoHead(tagBITMAPINFOHEADER pBmpinfoHead) {
	cout <<"λͼ��Ϣͷ:\n"<< endl;
	cout <<"�ṹ��ĳ���: \n"<< pBmpinfoHead.biSize<< endl;
	cout <<"λͼ��: \n"<< pBmpinfoHead.biWidth<< endl;
	cout <<"λͼ��: \n"<< pBmpinfoHead.biHeight<< endl;
	cout <<"biPlanesƽ����: \n"<< pBmpinfoHead.biPlanes<< endl;
	cout <<"biBitCount������ɫλ��: \n"<< pBmpinfoHead.biBitCount<< endl;
	cout <<"ѹ����ʽ: \n"<< pBmpinfoHead.biCompression<< endl;
	cout <<"biSizeImageʵ��λͼ����ռ�õ��ֽ���: \n"<< pBmpinfoHead.biSizeImage<< endl;
	cout <<"X����ֱ���: \n"<< pBmpinfoHead.biXPelsPerMeter<< endl;
	cout <<"Y����ֱ���: \n"<< pBmpinfoHead.biYPelsPerMeter<< endl;
	cout <<"ʹ�õ���ɫ��: \n"<< pBmpinfoHead.biClrUsed<< endl;
	cout <<"��Ҫ��ɫ��: \n"<< pBmpinfoHead.biClrImportant<< endl;
}
int main() {
	char strFile[LENGTH_NAME_BMP];
	IMAGEDATA *imagedata = nullptr;
	IMAGEDATA *imagedataRot = nullptr;

	int width, height;

	cout << "��������Ҫ��ȡ�ļ�����" << endl;
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