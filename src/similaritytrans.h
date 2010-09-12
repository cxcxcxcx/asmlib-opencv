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

#ifndef SIMILARITYTRANS_H
#define SIMILARITYTRANS_H

#include "cv.h"
using cv::Mat;

class ShapeVec;

//! A similarity transformation
class SimilarityTrans{
public:
    //! Inverted transformation
    /*! \note src and dst can be the same vector
    */
    void backTransform(const ShapeVec &src, ShapeVec &dst);
    
    //! Transform a shape
    /*! \note src and dst can be the same vector
    */
    void transform(const ShapeVec &src, ShapeVec &dst) const;
    
    //! find the transformation that best align x to xp
    void setTransformByAlign(const ShapeVec &x, const ShapeVec &xp);
    
    void warpImage(const Mat &imgSrc, Mat &imgDst);
    void warpImgBack(const Mat &imgSrc, Mat &imgDst, bool useDstSize=false);
    
    //! Get the scale factor
    double getS() const { return sqrt(a*a+b*b); }
    
    SimilarityTrans():Xt(0), Yt(0), a(1), b(0){}
    
    SimilarityTrans operator *(const SimilarityTrans & s2){
        SimilarityTrans ns;
        ns.a = a*s2.a-b*s2.b;
        ns.b = s2.a*b+s2.b*a;
        ns.Xt = a*s2.Xt - b*s2.Yt + Xt;
        ns.Yt = b*s2.Xt + a*s2.Yt + Yt;
        return ns;
    }
// private:
    //! X translate
    double Xt;
    //! Y translate
    double Yt;
    
    //! a in similarity transformation matrix
    double a;
    //! b in similarity transformation matrix
    double b;
}; 
#endif // SIMILARITYTRANS_H
