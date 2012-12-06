//
//  MainViewController.h
//  BeerCaddy
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#import "FlipsideViewController.h"
#import "BeerClassifier.h"

@interface MainViewController : UIViewController <FlipsideViewControllerDelegate, UIImagePickerControllerDelegate, UINavigationControllerDelegate> {
@public
    BeerClassifier bottler;
}

- (NSString *)labelImage:(UIImage *)image;

@end
