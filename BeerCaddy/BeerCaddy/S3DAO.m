//
//  S3DAO.m
//  BeerCaddy
//
//  Created by Peter Sugihara on 12/6/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#import "S3DAO.h"

@implementation S3DAO

- (id)init
{
    if ((self = [super init]) && self.s3 == nil) {
        self.s3 = [[AmazonS3Client alloc] initWithAccessKey:ACCESS_KEY_ID withSecretKey:SECRET_KEY];
        return self;
    }
    return nil;
}

- (void)uploadImage:(UIImage *)image withName:(NSString *)name
{
    NSData *imageData = UIImagePNGRepresentation(image);

    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_async(queue, ^{

        // Upload image data.  Remember to set the content type.
        S3PutObjectRequest *put = [[S3PutObjectRequest alloc] initWithKey:name
                                                                  inBucket:PICTURE_BUCKET];
        put.contentType = @"image/png";
        put.data        = imageData;

        // Put the image data into the specified s3 bucket and object.
        S3PutObjectResponse *putObjectResponse = [self.s3 putObject:put];

        dispatch_async(dispatch_get_main_queue(), ^{

            if (putObjectResponse.error != nil) {
                NSLog(@"Error: %@", putObjectResponse.error);
            } else {
                // UPLOAD OK!
            }

            [[UIApplication sharedApplication] setNetworkActivityIndicatorVisible:NO];
        });
    });
}

@end
