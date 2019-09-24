#include "BmpSpy.h"

#define PI 3.14159
#define LENGTH_NAME_BMP 30
#define _CRT_SECURE_NO_WARNINGS
#define CHAR_NUM 16

BITMAPFILEHEADER strHead;
RGBQUAD strPla[256];
BITMAPINFOHEADER strInfo;

using namespace std;


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
int rotateBMPimage(char filename[], IMAGEDATA *imagedata, IMAGEDATA *imagedataRot) {
	int width, height;
	FILE *fpw;
	FILE *fpi;
	fpi = fopen(filename, "rb");
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

		initPla(fpi);
		width = strInfo.biWidth;
		height = strInfo.biHeight;

		//+3��Ϊ�˲���...
		width = (width * sizeof(IMAGEDATA) + 3) / 4 * 4;
		imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));
		//imagedata = (IMAGEDATA*)malloc(width * height);
		imagedataRot = (IMAGEDATA*)malloc(2 * width * 2 * height * sizeof(IMAGEDATA));
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				(*(imagedata + i * width + j)).blue = 0;
				(*(imagedata + i * width + j)).green = 0;
				(*(imagedata + i * width + j)).blue = 0;
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
		//fseek(fpi, 54, SEEK_SET);+b+yy
		fread(imagedata, sizeof(struct tagIMAGEDATA) * width, height, fpi);
	}
	else {
		cout << "file open error!" << endl;
		return NULL;
	}

	int RotateAngle;
	double angle;
	int midX_pre, midY_pre, midX_aft, midY_aft;
	midX_pre = width / 2;
	midY_pre = height / 2;
	midX_aft = width;
	midY_aft = height;

	int pre_i, pre_j, after_i, after_j;
	cout << "����Ҫ��ת�ĽǶ�(0-360, ��ʱ����ת)";
	cin >> RotateAngle;
	angle = 1.0 * RotateAngle * PI / 180;
	for (int i = 0; i < 2 * height; ++i)
	{
		for (int j = 0; j < 2 * width; ++j) {
			after_i = i - midY_aft;
			after_j = j - midX_aft;
			pre_i = (int)(cos((double)angle) * after_i - sin((double)angle)*after_j) + midX_pre;
			pre_j = (int)(sin((double)angle) * after_i + cos((double)angle)*after_j) + midY_pre;
			if (pre_i >= 0 && pre_i < height && pre_j >= 0 && pre_j < width)
				*(imagedataRot + i * 2 * width + j) = *(imagedata + pre_i * width + pre_j);
		}
	}
	//save
	if ((fpw = fopen("b.bmp", "wb")) == NULL) {
		cout << "create the bmp file error!" << endl;
		return NULL;
	}
	WORD bfType_w = 0x4d42;
	fwrite(&bfType_w, 1, sizeof(WORD), fpw);
	//fpw += 2;
	fwrite(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpw);
	strInfo.biWidth = 2 * strInfo.biWidth;
	strInfo.biHeight = 2 * height;
	fwrite(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpw);
	//save clolor palette
	for (unsigned int nCounti = 0; nCounti < strInfo.biClrUsed; nCounti++) {
		fwrite(&strPla[nCounti].rgbBlue, 1, sizeof(BYTE), fpw);
		fwrite(&strPla[nCounti].rgbGreen, 1, sizeof(BYTE), fpw);
		fwrite(&strPla[nCounti].rgbRed, 1, sizeof(BYTE), fpw);
		fwrite(&strPla[nCounti].rgbReserved, 1, sizeof(BYTE), fpw);
	}
	for (int i = 0; i < 2 * height; ++i) {
		for (int j = 0; j < 2 * width; ++j) {
			fwrite(&((*(imagedataRot + i * 2 * width + j)).blue), 1, sizeof(BYTE), fpw);
			fwrite(&((*(imagedataRot + i * 2 * width + j)).green), 1, sizeof(BYTE), fpw);
			fwrite(&((*(imagedataRot + i * 2 * width + j)).red), 1, sizeof(BYTE), fpw);
		}
	}
	fclose(fpw);
	delete[] imagedata;
	delete[] imagedataRot;
	cout << "done!";
}

