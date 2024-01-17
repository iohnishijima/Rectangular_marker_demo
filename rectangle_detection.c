/* ************************************************* rectangle_detection.c *** *
 * 楕円検出クラス
 * ************************************************************************* */
#include <opencv2/features2d/features2d.hpp>
#include "rectangle_detection.h"
#include "rectangle_fitting.h"
#include "rectangle.h"
#include <algorithm>
#include <iterator>

/*
 * コンストラク
 */
RectangleDetection::RectangleDetection() {
  minLength          = DEFAULT_MIN_LENGTH;
  gaussianKernelSize = DEFAULT_GAUSSIAN_KERNEL_SIZE;
  gaussianSigma      = DEFAULT_GAUSSIAN_SIGMA;
  cannyParam[0]      = DEFAULT_CANNY_PARAM1;
  cannyParam[1]      = DEFAULT_CANNY_PARAM2;
  aspectRatio        = DEFAULT_ASPECT_RATIO;
  areaThreshold      = DEFAULT_AREA_THRESHOLD;
  errorThreshold     = DEFAULT_ERROR_THRESHOLD;
  drawRectCenter     = false;
  rect_fitting.computeError = true;
}

/*
 * デストラクタ
 */
RectangleDetection::~RectangleDetection() {
  ;
}

/*
 * 長方形検出関数
 *
 * @param [in] image       : 入力画像
 * @param [out] rect_list  : 検出した長方形のリスト
 *
 * @return 長方形が1つ以上検出された場合はtrue, そうでなければfalse
 */
bool RectangleDetection::Detect(cv::Mat& image,
                                std::vector<Rects>& rect_list) {
  // 濃淡画像への変換
  cv::Mat gray;
  cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);

  // 平滑化処理
  cv::Mat edge;
  cv::GaussianBlur(gray, edge,
		   cv::Size(gaussianKernelSize, gaussianKernelSize),
		   gaussianSigma);

  // エッジ検出
  cv::Canny(edge, edge, cannyParam[0], cannyParam[1]);

  // 輪郭線抽出
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(edge, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

  // 条件を満たす長方形が当てはまる点列をリストに追加
  for (const auto& contour : contours) {
    // 点列数が閾値(minLength)より少ない場合は処理しない
    if (contour.size() < minLength) continue;

    // 長方形当てはめ
    Rects rect;
    bool result = rect_fitting.Fit(contour, rect);

    // 条件を満たす長方形をリストに追加
    if (result && rect_fitting.error < errorThreshold) {
      rect.SetPoints(contour);
      rect.ComputeAttributes();

      // 長方形リストに追加する条件
      if (/* 長方形の条件 */) {
        rect_list.push_back(rect);
      }
    }
  }

  // 検出した長方形中心を描画
  if (drawRectCenter) {
    for (const auto& rect : rect_list) {
      cv::Point center;
      center.x = rect.cx;
      center.y = rect.cy;
      cv::circle(image, center, 3, cv::Scalar(0, 255, 0), 1, 1);
    }
  }

  return !rect_list.empty();
}

/* ****************************************** End of rectangle_detection.c *** */
