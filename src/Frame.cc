#include "Frame.h"

namespace KLT
{
Frame::Frame(const Frame &frame)
    :mpFeatureExtractor(frame.mpFeatureExtractor),
     mK(frame.mK.clone()), mDistCoef(frame.mDistCoef.clone()),
     N(frame.N), mvKeys(frame.mvKeys),
     mvKeysUn(frame.mvKeysUn), //mvTrackedKeys(frame.mvTrackedKeys),
     mDescriptors(frame.mDescriptors.clone()), //mtrackedKeyDescriptors(frame.mtrackedKeyDescriptors.clone()),
     img(frame.img.clone())
{
    /* for(int i=0;i<FRAME_GRID_COLS;i++)
        for(int j=0; j<FRAME_GRID_ROWS; j++)
            mGrid[i][j]=frame.mGrid[i][j]; */

    /* if(!frame.mTcw.empty())
        SetPose(frame.mTcw); */
}
Frame::Frame(const Mat &im, FeatureExtractor* extractor, Mat &K, cv::Mat &distCoef):mDistCoef(distCoef), mK(K), mpFeatureExtractor(extractor), img(im)
{
    mnId++;
    ExtractFeature(im);
    N = mvKeys.size();

    UndistortKeyPoints();

    if(mbInitialComputations)
    {
        //

        fx = K.at<float>(0,0);
        fy = K.at<float>(1,1);
        cx = K.at<float>(0,2);
        cy = K.at<float>(1,2);
        invfx = 1.0f/fx;
        invfy = 1.0f/fy;
        mbInitialComputations = false;
    }

    //Grid Works removed
}

void Frame::ExtractFeature(const cv::Mat &im)
{
    cv::FAST(im,mvKeys,20);
}

void Frame::UndistortKeyPoints()
{
    if(mDistCoef.at<float>(0)==0.0)
    {
        mvKeysUn=mvKeys;
        return;
    }

    // Fill matrix with points
    cv::Mat mat(N,2,CV_32F);
    for(int i=0; i<N; i++)
    {
        mat.at<float>(i,0)=mvKeys[i].pt.x;
        mat.at<float>(i,1)=mvKeys[i].pt.y;
    }

    // Undistort points
    mat=mat.reshape(2);
    cv::undistortPoints(mat,mat,mK,mDistCoef,cv::Mat(),mK);
    mat=mat.reshape(1);

    // Fill undistorted keypoint vector
    mvKeysUn.resize(N);
    for(int i=0; i<N; i++)
    {
        cv::KeyPoint kp = mvKeys[i];
        kp.pt.x=mat.at<float>(i,0);
        kp.pt.y=mat.at<float>(i,1);
        mvKeysUn[i]=kp;
    }
}
}
