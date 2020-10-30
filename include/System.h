#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#include "opencv2/opencv.hpp"
#include "Tracker.h"
#include<string>
using namespace cv;
namespace KLT
{
class Tracker;
class System 
{
public:
    System(const std::string &strSettingFile, bool visualier);
    void Track(const Mat &im);
private:
    Tracker* mpTracker;
    std::vector<cv::KeyPoint> mTrackedKeyPointsUn;
    int mTrackingState;

    
};
}
#endif
