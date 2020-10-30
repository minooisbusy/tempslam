#ifndef _FRAME_H_
#define _FRMAE_H_
#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "FeatureExtractor.h"
#include <vector>
using namespace cv;
namespace KLT
{

class Frame 
{
public:
    Frame(){};
    Frame(const Mat &im, FeatureExtractor* extractor, Mat &K, cv::Mat &distCoef);
    Frame(const Frame& frame); // copy constructor

    void ExtractFeature(const cv::Mat &im);
    // Intrinsics
    bool mbInitialComputations;
    cv::Mat mK;
    float fx;
    float fy;
    float cx;
    float cy;
    float invfx;
    float invfy;
    Mat mDistCoef;

    // Temp
    //
    cv::Mat img;

    /// KeyPoints
    FeatureExtractor* mpFeatureExtractor;
    int N;// # of keyPoints;
    std::vector<KeyPoint> mvKeys;
    std::vector<KeyPoint> mvKeysUn;
    std::vector<KeyPoint> mvTrackedKeys;

    // Descriptors for valid
    Mat mDescriptors, mTrackedKeyDescriptors;

    // Camera Pose
    Mat mTcw;
    
    // Current and Next Frame id
    long unsigned int nNextId;
    long unsigned int mnId;

    //Undistored Image bound (computed once).
    // NOT declared
    
    void UndistortKeyPoints();
    
    // Rotation, Translation and camera center
    Mat mRcw;
    Mat mtcw;
    Mat Rwc;

};
}
#endif
