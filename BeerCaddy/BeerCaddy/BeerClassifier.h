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

class BeerClassifier {
    CvSVM *svm;

public:
    BeerClassifier(const std::string &path);
    void train(CvMat *samples, CvMat *labels);
    int classify(CvMat *sample);
};

#endif /* defined(__BeerCaddy__BeerClassifier__) */
