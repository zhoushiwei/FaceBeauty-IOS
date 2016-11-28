//
//  Calfps.h
//  MagicFace
//
//  Created by zhoushiwei on 14/10/23.
//  Copyright (c) 2014年 zhoushiwei. All rights reserved.
//

#ifndef __MagicFace__Calfps__
#define __MagicFace__Calfps__

#include <stdio.h>
#include "opencv2/opencv.hpp"
class Calfps{
public:
    double fps;
    int64 startime,stoptime,caltime;
    int framenum;
    
    Calfps(){
        this->resetTime();
    }
    void startCal(){
        framenum++;
        startime=cvGetTickCount();
        
    }
    void stopCal(){
        stoptime=cvGetTickCount();
        caltime+=stoptime-startime;
        if (framenum>=29) {
            fps=30.0/((double(caltime)/cvGetTickFrequency())/1e+6);
            framenum=0;
            caltime=0;
        }
    }
    
    void resetTime(){
        startime=cvGetTickCount();
        caltime=0;
        fps=0;
        framenum=0;
        
    }
    
    void write_fps_image(cv::Mat&image,cv::Point &position){
        char timestr[256];
        sprintf(timestr, "FPS: %d fps",(int)round(fps));
        std::string text=timestr;
        cv::putText(image, text, position, cv::FONT_HERSHEY_SIMPLEX, 1.6, cv::Scalar(0,255,0), 3);
        return;
    }
    
private:
    
};
#endif /* defined(__MagicFace__Calfps__) */
