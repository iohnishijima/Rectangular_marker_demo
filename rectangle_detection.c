//rectangle_detection.c

#include "rectangle_detection.h"

RectangleDetection::RectangleDetection(int gaussianKernelSize, double gaussianSigma, int lowThreshold, int highThreshold)
: gaussianKernelSize_(gaussianKernelSize), gaussianSigma_(gaussianSigma), lowThreshold_(lowThreshold), highThreshold_(highThreshold) {}

bool CompareImages(const cv::Mat& img1, const cv::Mat& img2) {
    // ORB検出器を初期化
    cv::Ptr<cv::ORB> detector = cv::ORB::create();
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;

    // 特徴点と記述子を検出
    detector->detectAndCompute(img1, cv::Mat(), keypoints1, descriptors1);
    detector->detectAndCompute(img2, cv::Mat(), keypoints2, descriptors2);

    // マッチングを行うために、BFMatcher（Brute-Force Matcher）を使用
    cv::BFMatcher matcher(cv::NORM_HAMMING);
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    // マッチング結果の良いものを選定（このしきい値は調整が必要です）
    double max_dist = 0;
    double min_dist = 100;
    for (int i = 0; i < descriptors1.rows; i++) {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }

    // 良いマッチだけを保持
    std::vector<cv::DMatch> good_matches;
    for (int i = 0; i < descriptors1.rows; i++) {
        if (matches[i].distance <= std::max(2 * min_dist, 30.0)) {
            good_matches.push_back(matches[i]);
        }
    }

    // std::cout << "Good matches found: " << good_matches.size() << std::endl;

    // しきい値に基づいて、マーカーが識別されたかどうかを返す
    // この値も実験に基づいて適切に設定する必要があります
    return good_matches.size() > 10;
}

bool isInside(const cv::Rect &inner, const cv::Rect &outer) {
    return (inner.x >= outer.x) && (inner.y >= outer.y) &&
           (inner.x + inner.width <= outer.x + outer.width) &&
           (inner.y + inner.height <= outer.y + outer.height);
}

void filterRectangles(std::vector<std::vector<cv::Point>>& rects) {
    std::vector<cv::Rect> rectangles;
    for (const auto& points : rects) {
        rectangles.push_back(cv::boundingRect(points));
    }

    std::vector<bool> toRemove(rectangles.size(), false);
    for (size_t i = 0; i < rectangles.size(); ++i) {
        for (size_t j = 0; j < rectangles.size(); ++j) {
            if (i != j && isInside(rectangles[i], rectangles[j])) {
                toRemove[i] = true;
                break;
            }
        }
    }

    std::vector<std::vector<cv::Point>> filteredRects;
    for (size_t i = 0; i < rects.size(); ++i) {
        if (!toRemove[i]) {
            filteredRects.push_back(rects[i]);
        }
    }

    rects = std::move(filteredRects);
}

bool RectangleDetection::Detect(cv::Mat& image, std::vector<std::vector<cv::Point>>& rect_list) {
    //2値化を行う
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);

    // 2値化 with Otsu
    cv::Mat thresholdImg;
    cv::threshold(gray, thresholdImg, 0, 255, cv::THRESH_OTSU);

    // 2値化画像から輪郭を抽出
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresholdImg, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

    // テンプレート画像をロード
    cv::Mat templateImage = cv::imread("marker.png", cv::IMREAD_GRAYSCALE);
    if (templateImage.empty()) {
        std::cerr << "Error: Template image not found." << std::endl;
        return false;
    }

    // テンプレート画像のコーナー点
    std::vector<cv::Point2f> templateCorners = {
        cv::Point2f(0, 0),
        cv::Point2f(templateImage.cols, 0),
        cv::Point2f(templateImage.cols, templateImage.rows),
        cv::Point2f(0, templateImage.rows)
    };

    std::vector<std::vector<cv::Point>> candidate_rects;
    for (const auto& contour : contours) {
        std::vector<cv::Point> contour_poly;
        cv::approxPolyDP(contour, contour_poly, 5, true);
        double area = cv::contourArea(contour_poly);

        if (contour_poly.size() == 4 && 2000 < area) {
            // 内包された矩形をフィルタリング
            candidate_rects.push_back(contour_poly);
        }
    }
    std::vector<std::vector<cv::Point>> out_sides_rects;
    filterRectangles(candidate_rects);
    // contour_polyからrect_listへの追加
    for (const auto& candidate_rect : candidate_rects) {
        out_sides_rects.push_back(candidate_rect);
    }

    for (const auto& out_sides_rect : out_sides_rects) {
        // ホモグラフィ行列を計算する
        std::vector<cv::Point2f> detectedCorners;
        for (const auto& pt : out_sides_rect) {
            detectedCorners.push_back(cv::Point2f(pt.x, pt.y));
        }
        cv::Mat homography = cv::findHomography(detectedCorners, templateCorners);

        // ホモグラフィ変換を適用して画像をワーピングする
        cv::Mat warpedImage;
        cv::warpPerspective(image, warpedImage, homography, templateImage.size());

        // テンプレート画像とワーピング画像を比較する
        double similarity = CompareImages(warpedImage, templateImage); // この関数は実装が必要です。
        // printf("Point: %f\n", similarity);
        if (similarity) {
            // マーカーとして識別された四角形をリストに追加
            rect_list.push_back(out_sides_rect);
        }
    }


    for (const auto& rect : rect_list) {
        // ここで四角形の各頂点に従って描画
        const cv::Point* p = &rect[0];
        int n = (int)rect.size();
        cv::polylines(image, &p, &n, 1, true, cv::Scalar(0, 255, 0), 2);
    }

    return !rect_list.empty();
}
