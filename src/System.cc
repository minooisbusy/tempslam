#include "System.h"

namespace KLT
{
System::System(const std::string &strSettingFile, bool visualier)
{

    cv::FileStorage fsSettings(strSettingFile.c_str(), cv::FileStorage::READ);
    if(!fsSettings.isOpened())
    {
        std::cerr<<"System can't open file: "<<strSettingFile<<std::endl;
    }
    mpTracker = new Tracker(this, strSettingFile);
}
void System::Track(const cv::Mat &im)
{

    cv::Mat Tcw = mpTracker->GrabImageMonocular(im);
    
    mTrackingState = mpTracker->mState;
        mTrackingState = mpTracker->mState;
    mTrackedKeyPointsUn = mpTracker->mCurrentFrame.mvKeysUn;

}
}
