
#include "Image.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <math.h>
using namespace std;

static void error(const std::string& filename, const std::string& errtext)
{
	cerr << "Error reading image: " << filename << ": " << errtext << '\n';
	exit(1);
}

Image::Image(const std::string& filename)
{
	cerr << "Reading " << filename << "...";
	ifstream in(filename.c_str(), ios::in | ios::binary);
	if (!in)
		error(filename, "Error opening file");
	if (in.get() != 'P' || !in)
		error(filename, "Error reading magic number (1st digit)");
	if (in.get() != '6' || !in)
		error(filename, "Error reading magic number (2nd digit)");
	int maxValue;
	in >> xres >> yres >> maxValue;
	in.get();
	cerr << " " << xres << "x" << yres << "...";
	if (!in)
		error(filename, "Error reading metadaa");
	data = new Pixel*[yres];
	Pixel* p = new Pixel[xres*yres];
	for (int i = 0; i<yres; i++)
		data[i] = p + i*xres;
	for (int i = yres - 1; i >= 0; i--)
		in.read(reinterpret_cast<char*>(data[i]), sizeof(Pixel)*xres);
	if (!in)
		error(filename, "Error reading pixels");
	cerr << " done\n";
}

Image::Image(int xres, int yres)
	: xres(xres), yres(yres)
{
	data = new Pixel*[yres];
	Pixel* p = new Pixel[xres*yres];
	for (int i = 0; i<yres; i++)
		data[i] = p + i*xres;
}

Image::~Image()
{
	delete[] data[0];
	delete[] data;
}
void Image::writeBMP(const std::string& filename) const
{
	FILE*f;
	int k = xres*yres;
	int s = 4 * k;
	int filesize = 54 + s;
	double factor = 39.375;
	int m = static_cast<int>(factor);
	
	int dpi = 72; //you may want to change this 

	int ppm = dpi*m;

	unsigned char bmpfileheader[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
	unsigned char bmpinfoheader[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };
	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(xres);
	bmpinfoheader[5] = (unsigned char)(xres >> 8);
	bmpinfoheader[6] = (unsigned char)(xres >> 16);
	bmpinfoheader[7] = (unsigned char)(xres >> 24);

	bmpinfoheader[8] = (unsigned char)(yres);
	bmpinfoheader[9] = (unsigned char)(yres >> 8);
	bmpinfoheader[10] = (unsigned char)(yres >> 16);
	bmpinfoheader[11] = (unsigned char)(yres >> 24);

	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s >> 8);
	bmpinfoheader[23] = (unsigned char)(s >> 16);
	bmpinfoheader[24] = (unsigned char)(s >> 24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm >> 8);
	bmpinfoheader[27] = (unsigned char)(ppm >> 16);
	bmpinfoheader[28] = (unsigned char)(ppm >> 24);

	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm >> 8);
	bmpinfoheader[31] = (unsigned char)(ppm >> 16);
	bmpinfoheader[32] = (unsigned char)(ppm >> 24);

	char*myFilename = new char[filename.length() + 1];

#ifdef _WIN32
	strcpy_s(myFilename, filename.length() + 1, filename.c_str());
	fopen_s(&f, myFilename, "wb");
#else 
	strcpy(myFilename, filename.c_str());	
	f = fopen(myFilename, "wb");
#endif
		
	delete[]myFilename;
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);

	for (int i = 0; i < k; i++){ 
		double myRed, myGreen, myBlue;		
		
		//i can find the data lol
		myRed = data[i]->r;
		myGreen = data[i]->g;
		myBlue = data[i]->b;
		
		unsigned char color[3] = { (int)floor(myBlue), (int)floor(myGreen), (int)floor(myRed) };
		fwrite(color, 1, 3, f);
		
	}
	fclose(f);

}
void Image::write(const std::string& filename) const
{
	std::ofstream out(filename.c_str(), std::ios::out | std::ios::binary);
	out << "P6 " << xres << " " << yres << " 255\n";
	for (int i = yres - 1; i >= 0; i--)
		out.write(reinterpret_cast<char*>(data[i]), sizeof(Pixel)*xres);
}
