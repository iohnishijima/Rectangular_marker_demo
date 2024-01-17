/* *************************************************** rectangle_fitting.h *** *
 * 長方形当てはめクラス(ヘッダファイル)
 * ************************************************************************* */
#pragma once

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

class RectangleFitting
{
  // メソッド
 public:
  // コンストラクタ
  RectangleFitting();

  // デストラクタ
  ~RectangleFitting();

  // 楕円パラメータを推定する関数
  bool Fit(std::vector<cv::Point> point);

  // メンバ変数
  Eigen::Vector4d params;            // 長方形パラメータ(x, y, width, height)
  double          error;        // 長方形当てはめの平均誤差
  bool            computeError; // 誤差を計算するかどうか
};

/* ******************************************** End of rectangle_fitting.h *** */
