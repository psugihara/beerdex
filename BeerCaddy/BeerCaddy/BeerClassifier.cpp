//
//  BeerClassifier.cpp
//  BeerCaddy
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#include "BeerClassifier.h"

#define FRAMES 8
#define BOW_FRAMES 35


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

	// this set of keypoints with 1000 clusters will return:
	// 90% in cross validation using the lbw-a dataset

	// y range of x range
	// 80 to 220 of 150 to 210

    keypoints.push_back(KeyPoint(150, 80, 16));
    keypoints.push_back(KeyPoint(170, 80, 16));
    keypoints.push_back(KeyPoint(190, 80, 16));
	keypoints.push_back(KeyPoint(210, 80, 16));

	keypoints.push_back(KeyPoint(150, 100, 16));
    keypoints.push_back(KeyPoint(170, 100, 16));
    keypoints.push_back(KeyPoint(190, 100, 16));
	keypoints.push_back(KeyPoint(210, 100, 16));

	keypoints.push_back(KeyPoint(150, 120, 16));
    keypoints.push_back(KeyPoint(170, 120, 16));
    keypoints.push_back(KeyPoint(190, 120, 16));
	keypoints.push_back(KeyPoint(210, 120, 16));

	keypoints.push_back(KeyPoint(150, 140, 16));
    keypoints.push_back(KeyPoint(170, 140, 16));
    keypoints.push_back(KeyPoint(190, 140, 16));
	keypoints.push_back(KeyPoint(210, 140, 16));

	keypoints.push_back(KeyPoint(150, 160, 16));
    keypoints.push_back(KeyPoint(170, 160, 16));
    keypoints.push_back(KeyPoint(190, 160, 16));
	keypoints.push_back(KeyPoint(210, 160, 16));

	keypoints.push_back(KeyPoint(150, 180, 16));
    keypoints.push_back(KeyPoint(170, 180, 16));
    keypoints.push_back(KeyPoint(190, 180, 16));
	keypoints.push_back(KeyPoint(210, 180, 16));

	keypoints.push_back(KeyPoint(150, 200, 16));
    keypoints.push_back(KeyPoint(170, 200, 16));
    keypoints.push_back(KeyPoint(190, 200, 16));
	keypoints.push_back(KeyPoint(210, 200, 16));

	keypoints.push_back(KeyPoint(150, 200, 16));
    keypoints.push_back(KeyPoint(170, 220, 16));
    keypoints.push_back(KeyPoint(190, 220, 16));
	keypoints.push_back(KeyPoint(210, 200, 16));



	// y range of x range
	// 250 to 450 of 130 to 230

	keypoints.push_back(KeyPoint(130, 250, 16));
    keypoints.push_back(KeyPoint(155, 250, 16));
    keypoints.push_back(KeyPoint(180, 250, 16));
	keypoints.push_back(KeyPoint(205, 250, 16));
	keypoints.push_back(KeyPoint(230, 250, 16));

	keypoints.push_back(KeyPoint(130, 275, 16));
    keypoints.push_back(KeyPoint(155, 275, 16));
    keypoints.push_back(KeyPoint(180, 275, 16));
	keypoints.push_back(KeyPoint(205, 275, 16));
	keypoints.push_back(KeyPoint(230, 275, 16));

	keypoints.push_back(KeyPoint(130, 300, 16));
    keypoints.push_back(KeyPoint(155, 300, 16));
    keypoints.push_back(KeyPoint(180, 300, 16));
	keypoints.push_back(KeyPoint(205, 300, 16));
	keypoints.push_back(KeyPoint(230, 300, 16));

	keypoints.push_back(KeyPoint(130, 325, 16));
    keypoints.push_back(KeyPoint(155, 325, 16));
    keypoints.push_back(KeyPoint(180, 325, 16));
	keypoints.push_back(KeyPoint(205, 325, 16));
	keypoints.push_back(KeyPoint(230, 325, 16));

	keypoints.push_back(KeyPoint(130, 350, 16));
    keypoints.push_back(KeyPoint(155, 350, 16));
    keypoints.push_back(KeyPoint(180, 350, 16));
	keypoints.push_back(KeyPoint(205, 350, 16));
	keypoints.push_back(KeyPoint(230, 350, 16));

	keypoints.push_back(KeyPoint(130, 375, 16));
    keypoints.push_back(KeyPoint(155, 375, 16));
    keypoints.push_back(KeyPoint(180, 375, 16));
	keypoints.push_back(KeyPoint(205, 375, 16));
	keypoints.push_back(KeyPoint(230, 375, 16));

	keypoints.push_back(KeyPoint(130, 400, 16));
    keypoints.push_back(KeyPoint(155, 400, 16));
    keypoints.push_back(KeyPoint(180, 400, 16));
	keypoints.push_back(KeyPoint(205, 400, 16));
	keypoints.push_back(KeyPoint(230, 400, 16));

	keypoints.push_back(KeyPoint(130, 425, 16));
    keypoints.push_back(KeyPoint(155, 425, 16));
    keypoints.push_back(KeyPoint(180, 425, 16));
	keypoints.push_back(KeyPoint(205, 425, 16));
	keypoints.push_back(KeyPoint(230, 425, 16));
	
	return keypoints;
}

