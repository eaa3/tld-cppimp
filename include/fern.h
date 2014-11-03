#ifndef FERN_H
#define FERN_H

#include "includes.h"
#include "twobitbp.h"
#include "serializable.h"


namespace tld {

class Fern : public Serializable
{
    protected:
    private:
    public:
        vector<TwoBitBP*> node;

        //Number of leafs of this fern
        int nLeafs;

        vector<int> p;
        vector<int> n;

        vector<float> posterior;


        Fern()
        {
            this->nLeafs = 0;

            
        }

        Fern(const Fern& other)
        {
            this->node.resize(other.node.size(),0);
            for(int i = 0; i < this->node.size(); i++)
            {
                this->node[i] = new TwoBitBP(*other.node[i]);
            }

            this->nLeafs = other.nLeafs;

            this->p.resize(other.p.size(),0);
            this->n.resize(other.n.size(),0);
            this->posterior.resize(other.posterior.size(),0);
            for(int i = 0; i < this->p.size(); i++)
            {
                this->p[i] = other.p[i];
            }
            for(int i = 0; i < this->n.size(); i++)
            {
                this->n[i] = other.n[i];
            }
            for(int i = 0; i < this->posterior.size(); i++)
            {
                this->posterior[i] = other.posterior[i];
            }

        }
        Fern(int nNodes, float minScale, float maxScale);
        virtual ~Fern();
        void release();


        int leafIndex(Mat& integralImage, BoundingBox patch );

        void train( Mat& integralImage, BoundingBox& patch, int patchLabel);

        float classify( Mat& integralImage,  BoundingBox& patch);

        virtual void save(ostream& out) const
        {
            out << nLeafs << " " << node.size() << endl;
            for(int i = 0; i < node.size(); i++ )
            {
                out << (*node[i]) << endl;
            }
 

            for(int i = 0; i < nLeafs; i++ )
            {
                out << p[i] << " ";
            }
            out << endl;

            for(int i = 0; i < nLeafs; i++ )
            {
                out << n[i] << " ";
            }
            out << endl;

            for(int i = 0; i < nLeafs; i++ )
            {
                out << posterior[i] << " ";
            }
            out << endl;


        }

        virtual void restore(istream& in)
        {
            this->release();


            int nNodes = 0;
            in >> nLeafs >> nNodes;


            this->p.resize(nLeafs,0);
            this->n.resize(nLeafs,0);
            this->posterior.resize(nLeafs,0);
            
            if( node.size() == 0 )
            {
                node.resize(nNodes, 0);
                for(int i = 0; i < nNodes; i++) this->node[i] = new TwoBitBP();
            }
            


            for(int i = 0; i < node.size(); i++ )
            {
                in >> *node[i];
            }
       

            for(int i = 0; i < nLeafs; i++ )
            {
                in >> p[i];
            }
 

            for(int i = 0; i < nLeafs; i++ )
            {
                in >> n[i];
            }

            for(int i = 0; i < nLeafs; i++ )
            {
                in >> posterior[i];
            }

   
  
        }

        friend ostream& operator << (ostream& out, const Fern& obj)
        {
            obj.save(out);

            return out;
        }

        friend istream& operator >> (istream& in, Fern& obj)
        {
            obj.restore(in);

            return in;
        }

};

}

#endif // FERN_H
