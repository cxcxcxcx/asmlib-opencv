#include "featureextracter.h"

namespace StatModel {
bool FeatureExtracter::loadImage(const Mat& imgdata)
{
    bool imgLoaded = false;
    cv::buildPyramid(imgdata, this->imgPyramid, 3);
    Mat grayImg;
    if (imgdata.channels() == 3){
        cv::cvtColor(imgdata, grayImg, CV_BGR2GRAY);
        imgPyrGray.resize(4);
        cv::buildPyramid(grayImg, imgPyrGray, 3);
    }
    else {
        grayImg = imgdata;
        this->imgPyrGray.resize(4);
        for (int i = 0; i <= 3; i++)
            imgPyrGray[i] = imgPyramid[i];
    }
    imgPyrGrayDeriv.resize(4);
    for (int i=0; i<=3; i++){
        imgPyrGray[i].convertTo(imgPyrGrayDeriv[i], CV_64F);
        cv::Sobel(imgPyrGrayDeriv[i], imgPyrGrayDeriv[i], CV_64F, 1, 1);
    }
    imgLoaded = true;
    return imgLoaded;
}

bool FeatureExtracter::releaseImage()
{
    bool imgLoaded = true;
    if (imgLoaded){
        // imgdata.release();
        for (int i=0; i<=3; i++){
            imgPyramid[i].release();
            imgPyrGray[i].release();
            imgPyrGrayDeriv[i].release();
        }
        imgLoaded = false;
    }
    return !imgLoaded;
}


Point2d FeatureExtracter::getNormDirection(
    const vector< Point_< int > >& points, int pointId) {
    // Find the norm direction
    Point2d va, vb, vDirection;
    va = points[shapeInfo_->pointInfo[pointId].connectFrom] - points[pointId];
    vb = points[shapeInfo_->pointInfo[pointId].connectTo] - points[pointId];
    
    // TODO: Clean the codes here.
    double td;
    td = norm(va);
    if (td>1e-10) va *= 1 / td;
    td = norm(vb);
    if (td>1e-10) vb *= 1 / td;
    vDirection.x = - va.y + vb.y;
    vDirection.y = va.x - vb.x;
    if (norm(vDirection)<1e-10){
        if (norm(va)>1e-10)
            vDirection = - va;
        else
            vDirection.x = 1, vDirection.y = 0;
    }
    else
        vDirection *= 1 / norm(vDirection);
    return vDirection;
}

} // Namespace