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
#include <map>


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

void set_map(map<int, string> &m)
{

	m[0] = "bud_light";
	m[1] = "budweiser";
	m[2] = "coors_light";
	m[3] = "corona";
	m[4] = "heineken";
	m[5] = "magic_hat";
	m[6] = "rolling_rock";
	m[7] = "sierra_nevada";
	m[8] = "stella_artois";
	m[9] = "yuengling";


}

int main (int argc, char *argv[])
{
	if (argc != 2) {
		cerr << "usage: beer im " << endl;
		return -1;
	}

	map<int, string> m;

	set_map(m);

	BeerClassifier classifier;

    
	classifier.load("../../trainer/build/bottles.model");

	Mat im = imread( argv[1], CV_LOAD_IMAGE_COLOR );

	cout << "result: " << m[classifier.label(im)] << endl;


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

	//Mat labels(2, 1, CV_32FC1);
	
	//labels.at<float>(1,1) = CORONA;
	//labels.at<float>(2,1) = BUD;

	
	return 0;
}















