/* *************************************************** rectangle_fitting.c *** *
 * 楕円当てはめクラス
 * ************************************************************************* */
#include "rectangle_fitting.h"

/*
 * コンストラクタ
 */
RectangleFitting::RectangleFitting() {
  params = Eigen::Vector4d::Zero(4);
  computeError = true;
}
/*
 * デストラクタ
 */
RectangleFitting::~RectangleFitting() {
  ;
}

/*
 * 長方形当てはめ関数
 *
 * @param [in] points : 長方形当てはめに使用する点列
 *
 * @return 長方形のパラメータを正しく推定できたらtrue、そうでなければfalse
 */
bool RectangleFitting::Fit(std::vector<cv::Point> points) {
  if (points.empty()) return false;

  // 点列から最小限の長方形を計算
  auto x_minmax = std::minmax_element(points.begin(), points.end(),
    [](const cv::Point& a, const cv::Point& b) { return a.x < b.x; });
  auto y_minmax = std::minmax_element(points.begin(), points.end(),
    [](const cv::Point& a, const cv::Point& b) { return a.y < b.y; });

  double left = x_minmax.first->x;
  double right = x_minmax.second->x;
  double top = y_minmax.first->y;
  double bottom = y_minmax.second->y;

  params(0) = left;    // x座標
  params(1) = top;     // y座標
  params(2) = right - left; // 幅
  params(3) = bottom - top; // 高さ

  // 誤差計算
  error = 0.0;
  if (computeError) {
    for (const auto& point : points) {
      double dist_x = std::max(point.x - right, left - point.x);
      double dist_y = std::max(point.y - bottom, top - point.y);
      error += std::sqrt(dist_x * dist_x + dist_y * dist_y);
    }
    error /= points.size();
}

// 正しい長方形のパラメータが推定されたかどうかを返す
// この例では、単純にパラメータが正の値であることを確認する
return (params(2) > 0 && params(3) > 0);
}

/* ******************************************** End of rectangle_fitting.c *** */