int readImageData(FILE *fpi) {
	WORD bftype;
	fread(&bftype, 1, sizeof(WORD), fpi);
	if (0x4d42 != bftype)
	{
		cout << "the file is not a bmp file;" << endl;
		return NULL;
	}
	//read bmp file filehead and infohead
	fread(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpi);
	fread(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpi);
	return 1;
}
int initPla(FILE *fpi) {
	for (unsigned int nCounti = 0; nCounti < strInfo.biClrUsed; nCounti++) {
		fread((char *)&(strPla[nCounti].rgbBlue), 1, sizeof(BYTE), fpi);
		fread((char *)&(strPla[nCounti].rgbGreen), 1, sizeof(BYTE), fpi);
		fread((char *)&(strPla[nCounti].rgbRed), 1, sizeof(BYTE), fpi);
	}
	return 1;
}
int savePic(FILE *fpw, char output_filename[], IMAGEDATA *imagedata) {
	int width, height;

	width = strInfo.biWidth;
	height = strInfo.biHeight;

	if ((fpw = fopen(output_filename, "wb")) == NULL) {
		cout << "create the crpto bmp file error!" << endl;
		return NULL;
	}
	WORD bfType_w = 0x4d42;
	fwrite(&bfType_w, 1, sizeof(WORD), fpw);
	fwrite(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpw);
	fwrite(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpw);
	//save clolor palette
	for (unsigned int nCounti = 0; nCounti < strInfo.biClrUsed; nCounti++) {
		fwrite(&strPla[nCounti].rgbBlue, 1, sizeof(BYTE), fpw);
		fwrite(&strPla[nCounti].rgbGreen, 1, sizeof(BYTE), fpw);
		fwrite(&strPla[nCounti].rgbRed, 1, sizeof(BYTE), fpw);
		fwrite(&strPla[nCounti].rgbReserved, 1, sizeof(BYTE), fpw);
	}
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			fwrite(&((*(imagedata + i * width + j)).blue), 1, sizeof(BYTE), fpw);
			fwrite(&((*(imagedata + i * width + j)).green), 1, sizeof(BYTE), fpw);
			fwrite(&((*(imagedata + i * width + j)).red), 1, sizeof(BYTE), fpw);
		}
	}
	fclose(fpw);
	delete[] imagedata;
	cout << "done!";
	return 1;
}
int bitManipulation0(unsigned char n, unsigned char m, int i) {
	int temp = m >> i & 1;
	if (temp == 1)
		return n | (temp << i);
	else
	{
		temp = 1;
		return n & (~(temp << i));
	}
}

int bitManipulation(unsigned char des, unsigned char src, int i) {
	int temp = src >> i & 1;
	if (temp == 1)
		return des | temp;
	else
	{
		temp = 1;
		return des & (~temp);
	}
}
int bitManipulation_reverse(unsigned char n, unsigned char m, int i) {
	int temp = m  & 1;
	if (temp == 1)
		return n |(temp << i);
	else
	{
		temp = 1;
		return n & (~(temp << i));
	}
}

//���ֽڴ�IMAGEDATA  n/m
int lowestBitReplace_plus(char origin_Filename[], char output_filename[], IMAGEDATA *imagedata, int ndm) {

	FILE *fpi = fopen(origin_Filename, "rb");
	FILE *fpw = nullptr;
	int width, height;
	if (fpi != NULL) {
		readImageData(fpi);
		showBmpInfoHead(strInfo);
		showBmpHead(strHead);
	}
	else {
		cout << "file open error!" << endl;
		return -1;
	}
	initPla(fpi);
	width = strInfo.biWidth;
	height = strInfo.biHeight;

	imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));
	BYTE* B_imagedata = (BYTE*)imagedata;

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			(*(imagedata + i * width + j)).blue = 0;
			(*(imagedata + i * width + j)).green = 0;
			(*(imagedata + i * width + j)).red = 0;
		}
	}
	fread(imagedata, sizeof(struct tagIMAGEDATA) * width, height, fpi);
	char crypto[CHAR_NUM * 16] = { 0 };
	cout << "����������ַ���: ";
	cin.getline(crypto, CHAR_NUM * 16);
	BYTE shift_bits = 0x00;
	int j = 0;
	int string_len = strlen(crypto);

	//�ַ�������
	for (int i = 0; i < CHAR_NUM; i++) {
		B_imagedata[i * ndm] = bitManipulation(B_imagedata[i * ndm], string_len, shift_bits++);
	}
	shift_bits = 0x00;
	for (int i = CHAR_NUM;; ++i) {
		B_imagedata[i * ndm] = bitManipulation(B_imagedata[i * ndm], crypto[j], shift_bits);
		if (++shift_bits == 8) {
			shift_bits = 0;
			if (crypto[++j] == '\0')
				break;
		}
	}
	fclose(fpi);
	if (savePic(fpw, output_filename, imagedata)) {
		return 1;
	}
	return 0;

}

