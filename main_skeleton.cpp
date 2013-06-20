#include "main_skeleton.h"

VideoCapture cap;
Mat img, roi;
BoundingBox box;
bool ended_drawing, drawing, caught_patch, inited;
Predator* tldTracker = NULL;
ofstream out;
bool alt;

int init()
{
    ended_drawing = drawing = caught_patch = inited = false;

    //cap.open("C:\\Users\\Ermano\\Dropbox\\IC-Voxar\\HeadTracker-Demo.avi");
    Params* params = new Params(TLD_CONFIG_FILE);
    cap.open(0);

    cap.set(CV_CAP_PROP_FRAME_WIDTH, params->frame_w);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, params->frame_h);

    cap.set(CV_CAP_PROP_FPS, params->fps);

    if( !cap.isOpened() )
    {
        printf("VideoCapture couldn't open the camera resource!\n");
        return 1;
    }

    namedWindow("TLD-Predator", 1);
    namedWindow("roi", 1);

    cap >> img;

    setMouseCallback("TLD-Predator", onMouseCB, &img);


    if( !img.empty() )
        tldTracker = new Predator(params);

    alt = true;
    return 0;
}

void deinit()
{
    out.open("posteriors.txt");
    for(int i = 0; i < tldTracker->detector->windows.size(); i++ )
        out << tldTracker->detector->windows[i].post << endl;
    out.close();

    out.open("detectionHistory.txt");
    for(int i = 0; i < tldTracker->detectionHistory.size(); i++)
        out << i << " " << tldTracker->detectionHistory[i].x << " " << tldTracker->detectionHistory[i].y << " " << tldTracker->detectionHistory[i].w << " " << tldTracker->detectionHistory[i].h  << " " << tldTracker->detectionHistory[i].conf << " " << tldTracker->detectionHistory[i].post << endl;
    out.close();

    out.open("confidenceHistory.txt");
    for(int i = 0; i < tldTracker->detectionHistory.size(); i++)
        out << tldTracker->detectionHistory[i].conf <<  endl;
    out.close();

    out.open("posteriorHistory.txt");
    for(int i = 0; i < tldTracker->detectionHistory.size(); i++)
        out << tldTracker->detectionHistory[i].post << endl;
    out.close();

    delete tldTracker;
}

void reset()
{
    ended_drawing = drawing = caught_patch = inited = false;
    alt = true;
    tldTracker->reset();
    box = BoundingBox();
}

int acquireFrame()
{
    cap >> img;

    if( img.empty() )
    {
        printf("Couldn't retrieve image from camera.");
        return 1;
    }

    resize(img, img, Size(tldTracker->frameW, tldTracker->frameH));

    if( ended_drawing && !caught_patch )
    {
        roi = get_roi(img, box, 5);
    }

    return 0;

}

void process()
{
    //Process each frame

    if( caught_patch && !inited )
    {
        //Train the initial detector on the captured initial patch
        //Initialize tracker
//        tracker->trackerBB = new Rect(box);
        tldTracker->selectObject(img, box);


        //tldTracker->processFrame(img);






        inited = true;
    }
    else if( inited )
    {

        tldTracker->processFrame(img);
        box = tldTracker->currBB;


        //Track the object using LK method and get the current patch
        //Use sliding window approach to detect patches with the detector

    }



}

void onMouseCB( int event, int x, int y, int flags, void* param)
{
    Mat temp;
    Range rowRange, colRange;

    switch( event )
    {
    case CV_EVENT_MBUTTONDBLCLK:

        //First patch was selected
        //printf("First patch was selected!\n");

        //Some processing

        break;
    case CV_EVENT_MOUSEMOVE:
        printf("mouse is moving! (%d,%d) box( %d, %d, %d, %d)\n", x,y, box.x, box.y, box.w, box.h);

        if( drawing && !caught_patch )
        {
            box.xf = x;
            box.yf = y;
            box.w = abs(box.xf - box.x);
            box.h = abs(box.yf - box.y);

        }

        break;
    case CV_EVENT_LBUTTONDOWN:

        printf("mouse left button down!\n");

        if( ended_drawing && x >= box.x && x <= box.xf && y >= box.y && y <= box.yf && !caught_patch )
        {
            printf("Patch was selected!\n");
            caught_patch = true;

        }
        else if( !drawing && !caught_patch )
        {
            //Updating box's limits
            box.x = box.xf = x;
            box.y = box.yf = y;
            box.w = box.h = 0;
            ended_drawing = false;
            drawing = true;
        }
        break;
    case CV_EVENT_LBUTTONUP:
        printf("mouse left button up!\n");


        ended_drawing = true;
        drawing = false;


        break;
    default:
        printf("another event\n");
        break;
    }


}

void draw_box(BoundingBox& b, Mat& src, Scalar color)
{
    Point2i p1(b.x, b.y);
    Point2i p2(b.xf, b.yf);
    rectangle(src, p1, p2, color, 5 );
}

void draw()
{
    draw_box(box,img);

    if( !caught_patch )
    {
        printf("Draw a bounding box, then click on it to start tracking\n");
        printf("You may press R to restart the system\n");
    }
    if( box.valid )
    {
        roi = get_roi(img, box, 5);
        imshow("roi", roi);


    }

    imshow("TLD-Predator",img);
}
