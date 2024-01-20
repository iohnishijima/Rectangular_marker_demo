//rectangle_detection.h

#ifndef RECTANGLEDETECTION_H
#define RECTANGLEDETECTION_H

#include "rectangle.h"
#include <opencv2/opencv.hpp>
#include <vector>

class RectangleDetection {
public:
    RectangleDetection(int gaussianKernelSize, double gaussianSigma, int lowThreshold, int highThreshold);
    bool Detect(cv::Mat& image, std::vector<std::vector<cv::Point>>& rect_list);

private:
    Rectangle convertToRectangle(const std::vector<cv::Point>& contourPoly);
    double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
    int gaussianKernelSize_;
    double gaussianSigma_;
    int lowThreshold_;
    int highThreshold_;
};

#endif // RECTANGLEDETECTION_H
