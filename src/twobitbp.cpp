#include "twobitbp.h"

namespace tld {

TwoBitBP::TwoBitBP(float minScale, float maxScale) : Feature(minScale, maxScale)
{
    //ctor
}

TwoBitBP::~TwoBitBP()
{
    //dtor
}


int TwoBitBP::evaluate(Mat& integralImage, BoundingBox patch)
{
    int eval = 0;

    /*******************************************

            A------G------B
            |      |      |
            |      |      |
            E------|------F
            |      |      |
            |      |      |
            C------H------D


            AreaABCD = D - C - B + A

    ******************************************/

    double x = (int)(xp * double(patch.w)) + patch.x;
    double y = (int)(yp * double(patch.h)) + patch.y;
    double w = (int)((wp * double(patch.w)) * 0.5);
    double h = (int)((hp * double(patch.h)) * 0.5);
    double xf = x + w;
    double yf = y + h;


    double A = integralImage.at<double>(y, x);
    double B = integralImage.at<double>(y, xf);
    double C = integralImage.at<double>(yf, x);
    double D = integralImage.at<double>(yf, xf);
    double E = integralImage.at<double>(y + h/2, x);
    double F = integralImage.at<double>(y + h/2, xf);
    double G = integralImage.at<double>(y, x + w/2);
    double H = integralImage.at<double>(yf, x + w/2);

    double A1 = D - C - F + E;
    double A2 = F - E - B + A;

    if( A1 > A2 ) eval |= 1;

    eval <<= 1;

    double B1 = H - C - G + A;
    double B2 = D - H - B + G;

    if( B1 > B2 ) eval |= 1;

    return eval;
}

}

