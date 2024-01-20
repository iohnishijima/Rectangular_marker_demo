//rectangle.h

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <opencv2/opencv.hpp>

class Rectangle {
public:
    Rectangle(const cv::Point& topLeft, const cv::Point& bottomRight);
    cv::Point topLeft() const;
    cv::Point bottomRight() const;

private:
    cv::Point topLeft_;
    cv::Point bottomRight_;
};

#endif // RECTANGLE_H
