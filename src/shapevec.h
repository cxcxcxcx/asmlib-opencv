/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SHAPEVEC_H
#define SHAPEVEC_H

#include "similaritytrans.h"

#include "cv.h"
#include "highgui.h"
#include <vector>
using std::vector;
using cv::Point_;
using cv::Mat_;
using cv::Mat;
using cv::imread;
using cv::mean;
typedef unsigned int uint;
//! Shape Vector
class ShapeVec : public Mat_< double > {
public:
    ShapeVec(const Mat_< double > &a):Mat_< double >(a){}
    ShapeVec & operator =(const Mat_< double > &a) {
        Mat_< double >::operator=(a);
        return *this;
    }
    ShapeVec(){}
    
    //! Align to another shape vector
    void alignTo(const ShapeVec & ref);
    
    //! Move the center of gravity to origin. X and Y are moved seperately.
    void zeroGravity();
    
    //! Scale the vector's norm to 1.
    void scaleToOne();
    
    void doTranslate(double vX, double vY);
    void doScale(double r);
    
    double getXMean() const{return mean(rowRange(0, rows/2))[0];}
    double getYMean() const{return mean(rowRange(rows/2, rows))[0];}
    
    double X(int i) const{ return (*this)(i, 0); }
    double & X(int i) { return (*this)(i, 0); }
    double Y(int i) const{ return (*this)(i+(rows>>1), 0); }
    double & Y(int i) { return (*this)(i+(rows>>1), 0); }
    
    cv::Rect_<double> getBoundRect();
    
    void restoreToPointList(vector< Point_< int > > &v, const SimilarityTrans &st);
	void fromPointList(const vector< cv::Point2i > &v);
    
    int nPoints() const { return (rows>>1); }

    SimilarityTrans getShapeTransformFitingSize(
        const cv::Size &rect, 
        double scaleRatio=0.9, double xOffset=0, double yOffset=0);

//    void setTransform(double s, double tX, double tY){
//        scale = s; translateX = tX; translateY = tY;
//    }

/*    bool hasTransformationData(){
        return (sTrans.a*sTrans.a+sTrans.b*sTrans.b)!=0;
    }*/ //return scale!=0; }
// private:
    // Save transformation
    // Note: scale first, then transalte.
    // double scale, translateX, translateY;
//     SimilarityTrans sTrans;
};

#endif // SHAPEVEC_H
