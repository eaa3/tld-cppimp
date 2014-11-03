#include "main_skeleton.h"

VideoCapture cap;
Mat img, roi;
BoundingBox box;
bool ended_drawing, drawing, caught_patch, inited;
Predator* tldTracker = NULL;
ofstream out;
bool alt;
int currPred;
vector<Predator*> tldTrackerList;
Params* params = NULL;

int init()
{
    ended_drawing = drawing = caught_patch = inited = false;

    //cap.open("C:\\Users\\Ermano\\Dropbox\\IC-Voxar\\HeadTracker-Demo.avi");
    params = new Params(TLD_CONFIG_FILE);
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
    //namedWindow("roi", 1);

    cap >> img;

    setMouseCallback("TLD-Predator", onMouseCB, &img);


    if( !img.empty() )
    {
        
        tldTrackerList.push_back( new Predator(params) );
        //tldTrackerList[1] = new Predator(params);
        currPred = 0;

        tldTracker = tldTrackerList[currPred];
    }
        

    alt = true;
    return 0;
}

void deinit()
{
    for(int i = 0; i < tldTrackerList.size(); i++)
    {
        tldTracker = tldTrackerList[i];
        
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

    delete params;
    
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

        tldTracker->selectObject(img, box);

        inited = true;
    }

    for(int i = 0; i < tldTrackerList.size(); i++ )
    {
        if( tldTrackerList[i]->initialized )
        {
            tldTrackerList[i]->processFrame(img);
        }
    }
        
    //Track the object using LK method and get the current patch
    //Use sliding window approach to detect patches with the detector

    



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

void draw_box(BoundingBox& b, Mat& src, string label, Scalar color)
{
    Point2i p1(b.x, b.y);
    Point2i p2(b.xf, b.yf);
    rectangle(src, p1, p2, color, 5 );
    

    // then put the text itself
    putText(img, label, p1, FONT_HERSHEY_PLAIN, 2, Scalar::all(255), 3, 8);

    char tracking_quality[255];
    sprintf(tracking_quality,"c: %0.2f p: %0.2f v: %0.2f",b.conf, b.post, b.var);
    putText(img, string(tracking_quality), Point2i(b.x,b.yf+20), FONT_HERSHEY_PLAIN, 1, Scalar(0,255,0), 2, 2);

}

void draw()
{
    

    if( !caught_patch )
    {
        draw_box(box, img,"Selecting Object");
        //printf("Draw a bounding box, then click on it to start tracking\n");
        //printf("You may press R to restart the system\n");
    }

    for(int  i = 0; i < tldTrackerList.size(); i++)
    {
        
        
        if( tldTrackerList[i]->currBB.getArea() > 400  )
        {
            //printf("Fell? %s\n", tldTrackerList[i]->currBB.fell? "FELL!" : "HELD" );
            //printf("BB scaleX(%f) scaleY(%f)\n", tldTrackerList[i]->currBB.sx, tldTrackerList[i]->currBB.sy);
            draw_box(tldTrackerList[i]->currBB,img,tldTrackerList[i]->label, currPred==i? Scalar(255,0,0) : Scalar(255,122,122));
            roi = get_roi(img, BoundingBox(tldTrackerList[i]->currBB), 1);
            imshow(tldTrackerList[i]->label, roi);
        }
    }
        
    
    

    imshow("TLD-Predator",img);
}

void saveToFile(string filename, string ext)
{
    ofstream out(filename+ext);
    tldTracker->label = filename;
    out << *tldTracker;

    out.close();
}
void loadFromFile(string filename, string ext)
{
    ifstream in(filename+ext);

    in >> *tldTracker;
    tldTracker->learningEnabled = false;

    caught_patch = true; inited = true;
}

void handleKeyboard(char key)
{
    string filename;

    if( key == 'r' ) {
            reset();
    }
    else if ( key == 'l' ) 
    {
        cout << "Enter filename: "; cin >> filename;
        loadFromFile(filename);
        cout << " LOADED OBJECT " << endl;
    }
    else if( key == 's' )
    {
        cout << "Enter filename: "; cin >> filename;
        saveToFile(filename);

        cout << " SAVED OBJECT " << endl;
    }
    else if( key == '+' )
    {
        tldTrackerList.push_back( new Predator(params) );
        currPred = tldTrackerList.size()-1;
        tldTracker = tldTrackerList[currPred];

        if( !tldTracker->initialized )
        {
            ended_drawing = drawing = caught_patch = inited = false;
        }
        else
        {
            ended_drawing = drawing = caught_patch = inited = true;   
        }

        
    }
    else if( key == '0' )
    {
        currPred = (currPred+1)%tldTrackerList.size();
        tldTracker = tldTrackerList[currPred];
            
        if( !tldTracker->initialized )
        {
            ended_drawing = drawing = caught_patch = inited = false;
        }
        else
        {
            ended_drawing = drawing = caught_patch = inited = true;   
        }
    }
}
