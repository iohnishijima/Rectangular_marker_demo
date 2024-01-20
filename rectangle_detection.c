//rectangle_detection.c

#include "rectangle_detection.h"

RectangleDetection::RectangleDetection(int gaussianKernelSize, double gaussianSigma, int lowThreshold, int highThreshold)
: gaussianKernelSize_(gaussianKernelSize), gaussianSigma_(gaussianSigma), lowThreshold_(lowThreshold), highThreshold_(highThreshold) {}

bool RectangleDetection::Detect(cv::Mat& image, std::vector<std::vector<cv::Point>>& rect_list) {
    // cv::Mat gray, edge;
    // cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);
    // cv::GaussianBlur(gray, edge, cv::Size(gaussianKernelSize_, gaussianKernelSize_), gaussianSigma_);
    // cv::Canny(edge, edge, lowThreshold_, highThreshold_);
    
    //2値化を行う
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);

    // 大津の2値化手法を使ってグレースケール画像を2値化します．
    cv::Mat thresholdImg;
    cv::threshold(gray, thresholdImg, 0, 255, cv::THRESH_OTSU);

    // 2値化画像から輪郭を抽出します．
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresholdImg, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);


    for (const auto& contour : contours) {
        std::vector<cv::Point> contour_poly;
        cv::approxPolyDP(contour, contour_poly, 5, true);
        double area = cv::contourArea(contour_poly);

        if (contour_poly.size() == 4 && 2000 < area) {
            double maxCosine = 0;
            for (int j = 2; j < 5; j++) {
                double cosine = std::fabs(angle(contour_poly[j%4], contour_poly[j-2], contour_poly[j-1]));
                maxCosine = std::max(maxCosine, cosine);
            }

            rect_list.push_back(contour_poly);
        }
    }

    for (const auto& rect : rect_list) {
        // cv::rectangle(image, rect.topLeft(), rect.bottomRight(), cv::Scalar(0, 255, 0), 2);
        // ここで四角形の各頂点に従って描画
        const cv::Point* p = &rect[0];
        int n = (int)rect.size();
        cv::polylines(image, &p, &n, 1, true, cv::Scalar(0, 255, 0), 2);
    }

    return !rect_list.empty();
}

Rectangle RectangleDetection::convertToRectangle(const std::vector<cv::Point>& contourPoly) {
    cv::Rect rect = cv::boundingRect(contourPoly);
    return Rectangle(rect.tl(), rect.br());
}

double RectangleDetection::angle(cv::Point pt1, cv::Point pt2, cv::Point pt0) {
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}
