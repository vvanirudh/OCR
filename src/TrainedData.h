#ifndef TRAINEDDATA_H
#define TRAINEDDATA_H

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cvaux.h"
#include "opencv/cxcore.h"
#include "opencv/ml.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
//#include "Image.h"
#include <vector>

using namespace std;

class TrainedData{
private:
	CvMat *samples;
	CvMat *labels;
	int samplesTrained;
	CvKNearest classifier;
	CvMat *identifysample;
	CvMat *identifylabel;
public:
	TrainedData();
	~TrainedData();
	pair<char, int> identify(vector<int> &sample);
	
	void do_training();
};

#endif
