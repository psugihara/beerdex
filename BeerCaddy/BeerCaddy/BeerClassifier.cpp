//
//  BeerClassifier.cpp
//  BeerCaddy
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#include "BeerClassifier.h"

using namespace std;

BeerClassifier::BeerClassifier(const string &path)
{
    svm = new CvSVM();
    svm->load(path.c_str());
}

void BeerClassifier::train(CvMat *samples, CvMat *labels)
{

}

int BeerClassifier::classify(CvMat *sample)
{
    int label = -1;

    return label;
}
