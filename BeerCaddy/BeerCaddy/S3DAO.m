//  Ben Klingher, Peter Sugihara
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#import "S3DAO.h"

@implementation S3DAO

- (id)init
{
    if ((self = [super init]) && self.s3 == nil) {
        _s3 = [[AmazonS3Client alloc] initWithAccessKey:ACCESS_KEY withSecretKey:SECRET_KEY];
        return self;
    }
    return nil;
}

- (void)uploadImage:(UIImage *)image withName:(NSString *)name
{
    NSData *imageData = UIImageJPEGRepresentation(image, IMAGE_COMPRESSION);

    double time = [[NSDate date] timeIntervalSince1970];
    NSString *fileName = [NSString stringWithFormat:@"%@-%f.jpg", name, time];

    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_async(queue, ^{
        NSLog(@"Uploading %@", fileName);

        // Upload image data.  Remember to set the content type.
        S3PutObjectRequest *put = [[S3PutObjectRequest alloc] initWithKey:fileName
                                                                 inBucket:PICTURE_BUCKET];
        put.contentType = @"image/jpeg";
        put.data        = imageData;

        // Put the image data into the specified s3 bucket and object.
        S3PutObjectResponse *putObjectResponse = [_s3 putObject:put];

        dispatch_async(dispatch_get_main_queue(), ^{

            if (putObjectResponse.error != nil) {
                NSLog(@"Error: %@", putObjectResponse.error);
            } else {
                NSLog(@"Upload OK!");
            }

            [[UIApplication sharedApplication] setNetworkActivityIndicatorVisible:NO];
        });
    });
}

- (void)downloadFile:(NSString *)name toPath:(NSString *)path
{
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_async(queue, ^{
        NSLog(@"Fetching %@", MODEL_FILE);

        // Upload image data.  Remember to set the content type.
        S3GetObjectRequest *get = [[S3GetObjectRequest alloc] initWithKey:name
                                                               withBucket:PICTURE_BUCKET];

        // Put the image data into the specified s3 bucket and object.
        S3GetObjectResponse *getObjectResponse = [_s3 getObject:get];

        dispatch_async(dispatch_get_main_queue(), ^{

            if (getObjectResponse.error != nil) {
                NSLog(@"Error: %@", getObjectResponse.error);
            } else {
                [getObjectResponse setOutputStream:[NSOutputStream outputStreamToFileAtPath:path append:NO]];
                NSLog(@"Download OK!");
            }

            [[UIApplication sharedApplication] setNetworkActivityIndicatorVisible:NO];
        });
    });
}

- (void)refreshModel
{
    NSString *modelPath = [[NSBundle bundleForClass:[self class]] pathForResource:@"model"
                                                                           ofType:@"yml"];
    [self downloadFile:@"model.yml" toPath:modelPath];

    NSString *vocabPath = [[NSBundle bundleForClass:[self class]] pathForResource:@"vocab"
                                                                           ofType:@"yml"];
    [self downloadFile:@"vocab.yml" toPath:vocabPath];
}

@end
