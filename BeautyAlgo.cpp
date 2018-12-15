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

#include "BeautyAlgo.hpp"

//垂直方向递归
void RunBEEPSVerticalHorizontal(double *data,int width,int height,double spatialDecay,double *exp_table,double *g_table)
{
    int length0=height*width;
    double* g= new double[length0];
    int m = 0;
    for (int k2 = 0;k2<height;++k2)
    {
        int n = k2;
        for (int k1 = 0;k1<width;++k1)
        {
            g[n]=data[m++];
            n += height;
        }
    }
    double*p = new double[length0];
    double*r = new double[length0];
    memcpy(p, g, sizeof(double) * length0);
    memcpy(r, g, sizeof(double) * length0);
    for (int k1 = 0;k1<width; ++k1)
    {
        int startIndex=k1 * height;
        double mu = 0.0;
        for (int k=startIndex+1,K =startIndex+height;k<K;++k)
        {
            int div0=fabs(p[k]-p[k-1]);
            mu =exp_table[div0];
            p[k] = p[k - 1] * mu + p[k] * (1.0 - mu);//文献中的公式1，这里做了一下修改，效果影响不大
        }
        for (int k =startIndex+height-2;startIndex <= k;--k)
        {
            int div0=fabs(r[k]-r[k+1]);
            mu =exp_table[div0];
            r[k] = r[k+1] * mu + r[k] * (1.0-mu) ;//文献公式3
        }
    }
    double rho0=1.0/(2-spatialDecay);
    for (int k = 0;k <length0;++k)
    {
        r[k]= (r[k]+p[k])*rho0-g_table[(int)g[k]];
    }
    m = 0;
    for (int k1=0;k1<width;++k1)
    {
        int n = k1;
        for (int k2 =0;k2<height;++k2)
        {
            data[n] = r[m++];
            n += width;
        }
    }
    
    memcpy(p,data, sizeof(double) * length0);
    memcpy(r,data, sizeof(double) * length0);
    for (int k2 = 0; k2<height;++k2)
    {
        int startIndex=k2 * width;
        double mu = 0.0;
        for (int k=startIndex+1, K=startIndex+width;k<K;++k)
        {
            int div0=fabs(p[k]-p[k-1]);
            mu =exp_table[div0];
            p[k] = p[k - 1] * mu + p[k] * (1.0 - mu);
        }
        for (int k=startIndex+width-2;startIndex<=k;--k)
        {
            int div0=fabs(r[k]-r[k+1]);
            mu =exp_table[div0];
            r[k] = r[k + 1] * mu + r[k] * (1.0 - mu) ;
        }
    }
    
    double init_gain_mu=spatialDecay/(2-spatialDecay);
    for (int k = 0; k <length0; k++)
    {
        data[k]=(p[k]+r[k])*rho0-data[k]*init_gain_mu;//文献中的公式5
    }
    delete[] p;
    delete[] r;
    delete[] g;
}

//水平方向递归
void RunBEEPSHorizontalVertical(double *data,int width,int height,double spatialDecay,double *exptable,double *g_table)
{
    int length=width*height;
    double* g = new double[length];
    double* p = new double[length];
    double* r = new double[length];
    memcpy(p,data, sizeof(double) * length);
    memcpy(r,data, sizeof(double) * length);
    double rho0=1.0/(2-spatialDecay);
    for (int k2 = 0;k2 < height;++k2)
    {
        int startIndex=k2 * width;
        for (int k=startIndex+1,K=startIndex+width;k<K;++k)
        {
            int div0=fabs(p[k]-p[k-1]);
            double mu =exptable[div0];
            p[k] = p[k - 1] * mu + p[k] * (1.0 - mu);//文献公式1
            
        }
        
        for (int k =startIndex + width - 2;startIndex <= k;--k)
        {
            int div0=fabs(r[k]-r[k+1]);
            double mu =exptable[div0];
            r[k] = r[k + 1] * mu + r[k] * (1.0 - mu);//文献公式3
        }
        for (int k =startIndex,K=startIndex+width;k<K;k++)
        {
            r[k]=(r[k]+p[k])*rho0- g_table[(int)data[k]];
        }
    }

    int m = 0;
    for (int k2=0;k2<height;k2++)
    {
        int n = k2;
        for (int k1=0;k1<width;k1++)
        {
            g[n] = r[m++];
            n += height;
        }
    }
    
    memcpy(p, g, sizeof(double) * height * width);
    memcpy(r, g, sizeof(double) * height * width);
    for (int k1=0;k1<width;++k1)
    {
        int startIndex=k1 * height;
        double mu = 0.0;
        for (int k =startIndex+1,K =startIndex+height;k<K;++k)
        {
            int div0=fabs(p[k]-p[k-1]);
            mu =exptable[div0];
            p[k] = p[k - 1] * mu + p[k] * (1.0 - mu);
        }
        for (int k=startIndex+height-2;startIndex<=k;--k)
        {
            int div0=fabs(r[k]-r[k+1]);
            mu =exptable[div0];
            r[k] = r[k + 1] * mu + r[k] * (1.0 - mu);
        }
    }

    double init_gain_mu=spatialDecay/(2-spatialDecay);
    for (int k = 0;k <length;++k)
    {
        r[k]= (r[k]+p[k])*rho0- g[k]*init_gain_mu;
    }
    m = 0;
    for (int k1=0;k1<width;++k1)
    {
        int n = k1;
        for (int k2=0;k2<height;++k2)
        {
            data[n]=r[m++];
            n += width;
        }
    }
    
    delete p;
    delete r;
    delete g;
}
