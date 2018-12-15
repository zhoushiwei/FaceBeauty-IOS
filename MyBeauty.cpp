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

#include "MyBeauty.h"
#include "BeautyAlgo.hpp"
class ParellelProcess:public cv::ParallelLoopBody
{
public:
    ParellelProcess(
                    double photometricStandardDeviation_,
                    double spatialDecay_,
                    Mat &show_,
                    Mat &src_)
    :ParallelLoopBody(), photometricStandardDeviation(photometricStandardDeviation_), spatialDecay(spatialDecay_),show(show_), src(src_)
    {
    }
    void operator ()(const cv::Range& range) const
    {
        
        double* m_exp_table=new double[256];
        double*m_g_table=new double[256];
        double c=-0.5/(photometricStandardDeviation * photometricStandardDeviation);
        double mu=spatialDecay/(2-spatialDecay);
        int m_nWidth=src.cols;
        int m_nHeight=src.rows;
        for (int i=0;i<=255;i++)
        {
            m_exp_table[i]=(1-spatialDecay)* exp(c*i*i);
            m_g_table[i]=mu*i;
        }
        
        int m_length =m_nWidth*m_nHeight;
        std::vector<Mat> planes;
        split(src,planes);
    for (int idxChannel = range.start; idxChannel < range.end; ++idxChannel) {
  
            double *data1 = new double[m_length];
            double *data2 = new double[m_length];
            uchar *graydata=planes[idxChannel].data;
            
            memcpy(data1, graydata, m_length*sizeof(double));
            memcpy(data2, graydata, m_length*sizeof(double));
            double  time=cv::getTickCount();
            RunBEEPSHorizontalVertical(data1, m_nWidth, m_nHeight,spatialDecay,m_exp_table,m_g_table);
            RunBEEPSVerticalHorizontal(data2, m_nWidth, m_nHeight,spatialDecay,m_exp_table,m_g_table);
        time=cv::getTickCount()-time;
        printf("time%.2f\n",time*1000/cv::getTickFrequency());
            for (int k = 0; k < m_length; k++) {
                
                data1[k] = 0.5F * (data1[k] + data2[k]);
                
            }
            for (int y=0; y<m_nHeight; y++) {
                for (int x=0; x<m_nWidth; x++) {
                    if (data1[m_nWidth*y+x]<0) {
                        data1[m_nWidth*y+x]=0.0;
                    }
                    show.at<Vec3b>(y,x)(idxChannel)=(int)data1[m_nWidth*y+x];
                }
            }
            
            delete[] data1;
            delete[] data2;
        }
        
        delete[] m_exp_table;m_exp_table=NULL;
        delete[] m_g_table;m_g_table=NULL;
    
    }
private:

    double photometricStandardDeviation;
    double spatialDecay;
    Mat &show;
    Mat &src;
};

MyBeauty::MyBeauty()
{
    
}


Mat MyBeauty::ApplyZSWBeauty(Mat &RGB)
{
    double photometricStandardDeviation=10;
    double spatialDecay=0.02;
    Mat show(RGB.size(),RGB.type(),Scalar::all(0));
    Mat src;
    RGB.convertTo(src, CV_64F);
   ParellelProcess body(photometricStandardDeviation,spatialDecay,show,src);
    cv::parallel_for_(cv::Range(0,3), body);
    
    Mat highPass=show;
    Mat highPass2=highPass-RGB+128;
    GaussianBlur(highPass2, highPass2, cv::Size(5,5),0,0);
    Mat temp=RGB+2*highPass2-255;
    float Opacity=50.0;
    Mat Dest=(RGB*(100-Opacity)+(temp)*Opacity)/100.0;
    
    return Dest;
}
