//
//  beer.cpp
//  
//
//  Created by Benjamin Klingher on 11/24/12.
//
//

#include <stdio.h>
#include <iostream>

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/ml/ml.hpp>


#include "Beers.h"
#include "BeerClassifier.h"

#define FRAMES 2


using namespace std;
using namespace cv;

Mat extractFeatures(Mat im);

int main (int argc, char *argv[])
{
	if (argc != 3) {
		cerr << "usage: beer im1 im2" << endl;
		return -1;
	}

	BeerClassifier classifier;

	Mat im_1 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	Mat im_2 = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE );
//	Mat im_3 = imread( argv[3], CV_LOAD_IMAGE_GRAYSCALE );
//	Mat im_4 = imread( argv[4], CV_LOAD_IMAGE_GRAYSCALE );


	vector<Mat> images;

	images.push_back(im_1);
	images.push_back(im_2);
//	images.push_back(im_3);
//	images.push_back(im_4);


	/*
	
	CvMat descriptors_1 = classifier.extract_feats(im_1);
	CvMat descriptors_2 = classifier.extract_feats(im_2);
	CvMat descriptors_3 = classifier.extract_feats(im_3);
	CvMat descriptors_4 = classifier.extract_feats(im_4);

	Mat dest(4, FRAMES * 128);

	CvMat = dest;

	descriptors_1.copyTo(dest.row(1));
	descriptors_2.copyTo(dest.row(2));
	descriptors_3.copyTo(dest.row(3));
	descriptors_4.copyTo(dest.row(4));

	CvMat feats = dest;

	Mat labels(4, 1, CV_32FC1);

	labels.at(1,1) = 1;
	labels.at(2,1) = 2;
	labels.at(3,1) = 3;
	labels.at(4,1) = 4;

	CvMat cv_labels = labels;
	 
	*/

	Mat labels(2, 1, CV_16UC1);
	
	labels.at<int>(1,1) = CORONA;
	labels.at<int>(2,1) = BUD;

	classifier.train(images, labels);
	classifier.save("bottles.model");
	 

	return 0;
}















