#include "Predator.h"

namespace tld
{



Predator::Predator(string paramsFile)
{
    this->params = new Params(paramsFile);

    this->frameW = this->params->frame_w;
    this->frameH = this->params->frame_h;

    this->tracker = new MedianFlowTracker();
    this->ffClassifier = NULL;
    this->nnClassifier = NULL;
    this->detector = NULL;

    this->initialized = false;

    this->init();

    this->label = "defaultLabel";

}

Predator::Predator(Params* params)
{
    this->params = params;

    this->frameW = this->params->frame_w;
    this->frameH = this->params->frame_h;

    this->tracker = new MedianFlowTracker();
    this->ffClassifier = NULL;
    this->nnClassifier = NULL;
    this->detector = NULL;

    this->initialized = false;

    this->init();

    this->label = "defaultLabel";

}

Predator::~Predator()
{
    delete this->ffClassifier;
    delete this->nnClassifier;
    delete this->detector;
    delete this->tracker;
    delete this->params;
}


void Predator::init()
{
    if( !this->initialized )
    {
        this->ffClassifier = new FFClassifier(this->params->n_ferns, this->params->n_features, this->params->min_scale, this->params->max_scale, this->params->decision_threshold);
        this->nnClassifier = new NNClassifier(this->params->theta_tp, this->params->theta_fp);
        this->detector = new Detector(this->frameW, this->frameH, this->ffClassifier, this->nnClassifier, this->params->min_window_scale, this->params->max_window_scale, this->params->base_window_scale, this->params->shift, this->params->min_bb, this->params->cutoff);


        this->trackerEnabled = true;
        this->detectorEnabled = true;
        this->learningEnabled = true;

        
    }

}

void Predator::release()
{
    if( this->initialized )
    {
        delete this->ffClassifier;
        delete this->nnClassifier;
        delete this->detector;

        this->initialized = false;
    }
}

//Won't work if it was never initialized before
void Predator::reset()
{
    this->release();
    this->tracker->cleanPreviousData();
    this->detectionHistory.clear();
    this->init();
}


void Predator::storeData()
{
    this->prevImg.release();
    this->prevImg = this->currImg.clone();

    this->prevBB = this->currBB;

    this->tracker->cleanPreviousData();

    this->integralImg.release();
}

void Predator::selectObject(Mat& img, BoundingBox& bb)
{

    this->nnClassifier->release();
    this->ffClassifier->release();


    this->detector->objW = bb.w;
    this->detector->objH = bb.h;


    this->ffClassifier->init(this->params->n_ferns, this->params->n_features, this->params->min_scale, this->params->max_scale);
    this->detector->init();



    this->currBB = bb;
    this->currBB.conf = 1.0f;
    this->currBB.valid = true;

    this->detectedBB = BoundingBox();

    cvtColor( img, this->currImg, CV_RGB2GRAY );
    integral( this->currImg, this->integralImg,this->integralImgSqr, CV_64F);



    this->initialLearning();
    this->initialized = true;
    


}

void Predator::processFrame(Mat& img)
{

    this->storeData();


    cvtColor( img, this->currImg, CV_RGB2GRAY );
    integral( this->currImg, this->integralImg,this->integralImgSqr,CV_64F);




    if(trackerEnabled)
    {
        Rect rectPrevBB = this->prevBB;
        //printf("prevBB %d %d %d %d conf %f valid %d\n", prevBB.x, prevBB.y, prevBB.w, prevBB.h, prevBB.conf, prevBB.valid);
        //printf("currBB %d %d %d %d conf %f valid %d\n", currBB.x, currBB.y, currBB.w, currBB.h, currBB.conf, currBB.valid);
        
        this->tracker->track(this->prevImg, this->currImg, &rectPrevBB );



        //imshow("diff", this->prevImg - this->currImg);
        //imshow("curr", this->currImg);


    }



    if(detectorEnabled)
    {

        this->detectedBB = this->detector->detect( this->currImg, this->integralImg, this->integralImgSqr, this->objModel, !this->tracker->trackerBB );
    }


    this->fuseHypotheses();


    this->learn();


}

void Predator::initialLearning()
{
    //this->detectedBB = this->detector->detect(this->currImg, this->integralImg);
    Mat target = get_roi(this->currImg,this->currBB);
    this->objModel.label = this->label;
    this->objModel.load(target);
    this->detector->featureTracker->init(this->objModel);

    NormalizedPatch patch = this->currBB.extractNormalizedPatch(this->currImg);
    patch.positive = 1;

    float initVar = patch.var();//this->detector->varianceFilter->computeVar(this->integralImg,this->integralImgSqr, this->currBB);
    detector->varianceFilter->minVar = initVar/3.0;


    float overlaps[this->detector->windows.size()];
    this->currBB.overlap(this->detector->windows, overlaps);

    //Add all bounding boxes with high overlap

    vector< pair<int,float> > positiveIndices;
    vector<int> negativeIndices;

    //First: Find overlapping positive and negative patches

    for(int i = 0; i < this->detector->windows.size(); i++)
    {

        if(overlaps[i] > 0.6f)
        {
            positiveIndices.push_back(pair<int,float>(i,overlaps[i]));
        }

        if(overlaps[i] < 0.1f)
        {
            negativeIndices.push_back(i);
        }
    }


    sort(positiveIndices.begin(), positiveIndices.end(), tldSortByOverlapDesc);

    vector<NormalizedPatch> patches;

    patches.push_back(patch); //Add first patch to patch list

    int numIterations = std::min<size_t>(positiveIndices.size(), 10); //Take at most 10 bounding boxes (sorted by overlap)
    for(int i = 0; i < numIterations; i++)
    {
        int idx = positiveIndices.at(i).first;
        //Learn this bounding box
        //TODO: Somewhere here image warping might be possible
        this->detector->ffClassifier->train(this->integralImg, this->detector->windows[idx], true);
    }

    srand(time(NULL)); //TODO: This is not guaranteed to affect random_shuffle

    random_shuffle(negativeIndices.begin(), negativeIndices.end());

    //Choose 100 random patches for negative examples
    for(size_t i = 0; i < std::min<size_t>(100,negativeIndices.size()); i++)
    {
        int idx = negativeIndices.at(i);

        NormalizedPatch patch = this->detector->windows[idx].extractNormalizedPatch(this->currImg);
        patch.positive = 0;
        patches.push_back( patch );
    }

    this->detector->nnClassifier->learn(patches);



}

void Predator::learn()
{
    if(!learningEnabled || !this->currBB.valid || !detectorEnabled )
    {
        //printf("NO WE DIDN'T LEARN\n");
        return;
    }




    NormalizedPatch patch = this->currBB.extractNormalizedPatch(this->currImg);
    patch.positive = 1;


    float overlaps[this->detector->windows.size()];
    this->currBB.overlap(this->detector->windows, overlaps);


    //Add all bounding boxes with high overlap
    vector<pair<int,float> > positiveIndices;
    vector<int> negativeIndices;
    vector<int> negativeIndicesForNN;

    //First: Find overlapping positive and negative patches

    for(int i = 0; i < detector->windows.size(); i++)
    {

        if(overlaps[i] > 0.6f )
        {
            positiveIndices.push_back(pair<int,float>(i,overlaps[i]));
        }

        if(overlaps[i] < 0.1f)
        {
            if( this->detector->windows[i].post > 0.2f )   //TODO: Shouldn't this read as 0.5?
            {
                negativeIndices.push_back(i);
            }

            if( this->detector->windows[i].post > 0.5f )
            {
                negativeIndicesForNN.push_back(i);
            }

        }
    }
    //printf("Selected positive and negative patches\n");

    sort(positiveIndices.begin(), positiveIndices.end(), tldSortByOverlapDesc);

    //printf("Sorted by overlap descent\n");

    vector<NormalizedPatch> patches;

    patch.positive = 1;
    patches.push_back(patch);
    //TODO: Flip


    int numIterations = std::min<size_t>(positiveIndices.size(), 10); //Take at most 10 bounding boxes (sorted by overlap)

    for(size_t i = 0; i < negativeIndices.size(); i++)
    {
        int idx = negativeIndices.at(i);
        //TODO: Somewhere here image warping might be possible

        this->detector->ffClassifier->train(this->integralImg, this->detector->windows[idx], false);
    }

    //TODO: Randomization might be a good idea
    for(int i = 0; i < numIterations; i++)
    {
        int idx = positiveIndices.at(i).first;
        //TODO: Somewhere here image warping might be possible
        this->detector->ffClassifier->train(this->integralImg, this->detector->windows[idx], true);
    }

    for(size_t i = 0; i < negativeIndicesForNN.size(); i++)
    {
        int idx = negativeIndicesForNN.at(i);

        NormalizedPatch patch = this->detector->windows[idx].extractNormalizedPatch(this->currImg);
        patch.positive = 0;
        patches.push_back(patch);

    }

    this->detector->nnClassifier->learn(patches);


}

void Predator::fuseHypotheses()
{
    float smax, smin, tmin, tmax, thr, sx, sy;
    smax = 1;
    smin = 0.25;
    tmax = 20;
    tmin = 10;


    Rect* trackerBB = this->tracker->trackerBB;

    //trackedBB.cosine = this->tracker->cosine;
    //trackedBB.magnitude = this->tracker->magnitude;

    //******** CAIU code
    if( trackerBB ) {
        sx = float(trackerBB->width)/detector->objW;
         

        
    }
    thr = 20;//((sx - smin)*(tmax-tmin))/(smax-smin) + tmin;

    this->tracker->fell = fabs(this->tracker->cosine - 1) <= 0.15 && this->tracker->magnitude >= thr;
    //trackedBB.sy = float(trackedBB.h)/detector->objH;

    //trackedBB.fell = this->tracker->fell;

   
    //***********


    //int numClusters = detectorCascade->detectionResult->numClusters;
    //Rect* detectorBB = detectorCascade->detectionResult->detectorBB;


    this->currBB = BoundingBox();
    this->currBB.valid = false;

    if( detectedBB.detected )
    {
        NormalizedPatch patch = detectedBB.extractNormalizedPatch(this->currImg);
        detectedBB.conf = nnClassifier->classify(patch);
        detectedBB.post = this->ffClassifier->classify(this->integralImg, detectedBB);
        //this->detectionHistory.push_back(detectedBB);

        //printf("DETECTION HISTORY SIZE %d\n", this->detectionHistory.size());
    }

    if(trackerBB != NULL)
    {   

        //**** CAIU code
        


        trackedBB = BoundingBox(*trackerBB);
        

        

        NormalizedPatch patch = trackedBB.extractNormalizedPatch(this->currImg);
        trackedBB.conf = nnClassifier->classify(patch);
        trackedBB.post = this->ffClassifier->classify(this->integralImg, trackedBB);
        trackedBB.var = this->detector->varianceFilter->computeVar(this->integralImg,this->integralImgSqr,trackedBB);

        //printf("prevBB %d %d %d %d conf %f valid %d\n", prevBB.x, prevBB.y, prevBB.w, prevBB.h, prevBB.conf, prevBB.valid);
        //printf("detectedBB %d %d %d %d\n", detectedBB.x, detectedBB.y, detectedBB.w, detectedBB.h);
        //printf("trackedBB %d %d %d %d\n", trackedBB.x, trackedBB.y, trackedBB.w, trackedBB.h);
        //printf("tconf %f dconf %d\n", trackedBB.conf, detectedBB.conf);

        if( detectedBB.detected && detectedBB.conf > trackedBB.conf &&  trackedBB.overlap(detectedBB) < 0.5f)
        {

            currBB = detectedBB;


        }
        else
        {

            currBB = trackedBB;

            if( trackedBB.conf >= nnClassifier->thetaTP)
            {

                currBB.valid = true;


            }
            else if( prevBB.valid && trackedBB.conf >= nnClassifier->thetaFP)
            {
                currBB.valid = true;
            }

        }


    }
    else if( detectedBB.detected )
    {

        currBB = detectedBB;
        //currBB.valid = false;
    }

    if( this->tracker->fell )
    {

        printf("cos(%f) norm(%f) thr(%f) fell? %s\n",this->tracker->cosine,this->tracker->magnitude, thr, this->tracker->fell? "YES":"NO");
        this->tracker->magnitude = 0;
        this->tracker->cosine = 0;
        this->tracker->fell = false;
    }
       // if( this->tracker->fell )


    //printf("BB %d %d %d %d conf %f post %f valid %d\n", currBB.x, currBB.y, currBB.w, currBB.h, currBB.conf, currBB.post, currBB.valid);
    //printf("DETECTED BB %d %d %d %d conf %f post %f valid %d\n", detectedBB.x,  detectedBB.y,  detectedBB.w,   detectedBB.h, currBB.post,  detectedBB.conf,  detectedBB.valid);


}
}
