//
//  BeerClassifier.h
//  BeerCaddy
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#ifndef __BeerCaddy__BeerClassifier__
#define __BeerCaddy__BeerClassifier__

#include <iostream>
#include <vector>

#include <opencv2/nonfree/features2d.hpp>

using namespace std;
using namespace cv;

class BeerClassifier {
    CvSVM svm_;

public:
    void load(const char *path);
    void save(const char *path);
    void train(vector<Mat> &train_imgs, Mat &labels);
    int label(cv::Mat &sample);
};

#endif /* defined(__BeerCaddy__BeerClassifier__) */
