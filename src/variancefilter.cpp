
/*
 * variancefilter.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: Ermano Arruda, Based on Georg Nebehay implementation of OpenTLD
 */

#include "variancefilter.h"
#include "normalizedpatch.h"


namespace tld {

VarianceFilter::VarianceFilter() {
	enabled = true;
	minVar = 0;

}

VarianceFilter::~VarianceFilter() {
	release();
}

void VarianceFilter::release() {

}

//integral image (ii) and integral image squared (iiSqr)
float VarianceFilter::computeVar(Mat& ii, Mat& iiSqr, BoundingBox& bb) {


	double area = bb.getArea();
	double mX  = bb.getArea(ii) / area; //Sum of Area divided by area
	double mX2 = bb.getArea(iiSqr) / area;
	return (float) (mX2 - mX*mX);
}



bool VarianceFilter::filter(Mat& ii, Mat& iiSqr, BoundingBox& bb) {
	if(!enabled) return true;

	bb.var = computeVar(ii,iiSqr, bb);

	
	//printf("BLOCKED BY VARIANCE! Var: %f initVar: %f\n",bb.var,minVar);
	if(bb.var < minVar) {
		return false;
	}
	return true;
}

} /* namespace tld */
