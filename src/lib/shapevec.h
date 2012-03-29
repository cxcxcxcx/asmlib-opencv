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

namespace StatModel {

//! Shape vector, presented as a 1-D OpenCV matrix.
class ShapeVec : public Mat_< double > {
  public:
    //! Construct by a matrix.
    ShapeVec(const Mat_< double > &a):Mat_< double >(a){}

    //! Assign shape vector by a Mat.
    ShapeVec & operator =(const Mat_< double > &a) {
        Mat_< double >::operator=(a);
        return *this;
    }

    //! Default constructor does nothing.
    ShapeVec(){}

    //! Align to another shape vector
    void alignTo(const ShapeVec & ref);

    //! Move the center of gravity to origin. X and Y are moved seperately.
    void zeroGravity();

    //! Scale the vector's norm to 1.
    void scaleToOne();

    void doTranslate(double vX, double vY);
    void doScale(double r);

    double getXMean() const {return mean(rowRange(0, rows / 2))[0];}
    double getYMean() const {return mean(rowRange(rows / 2, rows))[0];}

    double X(int i) const { return (*this)(i, 0); }
    double & X(int i) { return (*this)(i, 0); }
    double Y(int i) const { return (*this)(i + (rows >> 1), 0); }
    double & Y(int i) { return (*this)(i+(rows>>1), 0); }

    cv::Rect_<double> getBoundRect();

    //! Do a similarity transform and restore it into a list of points.
    void restoreToPointList(
      vector< Point_< int > > &v,
      const SimilarityTrans &st
    );

    //! Flatten a list of points to the current shape vector.
    void fromPointList(const vector< cv::Point2i > &v);

    //! Number of points for this Shape
    int nPoints() const { return (rows >> 1); }

    //! Given a target size, find a proper transformation to rescale the shape.
    SimilarityTrans getShapeTransformFitingSize(
        const cv::Size &rect,
        double scaleRatio=0.9, double xOffset=0, double yOffset=0);
};
} // Namespace
#endif // SHAPEVEC_H
