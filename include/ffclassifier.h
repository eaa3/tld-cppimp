#ifndef FFCLASSIFIER_H
#define FFCLASSIFIER_H


#include "fern.h"
#include "serializable.h"


namespace tld {

const float DEFAULT_THRESHOLD = 0.5f;

//Fern Forest Classifier
class FFClassifier : public Serializable
{
    public:

        vector<Fern*> ferns;
        float threshold;
        int nFerns, nNodes;
        float minScale, maxScale;

        FFClassifier() {}
        FFClassifier(const FFClassifier& other)
        {
            this->threshold = other.threshold;
            this->nFerns = other.nFerns;
            this->nNodes = other.nNodes;
            this->minScale = other.minScale;
            this->maxScale = other.maxScale;

            this->ferns.resize(other.ferns.size(),0);

            for(int i = 0; i < this->ferns.size(); i++)
            {
                this->ferns[i] = new Fern(*other.ferns[i]);
            }
        }

        FFClassifier(int nFerns, int nNodes, float minScale, float maxScale, float threshold = DEFAULT_THRESHOLD);
        virtual ~FFClassifier();


        void train( Mat& integralImage, BoundingBox& patch, int positive);

        float classify( Mat& integralImage,  BoundingBox& patch);

        bool filter(Mat& integralImage,  BoundingBox& patch);

        void init(int nFerns = 10, int nNodes = 5, float minScale = 0.1f, float maxScale = 0.5f);
        void release();


        virtual void save(ostream& out) const
        {
            out << nFerns << " " << nNodes << " " << minScale << " " << maxScale << " " << threshold << endl;

            for(int i = 0; i < nFerns; i++)
            {
                out << (*ferns[i]) << endl;
            }

        }

        virtual void restore(istream& in)
        {
            this->release();

            in >> nFerns >> nNodes >> minScale >> maxScale >> threshold;

            if( this->ferns.size() == 0 )
            {
                this->ferns.resize(nFerns);
                for(int i = 0; i < this->ferns.size(); i++)
                {
                    this->ferns[i] = new Fern();
                }
            }

            for(int i = 0; i < nFerns; i++)
            {
                
                in>> *ferns[i];
            }
  
        }

        friend ostream& operator << (ostream& out, const FFClassifier& obj)
        {
            obj.save(out);

            return out;
        }

        friend istream& operator >> (istream& in, FFClassifier& obj)
        {
            obj.restore(in);

            return in;
        }


    protected:
    private:
};

//Fern Forest Classifier
class FFClassifier2 : public Serializable
{
    public:

        vector<Fern2*> ferns;
        float threshold;
        int nFerns, nNodes;
        float minScale, maxScale;

        FFClassifier2() {}
        FFClassifier2(const FFClassifier2& other)
        {
            this->threshold = other.threshold;
            this->nFerns = other.nFerns;
            this->nNodes = other.nNodes;
            this->minScale = other.minScale;
            this->maxScale = other.maxScale;

            this->ferns.resize(other.ferns.size(),0);

            for(int i = 0; i < this->ferns.size(); i++)
            {
                this->ferns[i] = new Fern2(*other.ferns[i]);
            }
        }

        FFClassifier2(int nFerns, int nNodes, float minScale, float maxScale, float threshold = DEFAULT_THRESHOLD);
        virtual ~FFClassifier2();


        void train( unsigned char c1, unsigned char c2, int positive);

        float classify(unsigned char c1, unsigned char c2);

        bool filter(unsigned char c1, unsigned char c2);

        void init(int nFerns = 10, int nNodes = 5, float minScale = 0.1f, float maxScale = 0.5f);
        void release();


        virtual void save(ostream& out) const
        {
            out << nFerns << " " << nNodes << " " << minScale << " " << maxScale << " " << threshold << endl;

            for(int i = 0; i < nFerns; i++)
            {
                out << (*ferns[i]) << endl;
            }

        }

        virtual void restore(istream& in)
        {
            this->release();

            in >> nFerns >> nNodes >> minScale >> maxScale >> threshold;

            if( this->ferns.size() == 0 )
            {
                this->ferns.resize(nFerns);
                for(int i = 0; i < this->ferns.size(); i++)
                {
                    this->ferns[i] = new Fern2();
                }
            }

            for(int i = 0; i < nFerns; i++)
            {
                
                in>> *ferns[i];
            }
  
        }

        friend ostream& operator << (ostream& out, const FFClassifier2& obj)
        {
            obj.save(out);

            return out;
        }

        friend istream& operator >> (istream& in, FFClassifier2& obj)
        {
            obj.restore(in);

            return in;
        }


    protected:
    private:
};


}
#endif // FFCLASSIFIER_H
