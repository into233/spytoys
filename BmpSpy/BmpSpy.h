#pragma once
#include <iostream>
#include <memory>
#include <math.h>
#include <stdlib.h>
#include <io.h>

typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef unsigned char BYTE;

typedef struct tagBITMAPFILEHEADER {
	//WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
}BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
}RGBQUAD;

typedef struct tagBITMAPINFO {
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD bmiColors[1];
}BITMAPINFO;

typedef struct tagIMAGEDATA {
	BYTE blue;
	BYTE green;
	BYTE red;
}IMAGEDATA;
