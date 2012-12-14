//  Ben Klingher, Peter Sugihara
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "S3DAO.h"

@class TagUploadViewController;

@protocol TagUploadViewControllerDelegate
@property (nonatomic, retain) UIImage *toTag;
- (void)tagUploadViewControllerDidFinish:(TagUploadViewController *)controller;
@end

@interface TagUploadViewController : UIViewController <UITableViewDataSource, UITableViewDelegate>

@property (nonatomic, retain) S3DAO *s3;
@property (weak, nonatomic) id <TagUploadViewControllerDelegate> delegate;

- (IBAction)done:(id)sender;

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath;
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section;

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath;

@end
