//
//  MainViewController.m
//  BeerCaddy
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#import "MainViewController.h"

@interface MainViewController ()
@end

@implementation MainViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    _s3 = [[S3DAO alloc] init];

    NSString *modelPath = [[NSBundle bundleForClass:[self class]] pathForResource:@"model"
                                                                           ofType:@"yml"];
    NSString *vocabPath = [[NSBundle bundleForClass:[self class]] pathForResource:@"vocab"
                                                                           ofType:@"yml"];
    if (modelPath && vocabPath)
        bottler.load_with_bow([modelPath UTF8String], [vocabPath UTF8String]);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - TagUpload View

- (void)tagUploadViewControllerDidFinish:(TagUploadViewController *)controller
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([[segue identifier] isEqualToString:@"tagCapture"]) {
        [[segue destinationViewController] setDelegate:self];
    }
}

#pragma mark - Camera

- (IBAction)startCamera:(id)sender
{
    [self startCameraControllerFromViewController:self
                                    usingDelegate:self];
}

- (BOOL)startCameraControllerFromViewController:(UIViewController*) controller
                                   usingDelegate:(id <UIImagePickerControllerDelegate,
                                                   UINavigationControllerDelegate>) delegate {

    if (([UIImagePickerController isSourceTypeAvailable:
          UIImagePickerControllerSourceTypeCamera] == NO)
        || (delegate == nil)
        || (controller == nil))
        return NO;

    UIImagePickerController *cameraUI = [[UIImagePickerController alloc] init];
    cameraUI.sourceType = UIImagePickerControllerSourceTypeCamera;

    // Only enable still picture capture.
    cameraUI.mediaTypes = [[NSArray alloc] initWithObjects: (NSString *) kUTTypeImage, nil];

    // Hides the controls for moving & scaling pictures, or for
    // trimming movies. To instead show the controls, use YES.
    cameraUI.allowsEditing = NO;

    cameraUI.delegate = delegate;

    UIImageView *bottleOverlay = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"beerbox.png"]];
    bottleOverlay.frame = CGRectMake(0, 0, bottleOverlay.image.size.width, bottleOverlay.image.size.height);
    cameraUI.cameraOverlayView = bottleOverlay;

    [controller presentViewController:cameraUI animated:YES completion:nil];
    return YES;
}

// For responding to the user tapping Cancel.
- (void)imagePickerControllerDidCancel: (UIImagePickerController *) picker {
    [[picker presentingViewController] dismissViewControllerAnimated:YES completion:nil];
}

// For responding to the user accepting a newly-captured picture or movie
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info {

    NSString *mediaType = [info objectForKey: UIImagePickerControllerMediaType];
    UIImage *originalImage, *editedImage, *imageToSave;

    // Handle a still image capture
    if (CFStringCompare ((CFStringRef) mediaType, kUTTypeImage, 0)
        == kCFCompareEqualTo) {

        editedImage = (UIImage *) [info objectForKey:
                                   UIImagePickerControllerEditedImage];
        originalImage = (UIImage *) [info objectForKey:
                                     UIImagePickerControllerOriginalImage];

        if (editedImage) {
            imageToSave = editedImage;
        } else {
            imageToSave = originalImage;
        }

        _prediction = [self labelImage:imageToSave];
        NSString *message = [NSString stringWithFormat:@"I think that's a %@ bottle?", _prediction];
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Prediction!"
                                                        message:message
                                                       delegate:self
                                              cancelButtonTitle:@"Yep!"
                                              otherButtonTitles:@"Nope!", nil];
        [alert show];
    }

    [[picker presentingViewController] dismissViewControllerAnimated:YES completion:nil];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	if (buttonIndex == 0) { // correct
        [_s3 uploadImage:_toTag withName:_prediction];
    } else if (buttonIndex == 1) { // incorrect
        [self performSegueWithIdentifier:@"tagCapture" sender: self];
    }
}

#pragma mark Image recognition

- (NSString *)labelImage:(UIImage *)image
{
	// Reformat image and classify.
	cv::Mat img = [self cvMatFromUIImage:image];

    std::cout << img.size() << std::endl;
    _toTag = [self UIImageFromCVMat:bottler.convert(img)];


    UIImageWriteToSavedPhotosAlbum(_toTag, nil, nil, nil);

	int label = bottler.label_bow(img);

	NSArray *beers = @[@"bud_light", @"budweiser", @"coors_light", @"corona",
	@"heineken", @"magic_hat", @"rolling_rock", @"sierra_nevada",
	@"stella_artois", @"yuengling"];

	return beers[label];
}

- (cv::Mat)cvMatFromUIImage:(UIImage *)image
{
    CGColorSpaceRef colorSpace = CGImageGetColorSpace(image.CGImage);
    CGFloat cols = image.size.width;
    CGFloat rows = image.size.height;

	NSLog(@"%f", cols);

    cv::Mat cvMat(rows, cols, CV_8UC4); // 8 bits per component, 4 channels

    CGContextRef contextRef = CGBitmapContextCreate(cvMat.data,                 // Pointer to  data
                                                    cols,                       // Width of bitmap
                                                    rows,                       // Height of bitmap
                                                    8,                          // Bits per component
                                                    cvMat.step[0],              // Bytes per row
                                                    colorSpace,                 // Colorspace
                                                    kCGImageAlphaNoneSkipLast |
                                                    kCGBitmapByteOrderDefault); // Bitmap info flags

    CGContextDrawImage(contextRef, CGRectMake(0, 0, cols, rows), image.CGImage);
    CGContextRelease(contextRef);
    CGColorSpaceRelease(colorSpace);

    // transpose for portrait orientation
    cv::flip(cvMat, cvMat, 0);
    return cvMat.t();
}

- (UIImage *)UIImageFromCVMat:(cv::Mat)cvMat
{
	NSData *data = [NSData dataWithBytes:cvMat.data length:cvMat.elemSize()*cvMat.total()];
	CGColorSpaceRef colorSpace;

	if (cvMat.elemSize() == 1) {
		colorSpace = CGColorSpaceCreateDeviceGray();
	} else {
		colorSpace = CGColorSpaceCreateDeviceRGB();
	}

	CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)data);

	// Creating CGImage from cv::Mat
	CGImageRef imageRef = CGImageCreate(cvMat.cols,                                 //width
					    cvMat.rows,                                 //height
					    8,                                          //bits per component
					    8 * cvMat.elemSize(),                       //bits per pixel
					    cvMat.step[0],                            //bytesPerRow
					    colorSpace,                                 //colorspace
					    kCGImageAlphaNone|kCGBitmapByteOrderDefault,// bitmap info
					    provider,                                   //CGDataProviderRef
					    NULL,                                       //decode
					    false,                                      //should interpolate
					    kCGRenderingIntentDefault                   //intent
					    );

	// Getting UIImage from CGImage
	UIImage *finalImage = [UIImage imageWithCGImage:imageRef];
	CGImageRelease(imageRef);
	CGDataProviderRelease(provider);
	CGColorSpaceRelease(colorSpace);

	return finalImage;
}

@end
