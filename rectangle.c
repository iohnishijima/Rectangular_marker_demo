/* *********************************************************** rectangle.c *** *
 * 楕円クラス
 * ************************************************************************* */
#include "rectangle.h"
#include <algorithm>
#include <iterator>

/*
 * コンストラクタ
 */
Rects::Rects() {
  params = Eigen::Vector4d::Zero(4);
  cx = 0.0;
  cy = 0.0;
  width = 0.0;
  height = 0.0;
}

/*
 * コンストラクタ
 *
 * @param [in] _params : 長方形のパラメータ (x, y, width, height)
 */
Rects::Rects(const Eigen::Vector4d& _params) {
  params = _params;
  cx = params(0) + params(2) / 2.0;
  cy = params(1) + params(3) / 2.0;
  width = params(2);
  height = params(3);
}

/*
 * デストラクタ
 */
Rects::~Rects() {
  ;
}

/*
 * 楕円パラメータを登録する関数
 *
 * @param [in] _params : 長方形のパラメータ
 */
void Rects::SetParam(const Eigen::Vector4d& _params) {
  params = _params;
  cx = params(0) + params(2) / 2.0;
  cy = params(1) + params(3) / 2.0;
  width = params(2);
  height = params(3);
}

/*
 * 点列を登録する関数
 *
 * @param [in] _points : 点のリスト
 */
void Rects::SetPoints(std::vector <cv::Point> _points) {
  point_list.clear();
  std::copy(_points.begin(), _points.end(), std::back_inserter(point_list));
}

/*
 * 長方形属性(中心座標, 幅, 高さ)を計算する関数
 */
void Rects::ComputeAttributes(void) {
    // 既にコンストラクタやSetParamsで計算しているため、この関数では特に何もしない
}

/*
 * 点(x, y)が楕円の外部か内部かを判定する関数 
 *
 * @param [in] x, y: 点(x, y)
 *
 * @return 点(x, y)が楕円の内部であればtrue, そうでなければfalse
 */
bool Rects::CheckInner(double x, double y) {
  double left = params(0);
  double top = params(1);
  double right = left + width;
  double bottom = top + height;

  return (x >= left && x <= right && y >= top && y <= bottom);
}

/* **************************************************** End of rectangle.c *** */
