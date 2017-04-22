#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//bitmap structure
struct bitfilehead
{
	unsigned short hf;
	unsigned int size;
	unsigned short rev1;
	unsigned short rev2;
	unsigned int pixoffset;
};

struct dibhead		//BITMAPINFOHEADER
{
	unsigned int hsize;			//always 40
	int width;					//signed?
	int height;					//signed?
	unsigned short colplanes;	//always 1
	unsigned short bitspp;
	unsigned int compression;	// 0 for none
	unsigned int imagesize;		// can be 0 if compression is none
	int horizontalres;			//signed?
	int verticalres;			//signed?
	unsigned int colpalnum;		// 0
	unsigned int numimport;		// 0
};

struct pixel
{
	char red;
	char green;
	char blue;
};

struct image
{
	unsigned int width;
	unsigned int height;
	char * pixeldata; //pixel * data;
	bitfilehead bfh;
	dibhead dib;
};


//load bitmap
image loadimage(char filepath[])
{
	streampos size;
	char * memblock;
	fstream file;
	image img;

	file.open(filepath,ios::binary|ios::in|ios::ate);

	if (file.is_open())
	{
		size = file.tellg();
    	memblock = new char [size];
    	file.seekg (0, ios::beg);
    	file.read (memblock, size);
    	file.close();
		
		cout << "file read successfully\n";
	}

	int pixdataoffset = sizeof(img.bfh) + sizeof(img.dib);
	char * pixdata = new char [((int) size) - pixdataoffset];
	
	for (int i = 0; i < size; i++)
	{
		pixdata[i] = memblock[i + pixdataoffset];
	}
	
	img.bfh = *(bitfilehead*) memblock;
	img.dib = *(dibhead*) (memblock+sizeof(img.bfh));
	img.pixeldata = pixdata;//(pixel*) pixdata;

	if (img.dib.width < 0)
		img.width = -img.dib.width;
	else
		img.width = img.dib.width;

	if (img.dib.height < 0)
		img.height = -img.dib.height;
	else
		img.height = img.dib.height;

	delete[] memblock;
	
	return img;
}



//save bitmap
void saveimage(const string& filepath, image imagedata)
{
	
}

void saveimage(const char * filepath, char * binarydata, int width, int height)
{
	fstream file;
	file.open(filepath,ios::binary|ios::out|ios::ate|ios::trunc);
	
	//create and fill dibhead
	dibhead dbh;
	dbh.hsize = 40;
	dbh.width = width;
	dbh.height = height;
	dbh.colplanes = 1;
	dbh.bitspp = 24;
	dbh.compression = 0;
	dbh.imagesize = 0;
	dbh.horizontalres = 3800;
	dbh.verticalres = 3800;
	dbh.colpalnum = 0;
	dbh.numimport = 0;
	
	//pad rows
	//padbytes = (4-(self.width*(self.colordepth//8))%4)%4
	int padbytes = (4-(width*3)%4)%4;
    //pixelbytes = bytearray()
    //for row in self.imgdata:
			//rowbytes = bytearray([colorbyte for pixel in row for colorbyte in pixel]) + bytearray([0 for b in range(padbytes)])
            //pixelbytes += rowbytes
						
	//create and fill bitfilehead
	bitfilehead bithead;
	bithead.hf = 0x4d42; //19778;
	bithead.size = 14 + sizeof(dibhead) + (width*3+padbytes)*height;
	bithead.rev1 = 0;
	bithead.rev2 = 0;
	bithead.pixoffset = 14 + sizeof(dibhead);

	//write data to file
	file.write((char*)&bithead.hf, 2);
	file.write((char*)&bithead.size, 4);
	file.write((char*)&bithead.rev1, 2);
	file.write((char*)&bithead.rev2, 2);
	file.write((char*)&bithead.pixoffset, 4);
	file.write((char*)&dbh, sizeof(dibhead));
	
	char padding[] = {0, 0, 0};
	
	char * dataptr = binarydata;
	
	int i;
	for (i = 0; i < height; i++)
	{
		file.write(dataptr + (width * 3 * i), width * 3);
		file.write(&padding[0], padbytes);
	}
	
	file.close();
}


