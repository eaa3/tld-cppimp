#ifndef _MAIN_SKELETON_
#define _MAIN_SKELETON_

#include "tld_util.h"
#include "Predator.h"


using namespace tld;

extern VideoCapture cap;
extern Mat img, roi;
extern BoundingBox box;
extern bool ended_drawing, drawing, caught_patch, inited;
extern Predator* tldTracker;
extern bool alt;
extern int currPred;
extern vector<Predator*> tldTrackerList;


extern ofstream out;

int init();
void deinit();
void reset();
int acquireFrame();
void onMouseCB( int event, int x, int y, int flags, void* param);
void draw_box(BoundingBox& b, Mat& src, string label, Scalar color = Scalar(128) );
void draw();
void process();
void saveToFile(string filename, string ext=".txt");
void loadFromFile(string filename, string ext=".txt");

void handleKeyboard(char key);




#endif
