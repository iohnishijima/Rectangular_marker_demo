//rectangle.c

#include "rectangle.h"

Rectangle::Rectangle(const cv::Point& topLeft, const cv::Point& bottomRight)
: topLeft_(topLeft), bottomRight_(bottomRight) {}

cv::Point Rectangle::topLeft() const {
    return topLeft_;
}

cv::Point Rectangle::bottomRight() const {
    return bottomRight_;
}
