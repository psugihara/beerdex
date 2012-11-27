//
//  BeerClassifier.cpp
//  BeerCaddy
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#include "BeerClassifier.h"

#define FRAMES 2

using namespace std;
using namespace cv;

static Mat extract_feats(Mat& im)
{
	//resizing image
	Mat dest(540, 360, im.type());

	resize(im, dest, dest.size(), 0, 0, INTER_CUBIC);

	SiftDescriptorExtractor extractor;

    // TODO: figure out good keypoints to use.
    vector<KeyPoint> keypoints;
    keypoints.push_back(KeyPoint(180, 330, 20));
    keypoints.push_back(KeyPoint(180, 300, 20));

	Mat descriptors;

	extractor.compute(dest, keypoints, descriptors);

    return descriptors.reshape(1, 1);
}

void BeerClassifier::train(vector<Mat> &train_imgs, Mat &labels)
{

	vector<Mat>::iterator it;

	Mat dest(train_imgs.size(), FRAMES * 128, CV_32FC1);

	int i = 0;
	for (it = train_imgs.begin(); it < train_imgs.end(); it++) {

		Mat descriptors = extract_feats(*it);

		descriptors.copyTo(dest.row(i));

		i++;
	}

    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

    svm_.train_auto(dest, labels, Mat(), Mat(), params, 3);
}

int BeerClassifier::label(Mat &sample_image)
{
    int label = -2;

    CvMat feats = extract_feats(sample_image);

    label = svm_.predict(&feats);

    return label;
}

float BeerClassifier::cross_validate(vector<Mat> &train_imgs, Mat &labels)
{
    vector<Mat> imgs;
    Mat train_labels(labels.size().height - 1, 1, CV_32FC1);

    float correct = 0;
    
    for (int i = 0; i < train_imgs.size(); i++) {
        imgs = train_imgs;
        imgs.erase(imgs.begin() + i);

        int skipped = 0;
        for (int j = 0; j < labels.size().height; j++) {
            if (i == j) {
                j++;
                skipped = 1;
            }

            train_labels.at<float>(j - skipped, 1) = labels.at<float>(j, 1);
        }

        train(imgs, train_labels);
        
        if (label(train_imgs[i]) == labels.at<float>(i, 1))
            correct++;
    }

    return correct / train_imgs.size();
}

#pragma mark serialization

void BeerClassifier::load(const char *path)
{
    svm_.load(path);
}

void BeerClassifier::save(const char *path)
{
    svm_.save(path);
}