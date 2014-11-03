#ifndef _VARIANCE_FILTER_H_
#define _VARIANCE_FILTER_H_

#include "includes.h"
#include "boundingbox.h"
#include "serializable.h"

using namespace cv;

namespace tld {

class VarianceFilter : public Serializable {

public:
	bool enabled;

	float minVar;

	VarianceFilter();
	virtual ~VarianceFilter();

	void release();
	bool filter(Mat& ii, Mat& iiSqr, BoundingBox& bb);
	float computeVar(Mat& ii,Mat& iiSqr, BoundingBox& bb);

	virtual void save(ostream& out) const
	{
		out << enabled << " " << minVar << endl;
	}

    virtual void restore(istream& in)
    {
    	in >> enabled >> minVar;
    }

    friend ostream& operator << (ostream& out, const VarianceFilter& obj)
    {
    	obj.save(out);

        return out;
    }

    friend istream& operator >> (istream& in, VarianceFilter& obj)
    {
    	obj.restore(in);

        return in;
    }
};

} /* namespace tld */
#endif /* _VARIANCE_FILTER_H_ */