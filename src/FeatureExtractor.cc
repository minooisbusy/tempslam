#include "FeatureExtractor.h"
namespace KLT
{
FeatureExtractor::FeatureExtractor(){}
void FeatureExtractor::extract(const cv::Mat &im, std::vector<cv::KeyPoint> &keys, int threshold, bool bNMS)
{
    cv::FAST(im, keys, threshold, bNMS);
}
}
