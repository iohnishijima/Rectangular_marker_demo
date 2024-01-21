//rectangle_detection.h

#ifndef RECTANGLEDETECTION_H
#define RECTANGLEDETECTION_H

#include <opencv2/opencv.hpp>
#include <vector>

class RectangleDetection {
public:
    RectangleDetection(int gaussianKernelSize, double gaussianSigma, int lowThreshold, int highThreshold);
    bool Detect(cv::Mat& image, std::vector<std::vector<cv::Point>>& rect_list);

private:
    int gaussianKernelSize_;
    double gaussianSigma_;
    int lowThreshold_;
    int highThreshold_;
};

#endif // RECTANGLEDETECTION_H
