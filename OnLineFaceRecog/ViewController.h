//
//  ViewController.h
//  OnLineFaceRecog
//
//  Created by zhoushiwei on 14/12/23.
//  Copyright (c) 2014年 zhoushiwei. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <opencv2/opencv.hpp>
#import <UIKit/UIKit.h>
#import "Calfps.h"

using namespace cv;
using namespace std;

@class VideoProcess;
@interface ViewController : UIViewController<UIScrollViewDelegate,UIGestureRecognizerDelegate,AVCaptureVideoDataOutputSampleBufferDelegate,UIImagePickerControllerDelegate,UINavigationControllerDelegate,UITextFieldDelegate,UIToolbarDelegate>
{
    Calfps CalTime;

    UIImageView *imageView;
    Mat RGB;
    bool isDetectFace;
    bool iSNeedDetectFace;
    
    UISlider *photometricStandardDeviation_Slider,*spatialDecay_Slider,*guass_Slider;
    double photometricStandardDeviation,spatialDecay;
    int guass_size;
}

@property (nonatomic, retain) IBOutlet UIImageView *imageView;
@property(nonatomic,retain)VideoProcess *m_VideoProcess;
@property(nonatomic,strong)IBOutlet UIButton *registerBution;
@end

