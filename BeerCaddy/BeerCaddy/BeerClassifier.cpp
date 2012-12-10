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

static vector<KeyPoint> generate_keypoints()
{
	vector<KeyPoint> keypoints;
    keypoints.push_back(KeyPoint(180, 330, 32));
    keypoints.push_back(KeyPoint(180, 330, 16));
    keypoints.push_back(KeyPoint(180, 300, 32));
    keypoints.push_back(KeyPoint(180, 170, 32));
    keypoints.push_back(KeyPoint(150, 300, 16));
    keypoints.push_back(KeyPoint(150, 330, 16));
    keypoints.push_back(KeyPoint(210, 300, 16));
    keypoints.push_back(KeyPoint(210, 330, 16));

	return keypoints;
}

static vector<KeyPoint> generate_keypoints_bow()
{
	vector<KeyPoint> keypoints;

    int grid_size = 8;

    for (int i = 115 + grid_size/2; i < 115 + 130; i += grid_size) {
        for (int j = 60 + grid_size/2; j < 60 + 360; j += grid_size) {
            keypoints.push_back(KeyPoint(i, j, grid_size));
        }
    }

	return keypoints;
}

static Mat extract_bow(Mat& im, vector<KeyPoint>& keypoints, Mat& vocab)
{
	SiftDescriptorExtractor *extractor = new SiftDescriptorExtractor();
	FlannBasedMatcher *matcher = new FlannBasedMatcher();

	BOWImgDescriptorExtractor bow_extractor(extractor, matcher);
	bow_extractor.setVocabulary(vocab);

	Mat bow_descriptor;

	bow_extractor.compute(im, keypoints, bow_descriptor);

    return bow_descriptor.reshape(1, 1);
}

Mat BeerClassifier::extract_feats(Mat& im, vector<KeyPoint> keypoints)
{
    Mat convert_dest = convert(im);

	SiftDescriptorExtractor extractor;
    Mat descriptors;
    extractor.compute(convert_dest, keypoints, descriptors);

    return descriptors.reshape(1, 1);
}

Mat BeerClassifier::convert(Mat &image)
{
    // Change image to desired format (480x360 CV_8UC3).

    Mat resize_dest(480, 360, image.type());
    resize(image, resize_dest, resize_dest.size(), 0, 0, CV_INTER_AREA);

    Mat convert_dest(480, 360, CV_8UC3);
    cvtColor(resize_dest, convert_dest, CV_BGRA2BGR);

    return convert_dest;
}

Mat BeerClassifier::extract_desc_bow(vector<Mat> &train_imgs, Mat &labels)
{
	Mat descriptors;
	int k = 2800;

	int i;
    vector<Mat>::iterator it;

	vector<KeyPoint> keypoints = generate_keypoints_bow();

	//SiftFeatureDetector detector;
	//vector<KeyPoint> keypoints2;

	Mat l2;

    int skipped = 0;

	for (i = 0, it = train_imgs.begin(); it < train_imgs.end(); it++, i++) {

		if ((*it).size().height < 480 || (*it).size().width < 360) {
            skipped++;
			continue;
        }

		l2.push_back(labels.row(i));
        
		Mat convert_dest = convert(*it);

		//detector.detect(convert_dest,keypoints);

		SiftDescriptorExtractor extractor;

		Mat descriptor;

		extractor.compute(convert_dest, keypoints, descriptor);

		descriptors.push_back(descriptor);
	}

	BOWKMeansTrainer bow_trainer(k);

	Mat vocab = bow_trainer.cluster(descriptors);

	vocab_ = vocab;

	SiftDescriptorExtractor *extractor = new SiftDescriptorExtractor();
	FlannBasedMatcher *matcher = new FlannBasedMatcher();

	BOWImgDescriptorExtractor bow_extractor(extractor, matcher);
	bow_extractor.setVocabulary(vocab);

	Mat bow_descriptors(train_imgs.size() - skipped, k, CV_32F);

	for (it = train_imgs.begin(), i = 0; it < train_imgs.end(); it++, i++) {

		if ((*it).size().height < 480 || (*it).size().width < 360)
			continue;

		Mat convert_dest = convert(*it);

		//detector.detect(convert_dest,keypoints2);
		Mat bow_descriptor;
		bow_extractor.compute(convert_dest, keypoints, bow_descriptor);
		bow_descriptor.copyTo(bow_descriptors.row(i));
	}

	labels = l2;

	return bow_descriptors;
}

