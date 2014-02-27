#include "Image.h"
#include "Box.h"
#include <queue>
#include <fstream>

// overloading the < operator to determine the characters in the image in a sequential order.
bool operator<(const Box& e1, const Box& e2)  {
	int x1 = e1.getRect().x;
  	int y1 = e1.getRect().y;
  	int x2 = e2.getRect().x;
  	int y2 = e2.getRect().y;
  	int h1 = e1.getRect().height;
  	int w1 = e1.getRect().width;
  	int h2 = e2.getRect().height;
  	int w2 = e2.getRect().width;
  	
  	int x11 = x1+w1;
  	int y11 = y1+h1;
  	int x22 = x2+w2;
  	int y22 = y2+h2;
  	
  	if(y1>y22) return true;
  	else if(y2>y11) return false;
  	else if(x1>x2) return true;
  	else return false;
 }
  	
// Constructer for the image class where memory for the contours and the data extracted from the image are stored.
Image::Image(IplImage *img){
	image = img;
	height = image->height;
	width = image->width;
	step = image->widthStep;
	channels = image->nChannels;
	data = (uchar *)image->imageData;
	tempImage = cvCreateImage(cvSize(width, height),IPL_DEPTH_8U, 1);
	cvConvertImage(img, tempImage);
	trainImage = cvCloneImage(tempImage);
	storage = NULL;
	storage=cvCreateMemStorage();
	contours = NULL;
	numOfBoxes=0;
}

//Destructor used to release all the memory used
Image::~Image(){
	cvReleaseImage(&tempImage);
	cvReleaseImage(&image);
	cvReleaseImage(&trainImage);
}

//This function is to just draw boxes around the characters present in the image.
void Image::box(){
	cvThreshold(tempImage, tempImage, 200, 255, CV_THRESH_BINARY);
	cvFindContours(tempImage, storage, &contours);
	cvZero(tempImage);
	CvRect rect;
	for(;contours!=NULL;contours=contours->h_next){
		if((CV_IS_SEQ_HOLE(contours))){
			double area=fabs(cvContourArea(contours,CV_WHOLE_SEQ));
			if(area>20){
				rect=cvBoundingRect(contours,0);				
				cvRectangle(image,cvPoint(rect.x,rect.y),cvPoint(rect.x+rect.width,rect.y+rect.height),cvScalar(0,0,255,0),1,8,0);
			}
		}
	}
}

