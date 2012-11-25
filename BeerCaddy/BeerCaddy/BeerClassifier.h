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
    CvSVM svm_;

public:
    void load(const char *path);
    void save(const char *path);
    void train(CvMat *samples, CvMat *labels, int count);
    int label(CvMat *sample);
};

#endif /* defined(__BeerCaddy__BeerClassifier__) */
