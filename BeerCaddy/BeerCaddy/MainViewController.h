//
//  MainViewController.h
//  BeerCaddy
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#import "TagUploadViewController.h"
#import "BeerClassifier.h"
#import "S3DAO.h"

@interface MainViewController : UIViewController <TagUploadViewControllerDelegate, UIImagePickerControllerDelegate, UINavigationControllerDelegate, UIAlertViewDelegate> {
    BeerClassifier bottler;
}

@property (nonatomic, retain) UIImage *toTag;
@property (nonatomic, retain) NSString *prediction;
@property (nonatomic, retain) S3DAO *s3;

- (IBAction)startCamera:(id)sender;
- (NSString *)labelImage:(UIImage *)image;

@end
