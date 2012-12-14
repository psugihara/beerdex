Beerdex
========

Snap, classify, consume. Gotta catch (and drink) 'em all!

## data/get_pics.py ##

This program fetches images that match a query string provided on the
command line. Each time this program is run, it makes 10 calls to Google's
Custom Search API. The current limit is 100 calls per developer per day,
so only run this program 10 times per day max.

Usage:

	python data/get_pics.py some search query

Dependencies:

* [requests](http://docs.python-requests.org/en/latest/)
* [apiclient](http://code.google.com/p/google-api-python-client/)

## Beerdex ##

This directory contains the Xcode project for the iPhone app. Open the Xcode project and connect a provisioned device to run it. Unfortunately it will not run in the simulator because it requires the camera. Both the OpenCV2 and AWS iOS SDK framework directories must be linked (these are included in the git repo).

## trainer ##

This is a command line utility written in C++. It can be used either to build the SVM model and vocabulary or to run cross validation. The first argument must be the root directory of a training image set, containing a collection of labeled subdirectories. The subdirectories should each contain images that are samples of that label.

Building:
	
	$ cd trainer/build
	$ cmake ..
	$ make
	
Usage:

	$ trainer <image-root> [model-output vocab-output]
