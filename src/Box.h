#ifndef BOX_H
#define BOX_H

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cvaux.h"
#include "opencv/cxcore.h"
#include "opencv/ml.h"
#include <iostream>

using namespace std;

class Box{
private:
	CvRect rect;
public:
	void setRect(CvRect r);
	CvRect getRect() const;
};
	
#endif
