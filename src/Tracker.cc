#include"Tracker.h"
namespace KLT
{
Tracker::Tracker(KLT::System *pSys, const std::string &strSettingFile):mState(NO_IMAGES_YET), mpSystem(pSys), firstFrame(true), winSize(Size(21,21))
{
        cv::FileStorage fSettings(strSettingFile, cv::FileStorage::READ);
        float fx = fSettings["Camera.fx"];
        float fy = fSettings["Camera.fy"];
        float cx = fSettings["Camera.cx"];
        float cy = fSettings["Camera.cy"];
        
        cv::Mat K = cv::Mat::eye(3,3,CV_32F);
        K.at<float>(0,0) = fx;
        K.at<float>(1,1) = fy;
        K.at<float>(0,2) = cx;
        K.at<float>(1,2) = cy;
        K.copyTo(mK);

        cv::Mat DistCoef(4,1,CV_32F);
        DistCoef.at<float>(0) = fSettings["Camera.k1"];
        DistCoef.at<float>(1) = fSettings["Camera.k2"];
        DistCoef.at<float>(2) = fSettings["Camera.p1"];
        DistCoef.at<float>(3) = fSettings["Camera.p2"];
        const float k3 = fSettings["Camera.k3"];
        if(k3!=0)
        {
            DistCoef.resize(5);
            DistCoef.at<float>(4) = k3;
        }
        DistCoef.copyTo(mDistCoef);

        float fps = fSettings["Camera.fps"];
        if(fps==0)
            fps=30;

        cout << endl << "Camera Parameters: " << endl;
        cout << "- fx: " << fx << endl;
        cout << "- fy: " << fy << endl;
        cout << "- cx: " << cx << endl;
        cout << "- cy: " << cy << endl;
        cout << "- k1: " << DistCoef.at<float>(0) << endl;
        cout << "- k2: " << DistCoef.at<float>(1) << endl;
        if(DistCoef.rows==5)
            cout << "- k3: " << DistCoef.at<float>(4) << endl;
        cout << "- p1: " << DistCoef.at<float>(2) << endl;
        cout << "- p2: " << DistCoef.at<float>(3) << endl;
        cout << "- fps: " << fps << endl;

        int nRGB = fSettings["Camera.RGB"];
        mbRGB = nRGB;
        
        if(mbRGB)
            cout << "- color order: RGB (ignored if grayscale)" << endl;
        else
            cout << "- color order: BGR (ignored if grayscale)" << endl;

        // Load feature extraction parameters

         int nFeatures = fSettings["ORBextractor.nFeatures"];
        float fScaleFactor = fSettings["ORBextractor.scaleFactor"];
        int nLevels = fSettings["ORBextractor.nLevels"];
        int fIniThFAST = fSettings["ORBextractor.iniThFAST"];
        int fMinThFAST = fSettings["ORBextractor.minThFAST"]; 
        mpFeatureExtractor = new FeatureExtractor();
}
cv::Mat Tracker::GrabImageMonocular(const cv::Mat &im)
{
    mImGray = im; // Shallow copy??
    if(im.channels() == 3)
    {
        if(mbRGB)
            cvtColor(mImGray,mImGray,cv::COLOR_RGB2GRAY);
        else
            cvtColor(mImGray,mImGray,cv::COLOR_BGR2GRAY);
    }
    else if(mImGray.channels()==4)
    {
        if(mbRGB)
            cvtColor(mImGray,mImGray,cv::COLOR_RGBA2GRAY);
        else
            cvtColor(mImGray,mImGray,cv::COLOR_BGRA2GRAY);
    }
    mCurrentFrame = Frame(mImGray,mpFeatureExtractor,mK,mDistCoef);
    Track();

    return Mat();
}
void Tracker::Track()
{
    if(firstFrame)
    {
        float data[] = {1, 0, 0, 0, 1, 0};
        WarpParam = cv::Mat(2,3,CV_32F,&data);
        std::cout<<WarpParam<<std::endl;
        mPrevFrame = Frame(mCurrentFrame);
    }
    if(mState == NO_IMAGES_YET)
    {
        mState = NOT_INITIALIZED;
    }
    FeatureTrack();

    mPrevFrame = Frame(mCurrentFrame);
}

void Tracker::FeatureTrack()
{
    /*
    const int scale = 1;
    const int delta = 0;
    const int ddepth = CV_8U;
    const int ksize = 1;
    cv::Mat Ix, Iy, Ixx, Iyy, Ixy;
    
    cv::Sobel(mImGray, Ix, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
    cv::Sobel(mImGray, Iy, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
    cv::Sobel(mImGray, Ixx, ddepth, 2, 0, ksize, scale, delta, BORDER_DEFAULT);
    cv::Sobel(mImGray, Iyy, ddepth, 0, 2, ksize, scale, delta, BORDER_DEFAULT);
    cv::Sobel(mImGray, Ixy, ddepth, 1, 1, ksize, scale, delta, BORDER_DEFAULT);
    */
    //std::vector<cv::Point2f> goodFeatures;
    
    vector<KeyPoint> srcPts = mPrevFrame.mvKeys;
    Mat I;
    cv::copyMakeBorder(mCurrentFrame.img, I, winSize.width/2,winSize.width/2,winSize.width/2, winSize.width/2, BORDER_REFLECT_101+BORDER_ISOLATED);
    Mat J = mCurrentFrame.img.clone();
    cv::cvtColor(J, J, COLOR_GRAY2BGR);

    //Draw Features with Rectangle blue then,
    for(int i=0; i<mPrevFrame.N; i++)
    {

        Point2i pt = srcPts[i].pt;
        int sr = pt.x + winSize.width/2;
        int er =  pt.x +3*winSize.width/2;
        int sc = pt.y + winSize.width/2;
        int ec =  pt.y +3*winSize.width/2;
        cv::rectangle(J, Point(sr,sc), Point(er,ec), Scalar(255,0,0));
    }
        imshow("image", J);
        waitKey(1);
    
    for(int i=0; i<mPrevFrame.N; i++)
    {
        Point2i pt = srcPts[i].pt;
        int sr = pt.x + winSize.width/2;
        int er =  pt.x +3*winSize.width/2;
        int sc = pt.y + winSize.width/2;
        int ec =  pt.y +3*winSize.width/2;
        cv::rectangle(J, Point(sr,sc), Point(er,ec), Scalar(0,0,255));
        //Make border
        I = I.rowRange(sc,ec).colRange(sr,er);
        imshow("patch", I);
        imshow("image", J);
        waitKey(0);

    }

    
}
}
