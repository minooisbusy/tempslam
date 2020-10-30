#include<iostream>
#include "opencv2/opencv.hpp"
#include "System.h"

using namespace cv;
using namespace std;


int main(int argc, char**argv)
{
    //Declaration and initialization camera
    cv::VideoCapture cap(0);
    if(!cap.isOpened())
        cerr<<"Error: Can't find camera 0"<<endl;

    //Initialize System
    KLT::System kltTracker(argv[1], true);
    

    Mat frame;
    while(true)
    {
    //get serialized image or camera capture
        cap >> frame;
    //Do tracking
        kltTracker.Track(frame);
    }
}
