//
//  MainViewController.h
//  BeerCaddy
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#import "FlipsideViewController.h"
#import "BeerClassifier.h"
#import "S3DAO.h"

@interface MainViewController : UIViewController <FlipsideViewControllerDelegate, UIImagePickerControllerDelegate, UINavigationControllerDelegate, UIAlertViewDelegate> {
    BeerClassifier bottler;
}

@property (nonatomic, retain) S3DAO *s3;

- (NSString *)labelImage:(UIImage *)image;

@end
