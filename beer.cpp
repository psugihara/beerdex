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

#include "BeerClassifier.h"


#FRAMES 2

#include "beer.h"

using namespace std;
using namespace cv;

Mat extractFeatures(Mat im);

int main (int argc, char *argv[])
{
	if (argc != 5) {
		cerr << "usage: beer im1 im2 im3 im4" << endl;
		return -1;
	}

	BeerClassifier classifier;

	Mat im_1 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	Mat im_2 = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE );
	Mat im_3 = imread( argv[3], CV_LOAD_IMAGE_GRAYSCALE );
	Mat im_4 = imread( argv[4], CV_LOAD_IMAGE_GRAYSCALE );
	
	Mat descriptors_1 = classifier.extractFeatures(im_1);
	Mat descriptors_2 = classifier.extractFeatures(im_2);
	Mat descriptors_3 = classifier.extractFeatures(im_3);
	Mat descriptors_4 = classifier.extractFeatures(im_4);

	Mat dest(4, FRAMES * 128);

	descriptors_1.copyTo(dest.row(1));
	descriptors_2.copyTo(dest.row(2));
	descriptors_3.copyTo(dest.row(3));
	descriptors_4.copyTo(dest.row(4));

	CvMat feats = dest;

	Mat labels(4, 1, CV_32FC1);

	labels.at(1,1) = "heineken";
	labels.at(2,1) = "stella";
	labels.at(3,1) = "bud";
	labels.at(4,1) = "corona";

	classifier.train(&feats, &labels, 0)
	classifier.("bottles.model");

	return 0;
}















