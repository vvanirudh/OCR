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




using namespace std;

int main (int argc, char const* argv[])
{
	// load the image file from which the characters are to recognised.
	IplImage* img = cvLoadImage(argv[1]);
	img = cvLoadImage(argv[1]);
	if(!img){
		printf("Could not open image file: %s", argv[1]);
		exit(0);
	}
	
	Image image(img);
	
	TrainedData data;
	
	//The training is done on image and the data is stored in "data".
	data.do_training();
	
	//testimage has to use this data and identify the numbers present in the image.
	image.identify(data);
	return 0;
}
