#ifndef NORMALIZEDPATCH_H
#define NORMALIZEDPATCH_H

#include "boundingbox.h"
#include "serializable.h"

namespace tld {

class BoundingBox;

class NormalizedPatch : public Serializable
{
    public:

        float values[NORMALIZED_PATCH_DIM*NORMALIZED_PATCH_DIM];
        bool positive;



        NormalizedPatch( const NormalizedPatch& other);
        NormalizedPatch( Mat& image,  BoundingBox& bb, bool positive = false);
        NormalizedPatch();

        void release();
        virtual ~NormalizedPatch();

        float ncc( NormalizedPatch& other);
        float var();
        float mean();


        static void extractNormalizedValues( Mat& image,  BoundingBox& bb, float* out_values);


        virtual void save(ostream& out) const
        {
            out << positive << endl;
            int size = NORMALIZED_PATCH_DIM*NORMALIZED_PATCH_DIM;

            for(int i = 0; i < size; i++)
            {
                out << values[i] << " ";
            }
            out << endl;

        }

        virtual void restore(istream& in)
        {
  
            int size = NORMALIZED_PATCH_DIM*NORMALIZED_PATCH_DIM;
            
            in >> positive;

            for(int i = 0; i < size; i++)
            {
                in >> values[i];
            }

  
        }

        friend ostream& operator << (ostream& out, const NormalizedPatch& obj)
        {
            obj.save(out);

            return out;
        }

        friend istream& operator >> (istream& in, NormalizedPatch& obj)
        {
            obj.restore(in);

            return in;
        }


    protected:

};

}

#endif // NORMALIZEDPATCH_H
