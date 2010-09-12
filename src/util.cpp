#include "util.h"

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

Mat drawMarkPointsOnImg(Mat& img, const vector< Point >& vP, ShapeInfo& shapeInfo, 
                          bool drawInPlace)
{
    Mat mb;
    if (drawInPlace)
        mb = img;
    else
        mb = img.clone();
    
    for (uint i=0;i<vP.size();i++){
        cv::circle(mb, vP[i], 3, cv::Scalar(25, 50, 255), 1, CV_AA);
    }
    for (int i=0;i<shapeInfo.nPath;i++){
        for (int j=shapeInfo.pathSeg[i]; j<shapeInfo.pathSeg[i+1]; j++){
            cv::line(mb, vP[shapeInfo.pointInfo[j].connectFrom],
                     vP[j],
                     cv::Scalar(200, 30, 80), 1, CV_AA);
        }
    }
    return mb;
}
