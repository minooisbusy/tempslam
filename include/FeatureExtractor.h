#ifndef _FEATURE_H_
#define _FEATURE_H_
#include "opencv2/opencv.hpp"
using namespace cv;
namespace KLT
{
class FeatureExtractor
{
public:
    FeatureExtractor();
    void extract(const cv::Mat &im, std::vector<cv::KeyPoint> &keys, int threshold, bool bNMS);
private:
};
}
#endif
