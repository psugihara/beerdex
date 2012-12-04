//
//  BeerClassifier.cpp
//  BeerCaddy
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#include "BeerClassifier.h"

#define FRAMES 8

using namespace std;
using namespace cv;

static Mat extract_feats(Mat& im)
{
	// Change image to desired format (540x360 CV_8UC3).
    Mat resize_dest(540, 360, im.type());
	resize(im, resize_dest, resize_dest.size(), 0, 0, CV_INTER_AREA);

    Mat convert_dest(540, 360, CV_8UC3);
    cvtColor(resize_dest, convert_dest, CV_RGBA2RGB);

//    Mat diff(convert_dest);
//    bitwise_xor(resize_dest, convert_dest, diff);
//    cout << sum(diff) << endl;

	SiftDescriptorExtractor extractor;

    vector<KeyPoint> keypoints;
    keypoints.push_back(KeyPoint(180, 330, 32));
    keypoints.push_back(KeyPoint(180, 330, 16));
    keypoints.push_back(KeyPoint(180, 300, 32));
    keypoints.push_back(KeyPoint(180, 170, 32));
    keypoints.push_back(KeyPoint(150, 300, 16));
    keypoints.push_back(KeyPoint(150, 330, 16));
    keypoints.push_back(KeyPoint(210, 300, 16));
    keypoints.push_back(KeyPoint(210, 330, 16));

    Mat descriptors;

    extractor.compute(convert_dest, keypoints, descriptors);

    return descriptors.reshape(1, 1);
}

void BeerClassifier::train(cv::Mat &descriptors, cv::Mat &labels)
{
    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::POLY;
    params.degree = 2;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 6000, 1e-6);

    svm_.train_auto(descriptors, labels, Mat(), Mat(), params, 24);
}

void BeerClassifier::train(vector<Mat> &train_imgs, Mat &labels)
{

	Mat descriptors(train_imgs.size(), FRAMES * 128, CV_32FC1);

	int i = 0;
    vector<Mat>::iterator it;
	for (it = train_imgs.begin(); it < train_imgs.end(); it++) {
		Mat descriptor = extract_feats(*it);
		descriptor.copyTo(descriptors.row(i));
		i++;
	}

    train(descriptors, labels);
}

int BeerClassifier::label(Mat &sample_image)
{
    CvMat feats = extract_feats(sample_image);
    return svm_.predict(&feats);
}

float BeerClassifier::cross_validate(vector<Mat> &train_imgs, Mat &labels)
{
    // Get all descriptors so we just calculate these once.
    Mat descriptors(train_imgs.size(), FRAMES * 128, CV_32FC1);
	int i = 0;
    vector<Mat>::iterator it;
	for (it = train_imgs.begin(); it < train_imgs.end(); it++) {
		Mat descriptor = extract_feats(*it);
		descriptor.copyTo(descriptors.row(i));
		i++;
	}

    Mat train_feats(descriptors.size().height - 1,
                    descriptors.size().width, descriptors.type());
    Mat train_labels(labels.size().height - 1, 1, labels.type());

    float correct = 0;

    // Cross validate by withholding a sample, 1 at a time then testing on it.
    for (int i = 0; i < train_imgs.size(); i++) {

        // Create a new training Mats without the ith row.
        int skipped = 0;
        for (int j = 0; j < labels.size().height; j++) {
            if (i == j) {
                skipped = 1;
				continue;
            }

            labels.row(j).copyTo(train_labels.row(j - skipped));
            descriptors.row(j).copyTo(train_feats.row(j - skipped));
        }

        train(train_feats, train_labels);

        if (svm_.predict(descriptors.row(i)) == labels.at<int>(i, 0))
            correct++;

        cout << correct << "/" << i + 1 << endl;
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