#ifndef DETECTOR_H
#define DETECTOR_H

#include <vector>
#include "includes.h"
#include "boundingbox.h"
#include "ffclassifier.h"
#include "nnclassifier.h"
#include "variancefilter.h"
#include "clusterer.h"
#include "serializable.h"
#include "featureTracker.h"

using namespace std;
using namespace vision_toolbox;

namespace tld {


class Detector : public Serializable
{
    public:

        FFClassifier* ffClassifier;
        NNClassifier* nnClassifier;
        VarianceFilter* varianceFilter;
        FeatureTracker* featureTracker;


        Clusterer* clusterer;

        int frameW, frameH;
        float objW, objH;
        vector<BoundingBox> windows;

        float minScale, maxScale, baseScale;
        float shift;
        int min_bb;


        bool initialized;

        void initWindows();

        Detector() {
            this->ffClassifier = new FFClassifier();
            this->nnClassifier = new NNClassifier();

            this->clusterer = new Clusterer();


            this->varianceFilter = new VarianceFilter();

            this->featureTracker = new FeatureTracker();
        }

        Detector(int frameW, int frameH, FFClassifier* ffClassifier, NNClassifier* nnClassifier = NULL);

        Detector(int frameW, int frameH, FFClassifier* ffClassifier, NNClassifier* nnClassifier = NULL, float minScale = MIN_WINDOW_SCALE, float maxScale = MAX_WINDOW_SCALE, float baseScale = BASE_WINDOW_SCALE,  float shift = SHIFT, int min_bb = MIN_BB, float cutoff = CUTOFF);
        virtual ~Detector();

        void init();
        void release();


        BoundingBox detect(Mat& img, Mat& integralImage, Mat& integralImageSqr, ObjectModel& objModel,bool emergency = false);


        virtual void save(ostream& out) const
        {
            out << frameW << " " << frameH << " " << minScale << " " << maxScale << " " << baseScale << " " << shift << " " << min_bb << " " << objW << " " << objH << endl;
            out << *ffClassifier << endl;
            out << *nnClassifier << endl;
            out << *clusterer << endl;
            out << *varianceFilter << endl;

        }

        virtual void restore(istream& in)
        {

            in >> frameW >> frameH >> minScale >> maxScale >> baseScale >> shift >> min_bb >> objW >> objH;
            in >> *ffClassifier;
            in >> *nnClassifier;
            in >> *clusterer;
            in >> *varianceFilter;

            this->windows.clear();
            this->init();
  
        }

        friend ostream& operator << (ostream& out, const Detector& obj)
        {
            obj.save(out);

            return out;
        }

        friend istream& operator >> (istream& in, Detector& obj)
        {
            obj.restore(in);

            return in;
        }
    protected:
    private:
};

}

#endif // DETECTOR_H
