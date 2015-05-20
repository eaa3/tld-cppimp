#include "ffclassifier.h"

namespace tld
{

FFClassifier::FFClassifier(int nFerns, int nNodes, float minScale, float maxScale, float threshold)
{
    this->nFerns = nFerns;
    this->nNodes = nNodes;
    this->minScale = minScale;
    this->maxScale = maxScale;
    this->threshold = threshold;

    this->init(nFerns, nNodes, minScale, maxScale);

}

FFClassifier::~FFClassifier()
{
    this->release();
}


void FFClassifier::init(int nFerns, int nNodes, float minScale, float maxScale)
{
    this->ferns.resize(nFerns);

    for(int i = 0; i < this->ferns.size(); i++) this->ferns[i] = new Fern(nNodes, minScale, maxScale);
}

void FFClassifier::release()
{
    for(int i = 0; i < this->ferns.size(); i++)
    {
        delete ferns[i];
    } 

    this->ferns.clear();

}



void FFClassifier::train( Mat& integralImage,  BoundingBox& patch, int positive)
{
    float post = 0.0f;

    post = classify(integralImage, patch);

    if( (positive && post <= this->threshold) || (!positive && post >= threshold && patch.conf < 0.85f))
    {
        
        for(int i = 0; i < this->ferns.size(); i++)
        {
            this->ferns[i]->train(integralImage, patch, positive);
        }
    }

}

float FFClassifier::classify( Mat& integralImage,  BoundingBox& patch)
{
    float sum = 0.0f;

    for(int i = 0; i < this->ferns.size(); i++)
    {
        sum += this->ferns[i]->classify(integralImage, patch);
    }

    return sum/float(this->ferns.size());
}

bool FFClassifier::filter(Mat& integralImage,  BoundingBox& patch)
{
    float post = classify(integralImage, patch);

    patch.post = post;

    return post >= this->threshold;
}

//FFClassifier2

FFClassifier2::FFClassifier2(int nFerns, int nNodes, float minScale, float maxScale, float threshold)
{
    this->nFerns = nFerns;
    this->nNodes = nNodes;
    this->minScale = minScale;
    this->maxScale = maxScale;
    this->threshold = threshold;

    this->init(nFerns, nNodes, minScale, maxScale);

}

FFClassifier2::~FFClassifier2()
{
    this->release();
}


void FFClassifier2::init(int nFerns, int nNodes, float minScale, float maxScale)
{
    this->ferns.resize(nFerns);

    for(int i = 0; i < this->ferns.size(); i++) this->ferns[i] = new Fern2(nNodes, minScale, maxScale);
}

void FFClassifier2::release()
{
    for(int i = 0; i < this->ferns.size(); i++)
    {
        delete ferns[i];
    } 

    this->ferns.clear();

}



void FFClassifier2::train( unsigned char c1, unsigned char c2, int positive)
{
    float post = 0.0f;

    post = classify(c1, c2);

    for(int i = 0; i < this->ferns.size(); i++)
    {
        this->ferns[i]->train(c1, c2, positive);
    }

}

float FFClassifier2::classify( unsigned char c1, unsigned char c2)
{
    float sum = 0.0f;

    for(int i = 0; i < this->ferns.size(); i++)
    {
        sum += this->ferns[i]->classify(c1, c2);
    }

    return sum/float(this->ferns.size());
}

bool FFClassifier2::filter(unsigned char c1, unsigned char c2)
{
    float post = classify(c1, c2);

    return post >= this->threshold;
}

}
