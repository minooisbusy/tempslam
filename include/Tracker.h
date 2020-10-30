#ifndef _TRACKER_H_
#define _TRACKER_H_
#include"opencv2/opencv.hpp"
#include"System.h"
#include"Frame.h"
using namespace std;
namespace KLT 
{
class System;
class Tracker
{
public:
    Tracker(KLT::System* pSys, const std::string &strSettingFile);
    cv::Mat GrabImageMonocular(const cv::Mat &im);
private:
    void Track();
    void FeatureTrack();
    double error(cv::Point2f &x, const Mat &p); // I(x) - J(x;p)


public:
    // Tracking states
    enum eTrackingState{
        SYSTEM_NOT_READY=-1,
        NO_IMAGES_YET=0,
        NOT_INITIALIZED=1,
        OK=2,
        LOST=3
    };
    eTrackingState mState;
    eTrackingState mLastProcessedState;
    Frame mCurrentFrame, mPrevFrame;
private:
    // Window sizes
    Size winSize;
    //Variables
    cv::Mat mImGray;

    // Feature related
    FeatureExtractor* mpFeatureExtractor;

    //Calibration matrix
    cv::Mat mK;
    cv::Mat mDistCoef;

    //Motion Model
    cv::Mat mVelocity;

    //Feature matchinig
    Ptr<DescriptorMatcher> matcher;
    //Color order (true RGB, false BGR, ignored if grayscale)
    bool mbRGB;

    //The first frame
    bool firstFrame;

    // System
    KLT::System* mpSystem;

    //KLT Related;
    Mat WarpParam;

};
}
#endif
