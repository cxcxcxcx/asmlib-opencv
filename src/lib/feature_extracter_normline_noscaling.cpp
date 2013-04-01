/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2013  CHEN Xing <email>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "feature_extracter_normline_noscaling.h"


namespace StatModel {

void FeatureExtracterNormLineNoScaling::getPointsOnNorm(
        const vector< Point2i >& points, int pointId, 
        int level,
        vector< Point2i >& outputPoints, 
        double step, int pOffset) {
    Point2d vDirection = getNormDirection(points, pointId);
    int i, j;
    int nx, ny;
    int offsetX, offsetY;
    int prevX, prevY;

    // Find the center point, here step===1
    prevX = 0;
    prevY = 0;
    nx = ny = 0;
    j = 1;
    for (i=1; i<=abs(pOffset);i++){
        do {
            nx = cvRound(j*vDirection.x);
            ny = cvRound(j*vDirection.y);
            j++;
        } while (nx == prevX && ny == prevY);

        prevX = nx;
        prevY = ny;
    }
    j--;
//     printf("%d, %d\n", nx, ny);
    if (pOffset>0)
        offsetX = nx, offsetY = ny;
    else
        offsetX = -nx, offsetY = -ny;

    // Apply the "step", and find points
    vDirection *= step;
    prevX = 0;
    prevY = 0;
    nx = ny = 0;

    int pointsPerDirection = pointsPerDirection_;
    // Test best j
    j = 1;
    for (i=1; i <= pointsPerDirection; i++){
        do {
            nx = cvRound(j*vDirection.x);
            ny = cvRound(j*vDirection.y);
            j++;
        } while (nx == prevX && ny == prevY);

        prevX = nx;
        prevY = ny;
    }
    j--;

    outputPoints.resize(2 * pointsPerDirection + 1);
    int rX, rY;
    for (i = pointsPerDirection; i >= -pointsPerDirection; i--){
        rX = (points[pointId].x>>level) + nx + offsetX;
        rY = (points[pointId].y>>level) + ny + offsetY;
        if (rX < 0) rX = 0;
        if (rY < 0) rY = 0;
        if (rX >= (imgPyramid[level].cols))
            rX = (imgPyramid[level].cols) - 1;
        if (rY >= (imgPyramid[level].rows)) 
            rY = (imgPyramid[level].rows) - 1;
        outputPoints[i + pointsPerDirection] = Point_< int >(rX, rY);
        do {
            nx = cvRound(j*vDirection.x);
            ny = cvRound(j*vDirection.y);
            j--;
        } while (nx == prevX && ny == prevY);
        prevX = nx;
        prevY = ny;
    }
}

bool FeatureExtracterNormLineNoScaling::getCandidatesWithFeature(
        const vector< Point2i >& points,
        int pointId,
        int level,
        vector< Point2i >& candidatePoints,
        vector< Mat_< FeatureDataType > >& features) {
    assert(searchStepAreaRatio_ > 0);
    
    double absSum;
    vector< Point2i > normPoints;
    Mat_<double> img = this->imgPyrGrayDeriv[level];
    for (int e = searchPointsPerDirection_; e >= -searchPointsPerDirection_; e--){
        getPointsOnNorm(points, pointId, level, normPoints, 2 * searchStepAreaRatio_, e);

        Mat_< FeatureDataType > nrmV(2 * pointsPerDirection_ + 1, 1);
        absSum = 0;
        for (int j = -pointsPerDirection_; j <= pointsPerDirection_; j++){
            nrmV(j + pointsPerDirection_, 0) = img(normPoints[j + pointsPerDirection_]);
            absSum += fabs(nrmV(j + pointsPerDirection_, 0));
        }
        // TODO: Use cv::normalize to normalize the vector
        nrmV *= 1 / absSum;
        
        candidatePoints.push_back(normPoints[pointsPerDirection_]);
        features.push_back(nrmV);
    }
    return true;
}

Mat_< FeatureDataType > FeatureExtracterNormLineNoScaling::getFeature(
        const vector< Point_< int > >& points,
        int pointId, int level) {
    // TODO: precompute step
    // First, we have find a proper "step" based on the size of face
    int xMin, xMax, yMin, yMax;
    double myStep;
    xMin = yMin = 100000000;
    xMax = yMax = 0;
    for (int j=0; j<points.size(); j++){
        if (points[j].x < xMin)
            xMin = points[j].x;
        if (points[j].y < yMin)
            yMin = points[j].y;
        if (points[j].x > xMax)
            xMax = points[j].x;
        if (points[j].y > yMax)
            yMax = points[j].y;
    }
    myStep = 1.3* sqrt( (xMax-xMin)*(yMax-yMin) / 10000.);
//         printf("step: %f\n", myStep[i]);
    
    vector< Point2i > normPoints;
    this->getPointsOnNorm(points, pointId, level, normPoints, myStep, 0);
    Mat_< double > diffV(2 * pointsPerDirection_ + 1, 1);
    double absSum = 0;
    for (int i= pointsPerDirection_; i >= -pointsPerDirection_; i--){
        diffV(i+pointsPerDirection_, 0) = imgPyrGrayDeriv[level](normPoints[i + pointsPerDirection_]);
        absSum += fabs(diffV(i + pointsPerDirection_, 0));
    }
    if (absSum==0){
        printf("Warning: absSum=0....Level: %d, pID: %d\n", level, pointId);
        // show(level, pId);
    }
    else
        diffV *= 1/absSum;
    return diffV;
}

} // Namespace