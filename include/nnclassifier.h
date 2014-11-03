#ifndef NNCLASSIFIER_H
#define NNCLASSIFIER_H



#include "normalizedpatch.h"
#include "serializable.h"


namespace tld {

class NNClassifier : public Serializable
{
    protected:
    private:


        vector<NormalizedPatch> truePositives;
        vector<NormalizedPatch> falsePositives;


    public:

        float thetaTP, thetaFP;
        NNClassifier(float thetaTP = THETA_TP, float thetaFP = THETA_FP);
        virtual ~NNClassifier();


        float classify( NormalizedPatch& patch);


        void learn( vector<NormalizedPatch>& patches);

        bool filter( NormalizedPatch& patch);


        void release();


        virtual void save(ostream& out) const
        {
            out << thetaTP << " " << thetaFP << " " << truePositives.size() << " " << falsePositives.size() << endl;

            for(int i = 0; i < truePositives.size(); i++)
            {
                out << truePositives[i] << endl;
            }

            for(int i = 0; i < falsePositives.size(); i++)
            {
                out << falsePositives[i] << endl;
            }

            

        }

        virtual void restore(istream& in)
        {
            //this->release();
            int tpSize = 0, fpSize = 0;
            in >> thetaTP >> thetaFP >> tpSize >> fpSize;

            truePositives.resize(tpSize);
            for(int i = 0; i < truePositives.size(); i++)
            {
                in >> truePositives[i];
            }

            falsePositives.resize(fpSize);
            for(int i = 0; i < falsePositives.size(); i++)
            {
                in >> falsePositives[i];
            }

  
        }

        friend ostream& operator << (ostream& out, const NNClassifier& obj)
        {
            obj.save(out);

            return out;
        }

        friend istream& operator >> (istream& in, NNClassifier& obj)
        {
            obj.restore(in);

            return in;
        }


};

}

#endif // NNCLASSIFIER_H
