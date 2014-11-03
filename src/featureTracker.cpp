#include "featureTracker.h"


void vision_toolbox::toGray(cv::Mat& imgColor)
{
	cv::Mat gray;
	cv::cvtColor(imgColor, gray, CV_BGR2GRAY);
	imgColor = gray;
}
void vision_toolbox::draw(cv::Mat img, std::vector<cv::Point2f> corners, std::vector<cv::Point2f> imgpts, cv::Point2f offset){
	   // cv::Point2f dirX = imgpts[0] - corners[0];
	   // cv::Point2f dirY = imgpts[1] - corners[0];
	   // cv::Point2f dirZ = imgpts[2] - corners[0];


	    cv::line( img, corners[0]+offset, offset + corners[1], cv::Scalar(0, 255, 0), 4 );
	    cv::line( img, corners[1]+offset, offset + corners[2] , cv::Scalar( 0, 255, 0), 4 );
	    cv::line( img, corners[2]+offset, offset + corners[3], cv::Scalar( 0, 255, 0), 4 );
	    cv::line( img, corners[3]+offset ,offset + corners[0], cv::Scalar( 0, 255, 0), 4 );


	  /*  cv::Point2f corner = corners[0] + offset;
	    cv::line(img, corner,corner + dirX, cv::Scalar(255,0,0), 5);
	    cv::line(img, corner,corner + dirY, cv::Scalar(255,255,0), 5);
	    cv::line(img, corner,corner + dirZ, cv::Scalar(0,0,255), 5);*/

	}


//Needs to know the feature type, the target object
vision_toolbox::FeatureTracker::FeatureTracker(cv::Mat intrinsic, cv::Mat distcoeffs, int featureType, int minDist, int minHessian, int nFeatures, int nLayers)
{
	this->intrinsic = intrinsic;
	this->distcoeffs = distcoeffs;

	this->maxDist = 0;
	this->minDist = minDist;
  
	if( featureType == SIFT )
	{
		this->detector = new cv::SiftFeatureDetector( nFeatures, nLayers );
		this->extractor = new cv::SiftDescriptorExtractor();
    this->matcher = cv::DescriptorMatcher::create("FlannBased");
    //this->matcher = new cv::FlannBasedMatcher();
	}
	else if( featureType == SURF )
	{
		this->detector = new cv::SurfFeatureDetector( minHessian );
		this->extractor = new cv::SurfDescriptorExtractor();
    this->matcher = cv::DescriptorMatcher::create("FlannBased");
	}
  else if ( featureType == ORB)
  {
    this->detector = new cv::OrbFeatureDetector( 1000 );
    this->extractor = new cv::OrbDescriptorExtractor();
    this->matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
  }
}
//Step #1
void vision_toolbox::FeatureTracker::init(ObjectModel& objModel)
{
	objModel.keypoints.clear();
	detector->detect( objModel.target, objModel.keypoints );

	objModel.descriptors.release();
	extractor->compute( objModel.target, objModel.keypoints, objModel.descriptors );


	std::cout << "nKpInit: " << objModel.keypoints.size() << std::endl;
}

bool vision_toolbox::FeatureTracker::track(ObjectModel& objModel, cv::Mat& frame)
{

  //this->init(objModel);

	std::vector< cv::KeyPoint > keypointsScene;
	cv::Mat descriptorsScene;


	detector->detect( frame, keypointsScene );


	extractor->compute( frame, keypointsScene, descriptorsScene );


    std::vector< cv::DMatch > matches;
    matcher->match( objModel.descriptors, descriptorsScene, matches );

    //Max and Min distance computation
    for( int i = 0; i < objModel.descriptors.rows; i++ )
    {
    	double dist = matches[i].distance;
      	if( dist < minDist ) minDist = dist;
      	if( dist > maxDist ) maxDist = dist;
    }

    //Compute good matches
    std::vector< cv::DMatch > goodMatches;

    for( int i = 0; i < objModel.descriptors.rows; i++ )
    {
    	if( matches[i].distance < 3*minDist )
    	{
    		goodMatches.push_back( matches[i]); 
    	}
    }

    //Debug:
    cv::Mat img_matches;
    drawMatches( objModel.target, objModel.keypoints, frame, keypointsScene,
                 goodMatches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
                 std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    imshow( objModel.label + "Matches", img_matches );


    //Compute Homography
    std::vector<cv::Point2f> obj;
    std::vector<cv::Point2f> scene;

    for( int i = 0; i < goodMatches.size(); i++ )
    {
      obj.push_back( objModel.keypoints[ goodMatches[i].queryIdx ].pt );
      scene.push_back( keypointsScene[ goodMatches[i].trainIdx ].pt );
    }

    //Not enough points to find homography, tracking failed.
    if( scene.size() < 4 ) 
    {
    	//std::cout << "LOL" << std::endl;
    	return false;
    }
    else if( scene.size() > 15 )
    {
      printf("Detected Object: %s keypoints %d\n", objModel.label.c_str(), scene.size());
    }

    cv::Mat H = findHomography( obj, scene, CV_RANSAC );

    objModel.sceneCorners.clear(); 
    objModel.sceneCorners.resize(objModel.modelCorners.size());

    perspectiveTransform( objModel.modelCorners, objModel.sceneCorners, H);

    /*solvePnP(objModel.points3D, objModel.sceneCorners, intrinsic, distcoeffs,
                           objModel.pose.rvec, objModel.pose.tvec);


    std::cout << "CamOri: " <<  objModel.pose.rvec << std::endl;


    std::cout << "CamPos: " <<  objModel.pose.tvec << std::endl;*/

    //objModel.pose.valid???

    return true;

}

#ifdef _TRACKER_DEMO_MAIN_

using namespace cv;
using namespace vision_toolbox;

  /** @function readme */
void readme()
{ std::cout << " Usage: ./featureObjectTracker <cam id> <objModelFile1.txt> ... <objectModelFile_N.txt>" << std::endl; }

int main(int argc, char** argv)
{
  //Variables
  
  Mat intrinsic; 
  Mat distcoeffs;
  vector<Point2f> imgpts;

  FileStorage fs1("mycalib.yml", FileStorage::READ);

  fs1["CM1"] >> intrinsic;
  fs1["D1"] >> distcoeffs;
  fs1.release();


  if( argc < 3 )
  { readme(); return -1; }

  VideoCapture cap(atoi(argv[1]));
  cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
  cap.set(CV_CAP_PROP_FPS,60);

  std::vector<ObjectModel> objModels;
  objModels.resize(argc-2,ObjectModel());

  //printf("Loading %d objects...\n", argc-2);
  for(int i=2; i < argc; i++)
  {
    //printf("loading object %s\n",argv[i]);
    objModels[i-2].load(argv[i]);
    //printf("loaded sucessfully\n");
  }
    


  

  

  
  Mat img_scene;

  FeatureTracker tracker(intrinsic,distcoeffs);
  cv::Mat frame, gray;

  for(int i = 0; i < objModels.size(); i++)
  {
    tracker.init(objModels[i]);
  }
  


  while( cv::waitKey(5) != 27 )
  {
    cap >> frame;
    gray = frame.clone();
    toGray(gray);


    for(int i = 0; i < objModels.size(); i++)
    {
      if( tracker.track(objModels[i], gray) ) 
      {
        //projectPoints(objModel.axis, objModel.pose.rvec, objModel.pose.tvec, intrinsic, distcoeffs, imgpts);

        draw(frame, objModels[i].sceneCorners, imgpts);


      }
    }

    imshow("Frame", frame);

    

  }


}

#endif // _TRACKER_DEMO_MAIN_

