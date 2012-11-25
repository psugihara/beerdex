//
//  beer.cpp
//  
//
//  Created by Benjamin Klingher on 11/24/12.
//
//

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <iostream>
#include <opencv2/nonfree/features2d.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"


#FRAMES 10

#include "beer.h"

using namespace std;
using namespace cv;

Mat extractFeatures(Mat im);

int main (int argc, char *argv[])
{
	if (argc != 4) {
		cerr << "usage: beer imagepath" << endl;
		return -1;
	}

	Mat im_1 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	Mat im_2 = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE );

	



	Mat descriptors_1 = extractFeatures(im_1).reshape(128*FRAMES, 1);
	Mat descriptors_2 = extractFeatures(im_2).reshape(128*FRAMES, 1);
	

	CvMat cv_desc_1 = descriptors_1;
	CvMat cv_desc_2 = descriptors_2;



	
	classify(cv_desc)


	Size s = descriptors.size();

	printf("size: width %d height %d\n", s.width, s.height);

	return 0;
}


Mat extractFeatures(Mat& im, vector<KeyPoint>& keypoints)
{
	//int minHessian = 400;

	//SurfFeatureDetector detector( minHessian );

	//vector<KeyPoint> keypoints;

	//detector.detect( im, keypoints );

	Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("SURF");

	Mat descriptors;

	extractor->compute(im, keypoints, descriptors);

	return descriptors;
}

char *classify(CvMat& descriptors, )
{
	float labels[4] = {1.0, -1.0, -1.0, -1.0};
    Mat labelsMat(4, 1, CV_32FC1, labels);

    float trainingData[4][2] = { {501, 10}, {255, 10}, {501, 255}, {10, 501} };
    Mat trainingDataMat(4, 2, CV_32FC1, trainingData);

    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

    // Train the SVM
    CvSVM SVM;
    SVM.train(trainingDataMat, labelsMat, Mat(), Mat(), params);
}