void BeerClassifier::train_bow(vector<Mat> &train_imgs, Mat &labels)
{
	Mat bow_descriptors = extract_desc_bow(train_imgs, labels);

	train_on_descriptors(bow_descriptors, labels);
}

void BeerClassifier::train_on_descriptors(Mat &descriptors, Mat &labels)
{
    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::RBF;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 6000, 1e-6);

    svm_.train_auto(descriptors, labels, Mat(), Mat(), params, 24);
}

void BeerClassifier::train(vector<Mat> &train_imgs, Mat &labels)
{

	Mat descriptors(train_imgs.size(), FRAMES * 128, CV_32FC1);

	vector<KeyPoint> keypoints = generate_keypoints();
    
    int i;
    vector<Mat>::iterator it;
	for (i = 0, it = train_imgs.begin(); it < train_imgs.end(); it++, i++) {

		Mat descriptor = extract_feats(*it, keypoints);

		descriptor.copyTo(descriptors.row(i));
	}

    train_on_descriptors(descriptors, labels);
}

int BeerClassifier::label(Mat &sample_image)
{
	vector<KeyPoint> keypoints = generate_keypoints();

    CvMat feats = extract_feats(sample_image, keypoints);
    return svm_.predict(&feats);
}

int BeerClassifier::label_bow(Mat &sample_image)
{
	vector<KeyPoint> keypoints = generate_keypoints_bow();

	Mat converted_image = convert(sample_image);

	CvMat feats = extract_bow(converted_image, keypoints, vocab_);

    int res =  svm_.predict(&feats);
	
	return res;
}

float BeerClassifier::cross_validate(vector<Mat> &train_imgs, Mat &labels)
{
    // Get all descriptors so we just calculate these once.
    Mat descriptors(train_imgs.size(), FRAMES * 128, CV_32FC1);

	vector<KeyPoint> keypoints = generate_keypoints();

	int i;
    vector<Mat>::iterator it;
	for (it = train_imgs.begin(), i = 0; it < train_imgs.end(); it++, i++) {
		Mat descriptor = extract_feats(*it, keypoints);
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

        train_on_descriptors(train_feats, train_labels);

        if (svm_.predict(descriptors.row(i)) == labels.at<int>(i, 0))
            correct++;

        cout << correct << "/" << i + 1 << endl;
    }

    return correct / train_imgs.size();
}

float BeerClassifier::cross_validate_bow(vector<Mat> &train_imgs, Mat &labels)
{
	Mat descriptors = extract_desc_bow(train_imgs, labels);

    Mat train_feats(descriptors.size().height - 1,
                    descriptors.size().width, descriptors.type());
    Mat train_labels(labels.size().height - 1, 1, labels.type());

    float correct = 0;

    // Cross validate by withholding a sample, 1 at a time then testing on it.
    for (int i = 0; i < descriptors.size().height; i++) {

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

        train_on_descriptors(train_feats, train_labels);

        if (svm_.predict(descriptors.row(i)) == labels.at<int>(i, 0))
            correct++;

        cout << correct << "/" << i + 1 << endl;
    }

    return correct / descriptors.size().height;
}

float BeerClassifier::test_bow(vector<Mat> &train_imgs, Mat &labels)
{
	Mat descriptors = extract_desc_bow(train_imgs, labels);

    float correct = 0;

    for (int i = 0; i < descriptors.size().height; i++) {

        if (svm_.predict(descriptors.row(i)) == labels.at<int>(i, 0))
            correct++;

        cout << correct << "/" << i + 1 << endl;
    }

    return correct / descriptors.size().height;
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

void BeerClassifier::save_with_bow(const char *path_model, const char *path_vocab)
{
    svm_.save(path_model);
	FileStorage fs(path_vocab, FileStorage::WRITE);
	fs << "mtx" << vocab_;
}

void BeerClassifier::load_with_bow(const char *path_model, const char *path_vocab)
{
    svm_.load(path_model);
	FileStorage fs(path_vocab, FileStorage::READ);
	fs["mtx"] >> vocab_;
}