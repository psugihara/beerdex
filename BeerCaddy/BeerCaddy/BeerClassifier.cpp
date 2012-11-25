//
//  BeerClassifier.cpp
//  BeerCaddy
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#include "BeerClassifier.h"
#include <opencv2/nonfree/features2d.hpp>

using namespace std;
using namespace cv;

#define FRAMES 1


static CvMat extract_feats(Mat& im)
{
	SurfDescriptorExtractor extractor;

    // TODO: figure out good keypoints to use.
    vector<KeyPoint> keypoints = {
        {100, 100, 20},
        {100, 200, 20}
    };

	Mat descriptors;

	extractor.compute(im, keypoints, descriptors);

    return descriptors.reshape(1, FRAMES * 128);;
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

int BeerClassifier::label(Mat &sample_image)
{
    int label = -1;

    CvMat feats = extract_feats(sample_image);

    label = svm_.predict(&feats);

    return label;
}

void BeerClassifier::save(const char *path)
{
    svm_.save(path);
}