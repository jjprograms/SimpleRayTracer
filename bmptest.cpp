#include "bmp.cpp"

int main()
{
	char * img = new char [199*200*3];
	int i;
	for (i = 0; i < 199*200*3; i++)
	{
		img[i] = 255;
	}
	
	saveimage("test.bmp", img, 199, 200);
	return 0;
}