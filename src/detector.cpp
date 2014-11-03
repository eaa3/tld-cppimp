#include "detector.h"



namespace tld {

Detector::Detector(int frameW, int frameH, FFClassifier* ffClassifier, NNClassifier* nnClassifier, float minScale, float maxScale, float baseScale,  float shift, int min_bb, float cutoff)
{
    this->frameW = frameW;
    this->frameH = frameH;



    this->ffClassifier = ffClassifier;
    this->nnClassifier = nnClassifier;

    this->clusterer = new Clusterer(cutoff);
    this->varianceFilter = new VarianceFilter();
    this->featureTracker = new FeatureTracker();

    this->minScale = minScale;
    this->maxScale = maxScale;
    this->baseScale = baseScale;

    this->shift = shift;
    this->min_bb = min_bb;

}

Detector::~Detector()
{
    this->release();
}


void Detector::release()
{
    delete this->clusterer;

    this->initialized = false;

}

void Detector::init()
{
    this->initWindows();

    this->initialized = true;
}



void Detector::initWindows()
{   


    float currentScale = 0.0f;

    int shiftW = 0;
    float shiftH = 0;

    BoundingBox windowFrame(1,1, frameW - 1, frameH - 1);
    BoundingBox currentBB;

    //Initializing windows and scales
    for(int i = minScale; i <= maxScale; i++ )
    {
        currentScale = powf(baseScale, i);

        currentBB.setWH(this->objW*currentScale, this->objH*currentScale);

        if( currentBB.w < min_bb || currentBB.h < min_bb ) continue;

        //float s1 = (windowFrame.w - currentBB.w)/20.0f;
        //float s2 = (windowFrame.h - currentBB.h)/20.0f;

        //shiftW = max(int(ceil(s1)), 5);
        //shiftH = max(int(ceil(s2)), 5);

        shiftW = shift;//floor(shift*min(currentBB.w, currentBB.h) + 1);
        shiftH = shift;//floor(shift*currentBB.h + 1);



        for(int j = windowFrame.x; (j + currentBB.w) < windowFrame.xf; j += shiftW )
        {
            for(int k = windowFrame.y; (k + currentBB.h) < windowFrame.yf; k += shiftH)
            {

                currentBB.setXY(j,k);

                if( !currentBB.isInside(windowFrame))
                {
                    continue;
                }

                this->windows.push_back(currentBB);


            }
        }

    }

    //printf("Windiws has been generated! sizeWindows %d\n", this->windows.size());





}



BoundingBox Detector::detect(Mat& img, Mat& integralImage, Mat& integralImageSqr, ObjectModel& objModel, bool emergency)
{

    BoundingBox bbRet;

    if( !initialized ) return bbRet;

    vector<BoundingBox*> confidentWindows;

    for(int i = 0; i < this->windows.size(); i++)
    {
        if( !this->varianceFilter->filter(integralImage,integralImageSqr,this->windows[i]) )
        {   
            continue;
        }
        if( !this->ffClassifier->filter(integralImage, this->windows[i]) )
        {
            continue;
        }

        NormalizedPatch npatch = this->windows[i].extractNormalizedPatch(img);

        if( !this->nnClassifier->filter(npatch) )
        {
            continue;
        }


        confidentWindows.push_back(&this->windows[i]);

    }
    bbRet = this->clusterer->cluster(confidentWindows);

    if( !bbRet.detected && emergency  )
    {
        
        //printf("Size sceneCorners: %lu\n ", this->objModel.sceneCorners.size());
        if( this->featureTracker->track(objModel,img) )
        {
            bbRet.x = min(max(0,(int)objModel.sceneCorners[0].x),this->frameW-24);
            bbRet.y = min(max(0,(int)objModel.sceneCorners[0].y),this->frameH-24);



            
            bbRet.h = abs(int(0.75*(objModel.sceneCorners[2].y - objModel.sceneCorners[0].y)));
            bbRet.w = (int)((objW/objH)*bbRet.h*0.75);//abs(int(0.8*(objModel.sceneCorners[2].x - objModel.sceneCorners[0].x)));
            bbRet.xf = min(bbRet.x+bbRet.w+24,this->frameW-24);
            bbRet.yf = min(bbRet.y+bbRet.h+24,this->frameH-24);
            bbRet.w = max(0,bbRet.xf - bbRet.x);
            bbRet.h = max(0,bbRet.yf - bbRet.y);

            printf("Detected by featureTracker %d %d %d %d %d %d\n", bbRet.x, bbRet.y, bbRet.w, bbRet.h, bbRet.xf, bbRet.yf);

            if( bbRet.x < bbRet.xf && bbRet.y < bbRet.yf && bbRet.w > 0 && bbRet.h > 0 ) {
                bbRet.detected = true;
                //bbRet.valid = true;
       
            }
            else
            {
                bbRet.detected = false;
                bbRet.valid = false;
            }

  


        }
    }

    return bbRet;
}


}
