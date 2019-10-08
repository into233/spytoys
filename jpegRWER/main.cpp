#include <jpeglib.h>
#include <iostream>
#include <exception>
int main(int argc, char **argv)
{
	std::FILE* inputfile = fopen("jpg.jpg", "rb");
	std::FILE* outputfile = fopen("jpg1.jpg", "rb");


	if (!inputfile)
		throw std::exception("Failed to open file");
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outputfile);

}