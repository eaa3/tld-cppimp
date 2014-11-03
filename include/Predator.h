#ifndef PREDATOR_H
#define PREDATOR_H


#include "MedianFlowTracker.h"
#include "detector.h"
#include "tld_util.h"
#include "params.h"
#include "serializable.h"
#include "featureTracker.h"


using namespace vision_toolbox;


namespace tld
{

class Predator : public Serializable
{

protected:

    Params* params;
public:

    string label;
    ObjectModel objModel;

    MedianFlowTracker* tracker;
    FFClassifier* ffClassifier;
    NNClassifier* nnClassifier;
    Detector* detector;

    Mat currImg;
    Mat prevImg;
    Mat integralImg;
    Mat integralImgSqr;

    BoundingBox prevBB;
    BoundingBox currBB;

    BoundingBox detectedBB;
    BoundingBox trackedBB;

    void storeData(); //done
    void fuseHypotheses();
    void learn();
    void initialLearning();


    bool initialized;

    int frameW, frameH;

public:

    bool trackerEnabled;
    bool detectorEnabled;
    bool learningEnabled;

    vector<BoundingBox> detectionHistory;


    Predator() {
        label = "defaultLabel";
        
    }
    Predator(string paramsFile); //done
    Predator(Params* params); //done
    virtual ~Predator(); //done

    void init(); //done
    void release(); // done
    void reset(); // done

    void selectObject(Mat& img, BoundingBox& bb); //done
    void processFrame(Mat& img); // done

    virtual void save(ostream& out) const
    {
        out << label << endl;
        out << frameW << " " << frameH << " " << initialized << " " << trackerEnabled << " " << detectorEnabled << " " << learningEnabled << endl;
        out << *this->detector << endl;
        cv::imwrite(this->label+".jpg",objModel.target);
  
    }

    virtual void restore(istream& in)
    {
        //this->reset();

        this->detector = new Detector();
        this->ffClassifier = detector->ffClassifier;
        this->nnClassifier = detector->nnClassifier;

        in >> label;
        in >> frameW >> frameH >> initialized >> trackerEnabled >> detectorEnabled >> learningEnabled;
        in >> *this->detector;

        cv::Mat target = cv::imread(this->label+".jpg", 0);
        //std::cout << "Restoring target: " << this->label+".jpg" << std::endl; 
        objModel.label = this->label;
        objModel.load(target);
        this->detector->featureTracker->init(this->objModel);
        //MedianFlowTracker* tracker;

        

  
    }

    friend ostream& operator << (ostream& out, const Predator& obj)
    {
        obj.save(out);

        return out;
    }

    friend istream& operator >> (istream& in, Predator& obj)
    {
        obj.restore(in);

        return in;
    }


};

}

#endif // PREDATOR_H
