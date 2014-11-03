#ifndef CLUSTERER_H
#define CLUSTERER_H

#include "boundingbox.h"
#include "disjointset.h"

#include "serializable.h"


namespace tld {



struct Distance
{
    int i, j;
    float dst;

    Distance()
    {
        i = j = -1;
        dst = 0.0f;
    }

};

bool comp(Distance a, Distance b);


class Clusterer : public Serializable
{
    private:

        void calcDistances( vector<BoundingBox*>& confidentWindows, vector<Distance>& distances);
        BoundingBox calcMeanBB( vector<BoundingBox*>& confidentWindows);
        void cluster(vector<Distance>& distances, DisjointSet& djSet);

        void release();

    public:
        float cutoff;

        
        Clusterer(const Clusterer& other) {
            cutoff = other.cutoff;
        }

        Clusterer(float cutoff = CUTOFF);
        virtual ~Clusterer();

        BoundingBox cluster( vector<BoundingBox*>& confidentWindows);

        virtual void save(ostream& out) const
        {
            out << this->cutoff << endl;
        }

        virtual void restore(istream& in)
        {
            in >> cutoff;
        }

        friend ostream& operator << (ostream& out, const Clusterer& obj)
        {
            obj.save(out);

            return out;
        }

        friend istream& operator >> (istream& in, Clusterer& obj)
        {
            obj.restore(in);

            return in;
        }


};

}

#endif // CLUSTERER_H
