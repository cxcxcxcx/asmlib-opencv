#ifndef UTIL_H
#define UTIL_H

#include "cv.h"
#include "shapevec.h"
#include "shapemodel.h"
#include <vector>
using cv::Rect;
using cv::Mat;
using cv::Point;
using cv::Size;

using std::vector;

namespace StatModel {

//! face detect
Rect findOneFace(const Mat &img);
vector< Rect > findAllFaces(const Mat &img);

// fill a convex poly by a point vector.
void fillConvexFromPoints(Mat &img, vector< Point > &vP, cv::Scalar s);

} // Namespace

#endif
