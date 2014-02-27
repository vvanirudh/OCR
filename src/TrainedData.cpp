#include "TrainedData.h"
#include "Box.h"
#include <fstream>


// the constrauctor of the TrainedData class in which the memory required for the matrices is created.
TrainedData::TrainedData(){
	samples = cvCreateMat(1400, 400, CV_32FC1);
	labels = cvCreateMat(1400, 1, CV_32FC1);
	identifysample = cvCreateMat(1,400, CV_32FC1);
	identifylabel = cvCreateMat(1, 1, CV_32FC1);
	samplesTrained = 0;
}

//Destructer used to release the memory used for the matrices.
TrainedData::~TrainedData(){
	cvReleaseMat(&samples);
	cvReleaseMat(&labels);
	cvReleaseMat(&identifysample);
	cvReleaseMat(&identifylabel);
}

//This function will return the label based on the feature vector.
pair<char, int> TrainedData::identify(vector<int> &sample){
        
        for(int y=0;y<400;y++){
                identifysample->data.fl[y] = sample[y];
                
        }
        int K = 6;
        CvMat* nearests = cvCreateMat( 1, K, CV_32FC1);
        classifier.find_nearest(identifysample,K,identifylabel,0,nearests);
        int accuracy=0;
        for(int i=0;i<K;i++)
        {
                if(nearests->data.fl[i] == identifylabel->data.fl[0]) accuracy++;
        }

        
        char a = identifylabel->data.fl[0];
        
        pair<char, int> b;
        b.first = a;
        b.second = (accuracy*100)/K;
        
        return b;
}


// this will extarct the feature vectors from the vectors.txt and coressponding labels from the labels.txt and creates a classifier
void TrainedData::do_training(){
	ifstream vectorfile("vectors.txt");
	ifstream labelfile("labels.txt");
	int value;
	char label;
	while(! (vectorfile.eof() || labelfile.eof())){
		for(int i=0; i<400; i++){
			vectorfile >> value;
			samples->data.fl[samplesTrained*400 + i] = value;
		}
		labelfile >> label;
		labels -> data.fl[samplesTrained] = label;
		samplesTrained++;
	}
	classifier.train(samples, labels);
	vectorfile.close();
	labelfile.close();
}
