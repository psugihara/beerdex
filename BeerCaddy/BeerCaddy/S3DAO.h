//
//  S3DAO.h
//  BeerCaddy
//
//  This object manages transactions with S3. Its main function is to facilitate
//  uploading images and getting a new classifier model.
//
//  Created by Peter Sugihara on 12/6/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <AWSiOSSDK/S3/AmazonS3Client.h>

#define ACCESS_KEY_ID @"someaccesskey"
#define SECRET_KEY @"secret"
#define PICTURE_BUCKET @"buck"


@interface S3DAO : NSObject

@property (nonatomic, retain) AmazonS3Client *s3;

- (void)uploadImage:(UIImage *)image withName:(NSString *)name;

@end
