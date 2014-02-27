#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cvaux.h"
#include "opencv/cxcore.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "Image.h"
#include "TrainedData.h"
#include "Box.h"
#include <fstream>

using namespace std;

int main (int argc, char const* argv[])
{
	IplImage *img = 0;
	char imagename[100];
	
	// opening a file containg multiple file names.
	ifstream file(argv[1]);
	ofstream file1("vectors.txt");
	ofstream file2("labels.txt");
	int num;
	file>>num;
	for(int i=0;i<num;i++)
	{
	
		// reading the image file name.
		file>>imagename;
		
		// loading the image
		img = cvLoadImage(imagename);
		if(!img){
			printf("Could not open image file: %s", imagename);
			exit(0);
		}
	
		Image image(img);
		
		// training on this image
		image.train();
	}// training on all the image files present specified in the file.
	file.close();
	file1.close();
	file2.close();
	return 0;
}
