//  Ben Klingher, Peter Sugihara
//
//  This object manages transactions with S3. Its main function is to facilitate
//  uploading images and getting a new classifier model.
//
//  Created by Peter Sugihara on 12/6/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <AWSiOSSDK/S3/AmazonS3Client.h>

// TODO: Unhardcode these before release!
#define ACCESS_KEY @"AKIAIGCU5NUUS3OSNVGA"
#define SECRET_KEY @"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"
#define PICTURE_BUCKET @"classified-beer"

#define IMAGE_COMPRESSION .5 // 0.0 is most compressed, 1.0 is uncompressed

#define MODEL_FILE @"bottles.model"

@interface S3DAO : NSObject

@property (nonatomic, retain) AmazonS3Client *s3;

- (void)uploadImage:(UIImage *)image withName:(NSString *)name;
- (void)refreshModel;

@end
