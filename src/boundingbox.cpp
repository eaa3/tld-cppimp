#include "boundingbox.h"


namespace tld
{

BoundingBox::BoundingBox(int x, int y, int w, int h, float conf, bool valid, bool detected, float post)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->xf = x + w;
    this->yf = y + h;
    this->conf = conf;
    this->valid = valid;
    this->detected = detected;
    this->post = post;
    this->var = 0;

    this->fell = false;
    this->sx = this->sy = 1;
    this->magnitude = this->cosine = 0;

}

BoundingBox::BoundingBox( const BoundingBox& other)
{
    this->x = other.x;
    this->y = other.y;
    this->w = other.w;
    this->h = other.h;
    this->xf = other.xf;
    this->yf = other.yf;
    this->conf = other.conf;
    this->valid = other.valid;
    this->detected = other.detected;
    this->post = other.post;
    this->var = other.var;

    this->fell = other.fell;

    this->sx = other.sx;
    this->sy = other.sy;

    this->cosine = other.cosine;
    this->magnitude = other.magnitude;
}

BoundingBox::BoundingBox( const Rect& other )
{
    (*this) = BoundingBox(other.x, other.y, other.width, other.height);
}

BoundingBox::~BoundingBox()
{
    //dtor
}


void BoundingBox::setX(int x)
{
    this->x = x;
    this->xf = x + this->w;
}

void BoundingBox::setY(int y)
{
    this->y = y;
    this->yf = y + this->h;
}

void BoundingBox::setXY(int x, int y)
{
    this->x = x;
    this->y = y;

    this->xf = x + this->w;
    this->yf = y + this->h;
}

void BoundingBox::setW(int w)
{
    this->w = w;
    this->xf = this->x + w;
}
void BoundingBox::setH(int h)
{
    this->h = h;
    this->yf = this->y + h;
}
void BoundingBox::setWH(int w, int h)
{
    this->w = w;
    this->xf = this->x + w;

    this->h = h;
    this->yf = this->y + h;
}

int BoundingBox::getArea()
{
    return this->w*this->h;
}

double BoundingBox::getArea( Mat& integralImage)
{
    double A = integralImage.at<double>(this->y,this->x);
    double B = integralImage.at<double>(this->y,this->xf);
    double C = integralImage.at<double>(this->yf,this->x);
    double D = integralImage.at<double>(this->yf,this->xf);

    return D - C - B + A;
}


bool BoundingBox::isOverlapping(  BoundingBox& other )
{
    if( this->x >= other.xf || this->xf <= other.x || this->y >= other.yf || this->yf <= other.y ) return false;
    else return true;
}



bool BoundingBox::isOverlapping(  BoundingBox& other, float minOverlapArea )
{
    return this->overlap(other) >= minOverlapArea;
}

bool BoundingBox::isInside(  BoundingBox& other )
{
    return (this->x >= other.x && this->y >= other.y && this->xf < other.xf && this->yf < other.yf);
}

float BoundingBox::overlap(  BoundingBox& other )
{
    if( !this->isOverlapping(other) ) return 0.0f;


    int x = max(this->x, other.x);
    int y = max(this->y, other.y);
    int xf = min(this->xf, other.xf);
    int yf = min(this->yf, other.yf);
    int w = xf - x;
    int h = yf - y;

    BoundingBox intersecBox(x, y, w, h );

    return float(intersecBox.getArea())/(this->getArea() + other.getArea() - intersecBox.getArea());
}

void BoundingBox::overlap( vector<BoundingBox>& others, float* overlaps )
{
    for(int i = 0; i < others.size(); i++)
    {
        overlaps[i] = this->overlap(others[i]);
    }
}

void BoundingBox::overlap(  vector<BoundingBox*>& others, float* overlaps )
{
    for(int i = 0; i < others.size(); i++)
    {
        overlaps[i] = this->overlap(*others[i]);
    }
}

Mat BoundingBox::extractPatch( Mat& image)
{
    Mat patch = image(*this).clone();

    return patch;
}

NormalizedPatch BoundingBox::extractNormalizedPatch( Mat& image, int dimension)
{
    return NormalizedPatch(image, *this, dimension);
}

BoundingBox::operator Rect()
{
    return Rect(this->x, this->y, this->w, this->h);
}

}
