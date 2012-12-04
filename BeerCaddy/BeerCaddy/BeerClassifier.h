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

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/nonfree/features2d.hpp>

class BeerClassifier {
    CvSVM svm_;
    
public:
    void load(const char *path);
    void save(const char *path);
    void train(cv::Mat &descriptors, cv::Mat &labels);
    void train(std::vector<cv::Mat> &train_imgs, cv::Mat &labels);
    int label(cv::Mat &sample);
    float cross_validate(std::vector<cv::Mat> &train_imgs, cv::Mat &labels);
};

#endif /* defined(__BeerCaddy__BeerClassifier__) */
