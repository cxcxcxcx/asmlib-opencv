#include "util.h"

namespace StatModel {

void fillConvexFromPoints(Mat &img, vector< Point > &pList, cv::Scalar s){
    cv::Mat_< cv::Vec2i > pointMat(1, pList.size());
    for (size_t i=0; i<pList.size(); i++)
        pointMat(0, i) = cv::Vec2i(pList[i].x, pList[i].y);
    vector< Point > vP;
    cv::convexHull(pointMat, vP);
    Point *pP = new Point[vP.size()];
    std::copy(vP.begin(), vP.end(), pP);
    cv::fillConvexPoly(img, pP, vP.size(), s);
    delete [] pP;
}

} // Namespace