/* **************************************************** rectangular_marker.h *** *
 *長方形マーカークラス(ヘッダファイル)
 * ************************************************************************* */
#pragma once

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include "rectangle.h"

class RectangularMarker
{
 public:
  // コンストラクタ
  RectangularMarker();
  RectangularMarker(const Rects& _rectOuter, const Rects& _rectInner, int _position);

  // デストラクタ
  ~RectangularMarker();

  // カメラの位置姿勢を計算する関数
  void ComputeCameraParam(void);
  
  // メンバ変数
  Rects rectOuter;   // 外側の長方形パラメータ
  Rects rectInner;   // 内側の長方形パラメータ
  int   position;    // マーカーの配置（水平:0, 垂直:1）
  Eigen::MatrixXd R; // カメラの回転行列
  Eigen::VectorXd T; // カメラの併進ベクトル
  float           M[16]; // モデルビュー行列（OpenGLで使用）
};

/* ********************************************* End of rectangular_marker.h *** */