//  Ben Klingher, Peter Sugihara
//
//  Created by Peter Sugihara on 11/24/12.
//  Copyright (c) 2012 Peter Sugihara. All rights reserved.
//

#import "TagUploadViewController.h"

@interface TagUploadViewController ()
@property (nonatomic, retain) NSArray *beers;
@end

@implementation TagUploadViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    _beers = @[@"bud_light", @"budweiser", @"coors_light", @"corona",
    @"heineken", @"magic_hat", @"rolling_rock", @"sierra_nevada",
    @"stella_artois", @"yuengling"];

    _s3 = [[S3DAO alloc] init];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Actions

- (IBAction)done:(id)sender
{
    [self.delegate tagUploadViewControllerDidFinish:self];
}

#pragma mark - UITableViewDataSource

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	int i = indexPath.row;

	UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault
						       reuseIdentifier:_beers[i]];
	cell.textLabel.text = _beers[i];
	return cell;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	if (section == 0)
		return [_beers count];
	return 0;
}

#pragma mark - UITableViewDelegate
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Thanks!"
                                                    message:@"Its always good to learn something new."
                                                   delegate:nil
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];

    [_s3 uploadImage:_delegate.toTag withName:_beers[indexPath.row]];

    [self.delegate tagUploadViewControllerDidFinish:self];
}


@end
