//
//  beer.cpp
//  
//
//  Created by Benjamin Klingher on 11/24/12.
//
//

#include <stdio.h>
#include <iostream>

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/ml/ml.hpp>

#include <boost/filesystem.hpp>

#include "Beers.h"
#include "BeerClassifier.h"

#define FRAMES 2


using namespace std;
using namespace cv;
using namespace boost::filesystem;


bool isHidden(const path &p)
{
	string name = boost::filesystem::basename(p);
	
    if(name != ".." &&
       name != "."  &&
       (name[0] == '.' || name == "")) {
		return true;
    }

    return false;
}

int read_images(char *root, vector<Mat> &images, vector<int> &labels)
{
	path p(root);

	if ( !exists( p ) ) return -1;
	if ( !is_directory( p ) ) return -1;

	vector<path> vec;

	copy(directory_iterator(p), directory_iterator(), back_inserter(vec));

	vector<path>::const_iterator it;

	int num = 0;
	int count = 0;

	for (it = vec.begin(); it != vec.end(); ++it) {
		if (isHidden(*it) || !is_directory(*it))
			continue;

		cout << (*it).string() << endl;

		vector<path> vec2;

		copy(directory_iterator(*it), directory_iterator(), back_inserter(vec2));

		vector<path>::const_iterator it2;

		for (it2 = vec2.begin(); it2 != vec2.end(); ++it2) {
			if (isHidden(*it2))
				continue;

			Mat im = imread( (*it2).string(), CV_LOAD_IMAGE_COLOR);

			images.push_back(im);
			labels.push_back(num);

			count++;
		}

		num++;

	}

	return num;
}

int main (int argc, char *argv[])
{
	if (argc != 2 && argc != 4) {
		cerr << "usage: trainer <image-root> [model-output vocab-output]" << endl;
		return -1;
	}

    char *image_root = argv[1];
    char *model_output = NULL;
    char *vocab_output = NULL;

    if (argc == 4) {
        model_output = argv[2];
        vocab_output = argv[3];
    }

	vector<Mat> images;
	vector<int> labels;

	int nr_class = read_images(argv[1], images, labels);

    cout << "count: " << labels.size() << endl;
	cout << "nr_class: " << nr_class << endl;
	
    Mat cv_labels(labels.size(), 1, CV_32SC1, &labels[0]);

	BeerClassifier classifier;

    if (model_output) {
        cout << "Creating model and vocab..." << endl;
        classifier.train_bow(images, cv_labels);
        classifier.save_with_bow(model_output, vocab_output);
    } else {
        cout << "Performing cross validation..." << endl;
        cout << classifier.cross_validate_bow(images, cv_labels) << endl;
	}

	return 0;
}








