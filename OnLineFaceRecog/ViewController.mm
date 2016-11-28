/*
 The MIT License (MIT)
 Copyright (c) 2015-2016 ShiWei Zhou
 visionhackerstudio@gmail.com
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#import "ViewController.h"
#import "UIImageCVMatConverter.h"
#import "VideoProcess.h"
#import "MyBeauty.h"

@interface ViewController ()
{
    AVCaptureSession * session;
    AVCaptureSession *captureSession;
    NSArray * captureDevices;
    AVCaptureDeviceInput * captureInput;
    AVCaptureVideoDataOutput * captureOutput;
    int currentCameraIndex;
    MyBeauty  MB;

}

@end

@implementation ViewController
@synthesize imageView;
- (void)viewDidLoad {
    [super viewDidLoad];
    CGRect bounds = [UIScreen mainScreen].bounds;
    self.imageView = [[UIImageView alloc] init];
    self.imageView.frame = CGRectMake(0, 0,bounds.size.width, bounds.size.height);
    self.imageView.contentMode=UIViewContentModeScaleAspectFill;
    self.imageView.backgroundColor = [UIColor clearColor];
    [self.view addSubview:self.imageView];
    self.m_VideoProcess = [[VideoProcess alloc] init];
    self.m_VideoProcess.m_captureImageType = MPVideoProcessorCaptureColorImageRGB;
    [self.m_VideoProcess setupAVCaptureSession];
    //< higher quality
    [self.m_VideoProcess.m_avSession setSessionPreset:AVCaptureSessionPreset640x480];
    [self.m_VideoProcess startAVSessionWithBufferDelegate:self];
 
        // Do any additional setup after loading the view, typically from a nib.
}





Mat output;
-(void)captureOutput:(AVCaptureOutput *)captureOutput
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
      fromConnection:(AVCaptureConnection *)connection
{
    CalTime.startCal();
    CVImageBufferRef imageBuffer =  CMSampleBufferGetImageBuffer(sampleBuffer);
    CVPixelBufferLockBaseAddress(imageBuffer, 0);
    
    uint8_t *baseAddress = (uint8_t *)CVPixelBufferGetBaseAddress(imageBuffer);
    size_t width = CVPixelBufferGetWidth(imageBuffer);
    size_t height = CVPixelBufferGetHeight(imageBuffer);
    
    Mat image=Mat((int)height,(int)width,CV_8UC4,baseAddress);
    cvtColor(image, RGB, COLOR_BGRA2BGR);
    Mat Dest=MB.ApplyZSWBeauty(RGB);
    cvtColor(Dest, output, CV_BGR2RGB);

    CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
    
    dispatch_sync(dispatch_get_main_queue(), ^{
        
        [self showVideoThread];
    });
    
}

-(void)showVideoThread
{
    imageView.image=[UIImageCVMatConverter UIImageFromCVMat:output];
}
    
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];

    // Dispose of any resources that can be recreated.
}

@end
