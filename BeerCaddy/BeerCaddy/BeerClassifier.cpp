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
	Mat dest(540, 360, 16);

	resize(im, dest, dest.size(), 0, 0, INTER_CUBIC);

	SurfDescriptorExtractor extractor;

    // TODO: figure out good keypoints to use.
    vector<KeyPoint> keypoints;
    keypoints.push_back(KeyPoint(180, 330, 20));
    keypoints.push_back(KeyPoint(180, 300, 20));

	Mat descriptors;

	extractor.compute(dest, keypoints, descriptors);

    return descriptors.reshape(1, 1);

}


void BeerClassifier::load(const char *path)
{
    svm_.load(path);
}

void BeerClassifier::train(vector<Mat> &train_imgs, Mat &labels)
{

	vector<Mat>::iterator it;

	Mat dest(train_imgs.size(), FRAMES * 128, CV_32FC1);

	cout << "train1" << endl;

	int i = 0;
	for (it = train_imgs.begin(); it < train_imgs.end(); it++) {

		cout << "trainx" << i << endl;

		Mat descriptors = extract_feats(*it);

		cout << "descriptors: " << descriptors.size() << endl;
		cout << "dest: " << dest.size() << endl;


		descriptors.copyTo(dest.row(i));

		cout << "trainy" << i << endl;


		i++;
	}

    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

	cout << "train_end" << endl;

	//cout << "k_fold: " << params.k_fold << endl;

	cout << "feat: " << dest.size() << endl;
	cout << "cvlbs: " << labels.size() << endl;

    svm_.train_auto(dest, labels, Mat(), Mat(), params, 3);
}

int BeerClassifier::label(Mat &sample_image)
{
    int label = -2;

    CvMat feats = extract_feats(sample_image);

    label = svm_.predict(&feats);

    return label;
}

void BeerClassifier::save(const char *path)
{
    svm_.save(path);
}