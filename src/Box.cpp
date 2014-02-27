#include "Box.h"

using namespace std;
// the box file only contains the rectangle it is defined especially for the ordering of the rectangles
void Box::setRect(CvRect r)
{
	rect=r;
}

CvRect Box::getRect() const
{
	return rect;
}
