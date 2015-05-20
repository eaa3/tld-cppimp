#ifndef FOURBITBP_H
#define FOURBITBP_H

#include "feature.h"

namespace tld {

class FourBitBP : public Feature
{
    public:
    	FourBitBP() {}
    	FourBitBP(const FourBitBP& other) : Feature(other) {}
    	FourBitBP(float minScale, float maxScale);
        virtual ~FourBitBP();


        virtual int evaluate(Mat& frame, BoundingBox box);

        virtual int evaluate(unsigned char c1, unsigned char c2);


    protected:
    private:

};


}

#endif // FOURBITBP_H
