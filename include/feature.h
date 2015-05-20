#ifndef FEATURE_H
#define FEATURE_H

#include "includes.h"
#include "boundingbox.h"
#include "tld_util.h"
#include "serializable.h"

namespace tld {

class Feature : public Serializable
{
    public:
        float xp, yp, wp, hp;

        Feature();
        Feature(float xp, float yp, float wp, float hp) : xp(xp), yp(yp), wp(wp), hp(hp) {}
        Feature(const Feature& other) : xp(other.xp), yp(other.yp), wp(other.wp), hp(other.hp) {}
        Feature(float minScale, float maxScale);
        virtual ~Feature();




        virtual int evaluate(Mat& integralImage, BoundingBox patch) = 0;


        virtual void save(ostream& out) const
        {
        	out << this->xp << " " << this->yp << " " << this->wp << " " << this->hp;
        }

        virtual void restore(istream& in)
        {
        	in >> this->xp >> this->yp >> this->wp >> this->hp;
        }

        friend ostream& operator << (ostream& out, const Feature& obj)
		{
			obj.save(out);

			return out;
		}

		friend istream& operator >> (istream& in, Feature& obj)
		{
			obj.restore(in);

			return in;
		}



    protected:
    private:
};

}
#endif // FEATURE_H
