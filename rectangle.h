/* *********************************************************** rectangle.h *** *
 * 長方形クラス(ヘッダファイル)
 * ************************************************************************* */
#pragma once

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

class Rects
{
 public:
  // コンストラクタ
  Rects();
  Rects(const Eigen::Vector4d& _u);

  // デストラクタ
  ~Rects();

  // 長方形パラメータを登録する関数
  void   SetParam		(const Eigen::Vector4d& _u);
  // 点列を登録する関数
  void   SetPoints		(std::vector<cv::Point> _points);
  // 長方形属性を計算する関数
  void   ComputeAttributes	(void);
  // 点(x, y)が長方形の内部にあるかどうか調べる関数
  bool   CheckInner		(double x, double y);

  // メンバ変数
  Eigen::Vector4d params; // 長方形パラメータ (x, y, width, height)
  double cx, cy;          // 長方形の中心座標
  double width, height;   // 長方形の幅と高さ
  std::vector<cv::Point> point_list; // 点列データ
};

/* *************************************************** End of rectangle.h *** */