static Mat extract_bow(Mat& im, vector<KeyPoint> keypoints, Mat& vocab)
{

	SiftDescriptorExtractor *extractor = new SiftDescriptorExtractor();
	FlannBasedMatcher *matcher = new FlannBasedMatcher();

	BOWImgDescriptorExtractor bow_extractor(extractor, matcher);
	bow_extractor.setVocabulary(vocab);

	Mat bow_descriptor;

	bow_extractor.compute(im, keypoints, bow_descriptor);

    return bow_descriptor.reshape(1, 1);
}

static Mat extract_feats(Mat& im, vector<KeyPoint> keypoints)
{
	// Change image to desired format (540x360 CV_8UC3).
    Mat resize_dest(540, 360, im.type());
	resize(im, resize_dest, resize_dest.size(), 0, 0, CV_INTER_AREA);

    Mat convert_dest(540, 360, CV_8UC3);
    cvtColor(resize_dest, convert_dest, CV_BGRA2BGR);

	SiftDescriptorExtractor extractor;
    Mat descriptors;
    extractor.compute(convert_dest, keypoints, descriptors);

    return descriptors.reshape(1, 1);
}

Mat BeerClassifier::extract_desc_bow(vector<Mat> &train_imgs, Mat &labels)
{
	Mat descriptors;
	int cluster_num = 1000;

	int i = 0;
	int skipped = 0;

    vector<Mat>::iterator it;

	vector<KeyPoint> keypoints = generate_keypoints_bow();

	//SiftFeatureDetector detector;
	//vector<KeyPoint> keypoints2;

	Mat l2;

	for (i = 0, it = train_imgs.begin(); it < train_imgs.end(); it++, i++) {

		if ((*it).size().height < 540 || (*it).size().width < 360) {
			skipped++;
			i++;
			continue;
		}

		l2.push_back(labels.row(i));

		Mat resize_dest(540, 360, (*it).type());
		resize(*it, resize_dest, resize_dest.size(), 0, 0, CV_INTER_AREA);

		Mat convert_dest(540, 360, CV_8UC3);
		cvtColor(resize_dest, convert_dest, CV_BGRA2BGR);

		//detector.detect(convert_dest,keypoints);

		SiftDescriptorExtractor extractor;

		Mat descriptor;

		extractor.compute(convert_dest, keypoints, descriptor);

		descriptors.push_back(descriptor);
	}

	BOWKMeansTrainer bow_trainer(cluster_num);

	Mat vocab = bow_trainer.cluster(descriptors);

	vocab_ = vocab;

	SiftDescriptorExtractor *extractor = new SiftDescriptorExtractor();
	FlannBasedMatcher *matcher = new FlannBasedMatcher();

	BOWImgDescriptorExtractor bow_extractor(extractor, matcher);
	bow_extractor.setVocabulary(vocab);

	Mat bow_descriptors(train_imgs.size() - skipped, cluster_num, CV_32F);

	i = 0;
	skipped = 0;
	for (it = train_imgs.begin(); it < train_imgs.end(); it++) {

		if ((*it).size().height < 540 || (*it).size().width < 360) {
			skipped++;
			continue;
		}

		Mat bow_descriptor;

		Mat resize_dest(540, 360, (*it).type());
		resize(*it, resize_dest, resize_dest.size(), 0, 0, CV_INTER_AREA);

		Mat convert_dest(540, 360, CV_8UC3);
		cvtColor(resize_dest, convert_dest, CV_BGRA2BGR);

		//detector.detect(convert_dest,keypoints2);

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
    params.kernel_type = CvSVM::LINEAR;
//    params.degree = 2;
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

    CvMat feats = extract_bow(sample_image, keypoints, vocab_);
    return svm_.predict(&feats);
}

float BeerClassifier::cross_validate(vector<Mat> &train_imgs, Mat &labels)
{
    // Get all descriptors so we just calculate these once.
    Mat descriptors(train_imgs.size(), FRAMES * 128, CV_32FC1);

	vector<KeyPoint> keypoints = generate_keypoints();

	int i = 0;
    vector<Mat>::iterator it;
	for (it = train_imgs.begin(); it < train_imgs.end(); it++) {
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
}

void BeerClassifier::load_with_bow(const char *path_model, const char *path_vocab)
{
    svm_.load(path_model);
	FileStorage fs(path_vocab, FileStorage::READ);
}