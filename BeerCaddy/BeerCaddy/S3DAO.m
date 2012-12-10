//  Ben Klingher, Peter Sugihara
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#import "S3DAO.h"

#define COMPRESSION .5 // 0.0 is most compressed, 1.0 is uncompressed

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
    NSData *imageData = UIImageJPEGRepresentation(image, COMPRESSION);

    NSString *fileName = [NSString stringWithFormat:@"%@.jpg", name];

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

@end
