//
//  BeerClassifier.cpp
//  BeerCaddy
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#include "BeerClassifier.h"

using namespace std;
using namespace cv;

#define FRAMES 10


static CvMat extract_feats(Mat& im, vector<KeyPoint>& keypoints)
{
	Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("SURF");

	Mat descriptors;
    
	extractor->compute(im, keypoints, descriptors);

    descriptors = descriptors.reshape(1, FRAMES * 128);
    
	return descriptors;
}


void BeerClassifier::load(const char *path)
{
    svm_.load(path);
}

void BeerClassifier::train(CvMat *feats, CvMat *labels, int count)
{
    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

    svm_.train(feats, labels, Mat(), Mat(), params);
}

int BeerClassifier::label(CvMat *sample_image)
{
    int label = -1;

    vector<KeyPoint> keypoints;
    CvMat feats = extract_feats((Mat &) *sample_image, keypoints);

    label = svm_.predict(&feats);

    printf("hey from the label function");

    return label;
}

void BeerClassifier::save(const char *path)
{
    svm_.save(path);
}