//int lowestBitReplace(char origin_Filename[], char output_filename[], IMAGEDATA *imagedata) {
//	FILE *fpi = fopen(origin_Filename, "rb");
//	FILE *fpw = nullptr;
//	int width, height;
//	if (fpi != NULL) {
//		readImageData(fpi);
//		showBmpInfoHead(strInfo);
//		showBmpHead(strHead);
//	}
//	else {
//		cout << "file open error!" << endl;
//		return -1;
//	}
//	initPla(fpi);
//	width = strInfo.biWidth;
//	height = strInfo.biHeight;
//
//	imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));
//	//imagedata = (IMAGEDATA*)malloc(width * height);
//	for (int i = 0; i < height; ++i) {
//		for (int j = 0; j < width; ++j) {
//			(*(imagedata + i * width + j)).blue = 0;
//			(*(imagedata + i * width + j)).green = 0;
//			(*(imagedata + i * width + j)).blue = 0;
//		}
//	}
//	//fseek(fpi, 54, SEEK_SET);+b+yy
//	fread(imagedata, sizeof(struct tagIMAGEDATA) * width, height, fpi);
//
//	//�������16λ�ļ���λ
//	char crypto[CHAR_NUM * 16] = { 0 };
//	cout << "����������ַ���: ";
//	cin.getline(crypto, CHAR_NUM * 16);
//	BYTE shift_bits = 0x00;
//	int j = 0;
//	int string_len = strlen(crypto);
//
//	//����
//	for (int i = 0; i < CHAR_NUM; ++i) {
//		imagedata[i].blue = bitManipulation(imagedata[i].blue, string_len, shift_bits++);
//		if (shift_bits == CHAR_NUM) {
//			break;
//		}
//		imagedata[i].green = bitManipulation(imagedata[i].green, string_len, shift_bits++);
//		imagedata[i].red = bitManipulation(imagedata[i].red, string_len, shift_bits++);
//	}
//	shift_bits = 0x00;
//	for (int i = CHAR_NUM;; ++i) {
//		imagedata[i].blue = bitManipulation(imagedata[i].blue, crypto[j], shift_bits);
//		if (++shift_bits == 8) {
//			shift_bits = 0;
//			if (crypto[++j] == '\0')
//				break;
//		}
//		imagedata[i].green = bitManipulation(imagedata[i].green, crypto[j], shift_bits);
//		if (++shift_bits == 8) {
//			shift_bits = 0;
//			if (crypto[++j] == '\0')
//				break;
//		}
//		imagedata[i].red = bitManipulation(imagedata[i].red, crypto[j], shift_bits);
//		if (++shift_bits == 8) {
//			shift_bits = 0;
//			if (crypto[++j] == '\0')
//				break;
//		}
//	}
//	fclose(fpi);
//	if (savePic(fpw, output_filename, imagedata)) {
//		return 1;
//	}
//	return 0;
//}