//This function is for displaying the image.
void Image::display(){
	cvNamedWindow("image", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("image", 100,100);
	cvShowImage("image", image);
	cvWaitKey(0);
}

//This function is for getting trained on the image
void Image::train(){

	// making the image binary i.e., the pixel values of the image will be either 0 or 255.
	cvThreshold(tempImage,tempImage,200,255,CV_THRESH_BINARY);
	CvRect rect;
	
	// this will find the contours and store them in the memory created 
	int n=cvFindContours(tempImage,storage,&contours,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);

	// opening the files to store the feature vectors and the labels for the coresponding vectors.
	ofstream vectors("vectors.txt",fstream::app);
	ofstream labels("labels.txt",fstream::app);
	//ofstream input("input4.txt");
	
	// this will extract the contours stored in the memory and create a rectangle around the contour.
	// and create a box object and push the box object in a priority queue so that we can detect the characters
	// in the order we follow to read.
	for(;contours!=NULL;contours=contours->h_next){
		if((CV_IS_SEQ_HOLE(contours))){
			double area=fabs(cvContourArea(contours,CV_WHOLE_SEQ));
			if(area>20){
				rect=cvBoundingRect(contours,0);
				Box b;
				b.setRect(rect);
				boxes.push(b);
				numOfBoxes++;
			}
		}
	}
	
	// this part does the training
	while(!boxes.empty())
	{
		// take the first box and get the rectangle present in it.
		Box m = boxes.top();
		boxes.pop();
		CvRect present_rect = m.getRect();
		
		// isolate the the part of image covered by the rectangle and make it our regeion of interest(ROI).
		cvSetImageROI(trainImage,present_rect);
		
		// resize the ROI to 20x20.
		IplImage* destination=cvCreateImage(cvSize(20,20),trainImage->depth,1);
		IplImage* displayImage = cvCreateImage(cvSize(100,100),trainImage->depth,1);
		cvResize(trainImage,destination);
		cvResize(trainImage, displayImage);
		
		// making the ROI binary i.e., the pixel values of the image will be either 0 or 255.
		cvThreshold(destination,destination,200,255,CV_THRESH_BINARY);
		cvThreshold(displayImage,displayImage,200,255,CV_THRESH_BINARY);
		
		
		// storing the label for the coressponding label in the labels.txt file.
		char key;
		cin>>key;
		labels<< (char)key <<" ";
		
		// extract the feature vector from the ROI and store it in vectors.txt file.
		int widthstep=destination->widthStep;
		unsigned char *data= reinterpret_cast<unsigned char *> (destination->imageData);
		for (int i=0; i<20; i++) {
			for (int j=0; j<20; j+= destination->nChannels) {
			
				vectors << (int) data[j] <<" ";
			}
			data+= widthstep;
		}
		vectors<<"\n";
		
		// release the ROI so that another rectangle can be processesd
		cvReleaseImage(&destination);
		cvReleaseImage(&displayImage);
		cvResetImageROI(trainImage);
	}
}

//This function is to identify the characters present in the image based on the trained data.
void Image::identify(TrainedData & traineddata){
	// making the entire image binary i.e., the pixel values of the image either contains o or 255.
	cvThreshold(tempImage,tempImage,200,255,CV_THRESH_BINARY);
	CvRect rect;
	
	// find the characters in the image and store their contours in the memory assigned.
	int n=cvFindContours(tempImage,storage,&contours,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);
	vector<int> sample;
	
	// this will extract the contours stored in the memory and create a rectangle around the contour.
	// and create a box object and push the box object in a priority queue so that we can detect the characters
	// in the order we follow to read.
	for(;contours!=NULL;contours=contours->h_next){
		if((CV_IS_SEQ_HOLE(contours))){
			double area=fabs(cvContourArea(contours,CV_WHOLE_SEQ));
			if(area>20){
				rect=cvBoundingRect(contours,0);
				Box b;
				b.setRect(rect);
				boxes.push(b);
			}
		}
	}
	
	// this part will do the identifing.
	while(!boxes.empty())
	{
		
		// extract the rectangle from the first box.
		Box m = boxes.top();
		boxes.pop();
		CvRect present_rect = m.getRect();
		
		// isolate the the part of image covered by the rectangle and make it our regeion of interest(ROI).
		cvSetImageROI(trainImage,present_rect);
		IplImage* destination=cvCreateImage(cvSize(20,20),trainImage->depth,1);	
		cvResize(trainImage,destination);
		
		// making the ROI binary i.e., the pixel values of the image will be either 0 or 255.
		cvThreshold(destination,destination,200,255,CV_THRESH_BINARY);
		int widthstep=destination->widthStep;
		
		// extracting the feature vector from the ROI and sending it to the TrainedData for identifying it. 
		unsigned char *data= reinterpret_cast<unsigned char *> (destination->imageData);
		for (int i=0; i<20; i++) {
			for (int j=0; j<20; j+= destination->nChannels) {
				
				sample.push_back(data[j]);
				
			}
			data+= widthstep;
		}
		
		pair<char, int> a = traineddata.identify(sample);
		sample.clear();
		
		// displaying the character recognised.
		cout<<a.first<<" ";
		// releasing the ROI so that another rectangle can be processed.
		cvReleaseImage(&destination);
		cvResetImageROI(trainImage);
	}
	
	cout<<endl;
	

}

void Image::reidentify(TrainedData & traineddata, IplImage* image1){
	int per =60;
	
	// create a tempImage1 for finding contours.
	IplImage * tempImage1;
	int width1 = image1->width;
	int height1 = image1->height;
	int step1 = image1->widthStep;
	int channels1 = image1->nChannels;
	uchar *data1 = (uchar *)image1->imageData;
	tempImage1 = cvCreateImage(cvSize(width1, height1),IPL_DEPTH_8U, 1);
	cvConvertImage(image1, tempImage1);
	IplImage* trainImage1 = cvCloneImage(tempImage1);
	
	// threshold the image with the edge of 140 i.e., if the pixel value is above 140 make it 255 else 0.
	cvThreshold(tempImage1,tempImage1,140,255,CV_THRESH_BINARY);
	cvThreshold(trainImage1,trainImage1,140,255,CV_THRESH_BINARY);
	CvRect rect, temprect;
	
	
	
	// find the contours present in the tempImage1.
	CvSeq* contours1 = NULL;
	CvMemStorage *storage1 = NULL;
	storage1 = cvCreateMemStorage();
	int n=cvFindContours(tempImage1,storage1,&contours1,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);
	vector<int> sample;
	priority_queue<Box> tempboxes;
	
	// create the Box object and push them into priority queue.
	for(;contours1!=NULL;contours1=contours1->h_next){
		if((CV_IS_SEQ_HOLE(contours1))){
			double area=fabs(cvContourArea(contours1,CV_WHOLE_SEQ));
			//cout<<"area :"<<area<<endl;
			if(area>4){
				rect=cvBoundingRect(contours1,0);
				Box b;
				b.setRect(rect);
				tempboxes.push(b);
			}
		}
	}
	
	while(!tempboxes.empty())
	{
		Box m = tempboxes.top();
		tempboxes.pop();
		CvRect present_rect = m.getRect();
		cvSetImageROI(trainImage1,present_rect);
		IplImage* destination=cvCreateImage(cvSize(20,20),image1->depth,1);	
		cvResize(trainImage1,destination);
		cvThreshold(destination,destination,140,255,CV_THRESH_BINARY);
		pair<char, int> a = reidentifyhelper(traineddata, destination);
		cvResetImageROI(trainImage1);
		
	
		// combine the first 2 parts and check its accuracy
		if(tempboxes.size() >= 1){
			Box fbox = tempboxes.top();
			tempboxes.pop();
			temprect = fbox.getRect();
			present_rect.width = present_rect.width + temprect.width;
			present_rect.width = max(present_rect.height, temprect.height);
			cvSetImageROI(trainImage1,present_rect);
				
			cvResize(trainImage1,destination);
			cvThreshold(destination,destination,140,255,CV_THRESH_BINARY);
			pair<char, int> b = reidentifyhelper(traineddata, destination);
			cvResetImageROI(trainImage1);
			if(b.second > per && tempboxes.size() >= 1){
				Box sbox = tempboxes.top();
				tempboxes.pop();
				temprect = sbox.getRect();
				present_rect.width = present_rect.width + temprect.width;
				present_rect.width = max(present_rect.height, temprect.height);
				cvSetImageROI(trainImage1,present_rect);
					
				cvResize(trainImage1,destination);
				cvThreshold(destination,destination,140,255,CV_THRESH_BINARY);
				pair<char, int> c = reidentifyhelper(traineddata, destination);
				cvResetImageROI(trainImage1);
				if(c.second > per && tempboxes.size() >= 1){
					Box tbox = tempboxes.top();
					tempboxes.pop();
					temprect = tbox.getRect();
					present_rect.width = present_rect.width + temprect.width;
					present_rect.width = max(present_rect.height, temprect.height);
					cvSetImageROI(trainImage1,present_rect);
						
					cvResize(trainImage1,destination);
					cvThreshold(destination,destination,140,255,CV_THRESH_BINARY);
					pair<char, int> d = reidentifyhelper(traineddata, destination);
					cvResetImageROI(trainImage1);
					if(d.second > per){
						cout<<d.second;
						
					}
					else{
						cout << c.second;
						tempboxes.push(tbox);
						
					}
				}
				else{
					cout<<b.second;
					tempboxes.push(sbox);
					
				}
			}
			else if(tempboxes.size() >= 1){
				Box sbox = boxes.top();
				boxes.pop();
				temprect = sbox.getRect();
				present_rect.width = present_rect.width + temprect.width;
				present_rect.width = max(present_rect.height, temprect.height);
				cvSetImageROI(trainImage1,present_rect);
				
				cvResize(trainImage1,destination);
				cvThreshold(destination,destination,140,255,CV_THRESH_BINARY);
				pair<char, int> c = reidentifyhelper(traineddata, destination);
				cvResetImageROI(trainImage1);
				if(c.second > per && tempboxes.size() >= 1){
					Box tbox = boxes.top();
					boxes.pop();
					temprect = tbox.getRect();
					present_rect.width = present_rect.width + temprect.width;
					present_rect.width = max(present_rect.height, temprect.height);
					cvSetImageROI(trainImage1,present_rect);
						
					cvResize(trainImage1,destination);
					cvThreshold(destination,destination,140,255,CV_THRESH_BINARY);
					pair<char, int> d = reidentifyhelper(traineddata, destination);
					cvResetImageROI(trainImage1);
					if(d.second > per){
						cout<<d.second;
						
					}
					else{
						cout << c.second;
						tempboxes.push(tbox);
						
					}
				}
				else{
					cout<<a.second;
					tempboxes.push(sbox);
					tempboxes.push(fbox);
					
				}
			}
		}
		
		cvReleaseImage(&destination);
		
	}
	
	
	

}

pair<char, int> Image::reidentifyhelper(TrainedData & traineddata, IplImage* image1){
	int widthstep=image1->widthStep;
	unsigned char *data= reinterpret_cast<unsigned char *> (image1->imageData);
	vector<int> sample;
	for (int i=0; i<20; i++) {
		for (int j=0; j<20; j+= image1->nChannels) {
			sample.push_back(data[j]);
		}
		data+= widthstep;
	}
	return traineddata.identify(sample);
}
