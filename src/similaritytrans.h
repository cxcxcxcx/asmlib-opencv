#ifndef SIMILARITYTRANS_H
#define SIMILARITYTRANS_H

#include "cv.h"
using cv::Mat;

namespace StatModel {

class ShapeVec;

//! A similarity transformation
class SimilarityTrans{
public:
    //! Inverted transformation
    /*! \note src and dst can be the same vector
    */
    void invTransform(const ShapeVec &src, ShapeVec &dst) const;

    //! Transform a shape
    /*! \note src and dst can be the same vector
    */
    void transform(const ShapeVec &src, ShapeVec &dst) const;

    //! find the transformation that best align x to xp
    void setTransformByAlign(const ShapeVec &x, const ShapeVec &xp);

    //! Warp an image by this similarity transform.
    void warpImage(const Mat &imgSrc, Mat &imgDst) const;

    //! Warp an image by the inverted similarity transform.
    void warpImgBack(const Mat &imgSrc, Mat &imgDst, bool useDstSize=false) const;

    //! Get the scale factor
    double getS() const { return sqrt(a*a+b*b); }

    SimilarityTrans():Xt(0), Yt(0), a(1), b(0){}

    //! The multiplication of two transformations.
    SimilarityTrans operator *(const SimilarityTrans & s2) const {
        SimilarityTrans ns;
        ns.a = a*s2.a-b*s2.b;
        ns.b = s2.a*b+s2.b*a;
        ns.Xt = a*s2.Xt - b*s2.Yt + Xt;
        ns.Yt = b*s2.Xt + a*s2.Yt + Yt;
        return ns;
    }

    //! X translate
    double Xt;
    //! Y translate
    double Yt;

    //! a in similarity transformation matrix
    double a;
    //! b in similarity transformation matrix
    double b;
};

} // Namespace
#endif // SIMILARITYTRANS_H