int read_encript_pic(char origin_Filename[], IMAGEDATA *imagedata) {
	FILE *fpi = fopen(origin_Filename, "rb");
	int width, height;
	if (fpi != NULL) {
		readImageData(fpi);
		showBmpInfoHead(strInfo);
		showBmpHead(strHead);
	}
	else {
		cout << "file open error!" << endl;
		return -1;
	}
	initPla(fpi);
	width = strInfo.biWidth;
	height = strInfo.biHeight;

	imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));
	//imagedata = (IMAGEDATA*)malloc(width * height);
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			(*(imagedata + i * width + j)).blue = 0;
			(*(imagedata + i * width + j)).green = 0;
			(*(imagedata + i * width + j)).blue = 0;
		}
	}
	//fseek(fpi, 54, SEEK_SET);+b+yy
	fread(imagedata, sizeof(struct tagIMAGEDATA) * width, height, fpi);

	//�������16λ�ļ���λ
	char crypto[65535] = { 0 };
	int string_len = 0;

	BYTE shift_bits = 0x00;
	int j = 0;
	string_len = strlen(crypto);

	//����
	for (int i = 0; ; ++i) {
		string_len = bitManipulation_reverse(string_len, imagedata[i].blue, shift_bits++);
		if (shift_bits == CHAR_NUM) {
			break;
		}
		string_len = bitManipulation_reverse(string_len, imagedata[i].green, shift_bits++);
		string_len = bitManipulation_reverse(string_len, imagedata[i].red, shift_bits++);
	}
	shift_bits = 0x00;

	//1���ַ�Ҫ8��BYTE, Ҳ����8/3 = 2.667����ɫ
	for (int i = 6;; ++i) {
		
		crypto[j] = bitManipulation_reverse(crypto[j], imagedata[i].green, shift_bits);
		if (++shift_bits == 8) {
			shift_bits = 0;
			if (++j >= string_len)
				break;
		}
		crypto[j] = bitManipulation_reverse(crypto[j], imagedata[i].red, shift_bits);
		if (++shift_bits == 8) {
			shift_bits = 0;
			if (++j >= string_len)
				break;
		}
		crypto[j] = bitManipulation_reverse(crypto[++j], imagedata[i].blue, shift_bits);
		if (++shift_bits == 8) {
			shift_bits = 0;
			if (++j >= string_len)
				break;
		}
	}
	cout << "���ܺ������Ϊ:" << crypto << endl;
	fclose(fpi);
	return 0;
}
int read_encript_pic_plus(char origin_Filename[], IMAGEDATA *imagedata, int ndim) {
	FILE *fpi = fopen(origin_Filename, "rb");
	int width, height;
	if (fpi != NULL) {
		readImageData(fpi);
		showBmpInfoHead(strInfo);
		showBmpHead(strHead);
	}
	else {
		cout << "file open error!" << endl;
		return -1;
	}
	initPla(fpi);
	width = strInfo.biWidth;
	height = strInfo.biHeight;

	imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));
	BYTE* B_imagedata = (BYTE*)imagedata;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			(*(imagedata + i * width + j)).blue = 0;
			(*(imagedata + i * width + j)).green = 0;
			(*(imagedata + i * width + j)).blue = 0;
		}
	}
	//fseek(fpi, 54, SEEK_SET);+b+yy
	fread(imagedata, sizeof(struct tagIMAGEDATA) * width, height, fpi);

	//�������16λ�ļ���λ
	char crypto[65535] = { 0 };
	int string_len = 0;

	BYTE shift_bits = 0x00;
	int j = 0;
	string_len = strlen(crypto);

	//����
	for (int i = 0;i < CHAR_NUM ; ++i) {
		string_len = bitManipulation_reverse(string_len, B_imagedata[i * ndim], shift_bits++);
	}
	cout << "��ȡ���ĳ���Ϊ:" << string_len << endl;
	shift_bits = 0x00;

	for (int i = CHAR_NUM;; ++i) {
		crypto[j] = bitManipulation_reverse(crypto[j], B_imagedata[i * ndim], shift_bits);
		if (++shift_bits == 8) {
			shift_bits = 0;
			j++;
			if (j >= string_len)
				break;
		}
	}
	cout << "���ܺ������Ϊ:" << crypto << endl;
	fclose(fpi);
	return 0;
}


int main() {
	char strFile[LENGTH_NAME_BMP];
	IMAGEDATA *imagedata = nullptr;
	IMAGEDATA *imagedataRot = nullptr;

/*
	cout << "��������Ҫ��ȡ�ļ�����" << endl;
	cin >> strFile;*/
	//rotateBMPimage(strFile, imagedata, imagedataRot);
	//lowestBitReplace("bkg.bmp", "bkg_cpto.bmp", imagedata);
	lowestBitReplace_plus("bkg.bmp", "bkg_cpto_plus.bmp", imagedata, 1);
	read_encript_pic_plus("bkg_cpto_plus.bmp", imagedata, 1);

	getchar();
	getchar();
}