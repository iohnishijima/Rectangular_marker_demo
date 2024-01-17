/* ************************************************* rectangle_detection.h *** *
 * 楕円検出クラス(ヘッダファイル)
 * ************************************************************************* */
#pragma once

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#define _USE_MATH_DEFINES
#include <math.h>
#include "rectangle_fitting.h"
#include "rectangle.h"

class RectangleDetection
{
  // メソッド
 public:
  // コンストラクタ
  RectangleDetection();

  // デストラクタ
  ~RectangleDetection();

  // 楕円検出
  bool Detect (cv::Mat& input, std::vector<Rects>& rect_list);

  // メンバ変数
  int    minLength;          // エッジ点列の最小点数  
  double cannyParam[2];      // Cannyオペレータのパラメータ
  int    gaussianKernelSize; // ガウシアンフィルタのパラメータ
  double gaussianSigma;
  double aspectRatio;        // 長方形を判定するためのアスペクト比
  double areaThreshold;      // 面積の閾値
  double errorThreshold;
  RectangleFitting rect_fitting;    // 長方形当てはめクラス
  std::vector<Rects> rect_list; // 検出した長方形のリスト
  bool drawRectCenter;            // 描画フラグ

  // デフォルトパラメータ
  const int    DEFAULT_MIN_LENGTH           = 50;
  const int    DEFAULT_GAUSSIAN_KERNEL_SIZE = 5;
  const double DEFAULT_GAUSSIAN_SIGMA       = 1.0;
  const int    DEFAULT_CANNY_PARAM1         = 50;
  const int    DEFAULT_CANNY_PARAM2         = 200;
  const double DEFAULT_AXIS_RATIO           = 0.3;
  const double DEFAULT_AXIS_LENGTH          = 20.0;
  const double DEFAULT_ERROR_THRESHOLD      = 1.4;
};

/* ****************************************** End of rectangle_detection.h *** */
