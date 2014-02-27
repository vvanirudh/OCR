#ifndef IMAGE_H
#define IMAGE_H

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cvaux.h"
#include "opencv/cxcore.h"
#include "opencv/ml.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "TrainedData.h"
#include "Box.h"
#include <vector>
#include <queue>

using namespace std;

class Image {
private:
	IplImage *image;
	IplImage *tempImage;
	IplImage *trainImage;
	int height;
	int width;
	int step;
	int channels;
	uchar *data;
	CvMemStorage *storage;
	CvSeq* contours;
	priority_queue<Box> boxes;
	int numOfBoxes;
public:
	Image(IplImage *img);
	~Image();
	void box();	
	void display();
	void train();
	void identify(TrainedData & traineddata);
	void reidentify(TrainedData &trainedata, IplImage* image);
	pair<char, int> reidentifyhelper(TrainedData &traineddata, IplImage* image);
};

#endif
