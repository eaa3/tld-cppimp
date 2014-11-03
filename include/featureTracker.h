
#pragma once

#ifndef __FEATURE_TRACKER__
#define __FEATURE_TRACKER__


#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <vector>

namespace vision_toolbox {

	void toGray(cv::Mat& imgColor);


	void draw(cv::Mat img, std::vector<cv::Point2f> corners, std::vector<cv::Point2f> imgpts, cv::Point2f offset = cv::Point2f(0,0));

	//Feature type
	enum {
			SIFT,
			SURF,
			ORB
		};
	
	class Pose {
	public:

		Pose() : tv(3), rv(3), tvec(tv), rvec(rv), valid(false) {

		}

		virtual ~Pose() {}


		std::vector<float> tv, rv;
		cv::Mat tvec, rvec;
		bool valid;

	};

	class ObjectModel
	 {
	public:

		ObjectModel() : inited(0) {}
		ObjectModel(const std::string& filename) : inited(0) {
			
			load(filename);
		 

		}
		ObjectModel(cv::Mat& target, float realW = 1, float realH = 1) : inited(0) {
			
			load(target,realW,realH);
		 

		}

		virtual ~ObjectModel() {}

		void load(const std::string& filename)
		{
			std::ifstream in(filename);

			std::string targetFileName;
			
			in >> targetFileName >> label;
			in >> realW >> realH;
			//printf("loading target image %s\n", targetFileName.c_str());
			this->target = cv::imread( targetFileName, CV_LOAD_IMAGE_GRAYSCALE );
			//printf("loaded target image %s\n", targetFileName.c_str());
			this->modelCorners.clear();
			this->modelCorners.resize(5);
		
		    this->modelCorners[0] = cv::Point2f(0,0); 
		    this->modelCorners[1] = cv::Point2f( this->target.cols, 0 );
		    this->modelCorners[2] = cv::Point2f( this->target.cols, this->target.rows ); 
		    this->modelCorners[3] = cv::Point2f( 0, this->target.rows );
		    this->modelCorners[4] = cv::Point2f(this->target.cols/2, this->target.rows/2);

		    this->axis.push_back(cv::Point3f(realW/2,0,0));
			this->axis.push_back(cv::Point3f(0,realH,0));
			this->axis.push_back(cv::Point3f(0,0,-realH/2));

  
			 this->points3D.push_back(cv::Point3f(0, 0 , 0.0f));
			 this->points3D.push_back(cv::Point3f(realW, 0 , 0.0f));
			 this->points3D.push_back(cv::Point3f(realW, realH , 0.0f));
			 this->points3D.push_back(cv::Point3f(0, realH , 0.0f));
			 this->points3D.push_back(cv::Point3f(realW/2, realH/2 , 0.0f));
			

			in.close();
		}
		//Target is a gray image. realW and realH are the real dimensions of the planar object
		void load(cv::Mat& target, float realW = 1, float realH = 1)
		{
			this->realW = realW;
			this->realH = realH;

			//printf("loading target image %s\n", targetFileName.c_str());
			this->target = target.clone();
		
			//printf("loaded target image %s\n", targetFileName.c_str());
			this->modelCorners.clear();
			this->modelCorners.resize(5);
		
		    this->modelCorners[0] = cv::Point2f(0,0); 
		    this->modelCorners[1] = cv::Point2f( this->target.cols, 0 );
		    this->modelCorners[2] = cv::Point2f( this->target.cols, this->target.rows ); 
		    this->modelCorners[3] = cv::Point2f( 0, this->target.rows );
		    this->modelCorners[4] = cv::Point2f(this->target.cols/2, this->target.rows/2);

		    this->axis.push_back(cv::Point3f(realW/2,0,0));
			this->axis.push_back(cv::Point3f(0,realH,0));
			this->axis.push_back(cv::Point3f(0,0,-realH/2));

  
			 this->points3D.push_back(cv::Point3f(0, 0 , 0.0f));
			 this->points3D.push_back(cv::Point3f(realW, 0 , 0.0f));
			 this->points3D.push_back(cv::Point3f(realW, realH , 0.0f));
			 this->points3D.push_back(cv::Point3f(0, realH , 0.0f));
			 this->points3D.push_back(cv::Point3f(realW/2, realH/2 , 0.0f));

		}



		cv::Mat target; //Target image
		std::vector< cv::Point3f >  points3D; //Points3D of the given object, with z=0
		std::vector< cv::Point2f >  modelCorners; //Initial 2D points corresponding to points3D
		
		//Attributes pre-processed when loaded object
		std::vector< cv::KeyPoint > keypoints;
		cv::Mat descriptors;

		
		std::vector< cv::Point2f >  sceneCorners; //Dynamically detected 2D corresponding to points3D
		std::vector<cv::Point3f> axis;

		Pose pose;

		float realW, realH;
		std::string label;

		bool inited;

	};

	class Tracker {

	public:
		virtual bool track(ObjectModel& objModel, cv::Mat& frame) = 0;
		virtual ~Tracker() {}

	};



	class FeatureTracker : public Tracker {


	public:

		

		//Needs to know the feature type, the target object
		FeatureTracker(cv::Mat intrinsic = cv::Mat(), cv::Mat distcoeffs = cv::Mat(), int featureType = SIFT, int minDist = 50, int minHessian = 600, int nFeatures = 300, int nLayers = 4);

		~FeatureTracker()
		{
			delete this->detector;
			delete this->extractor;
		}



		void init(ObjectModel& objModel);
		bool track(ObjectModel& objModel, cv::Mat& frame);

	private:

		cv::FeatureDetector* detector;
		cv::DescriptorExtractor* extractor;
		cv::Ptr<cv::DescriptorMatcher> matcher;
		cv::Mat intrinsic; 
		cv::Mat distcoeffs;
		float minDist, maxDist;

		/****
			Descriptor matcher type. Now the following matcher types are supported:

			BruteForce (it uses L2 )
			BruteForce-L1
			BruteForce-Hamming
			BruteForce-Hamming(2)
			FlannBased

		*///


	};

}

#endif