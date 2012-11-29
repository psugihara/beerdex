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

int read_images(char *root, vector<Mat> &images, vector<float> &labels)
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

			Mat im = imread( (*it2).string(), CV_LOAD_IMAGE_COLOR );

			images.push_back(im);
			labels.push_back(num);

			count++;


		}

		num++;

	}

	cout << "count: " << count << endl;

	return num;

}

int main (int argc, char *argv[])
{
	if (argc != 2) {
		cerr << "usage: trainer <image-root>" << endl;
		return -1;
	}

	vector<Mat> images;
	vector<float> labels;

	int nr_class = read_images(argv[1],images,labels);

	cout << "nr_class: " << nr_class << endl;

	float lbs[labels.size()];

	vector<float>::const_iterator it;
	int num = 0;
	for (it = labels.begin(); it != labels.end(); ++it) {
		lbs[num] = *it;

		cout << "label " << *it << endl;

		num++;
	}
	
    Mat cv_labels(labels.size(), 1, CV_32FC1, lbs);

	BeerClassifier classifier;

	classifier.train(images,cv_labels);

	classifier.save("bottles.model");

	return 0;
}








