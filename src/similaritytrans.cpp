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

#include "similaritytrans.h"
#include "shapevec.h"

void SimilarityTrans::backTransform(const ShapeVec &src, ShapeVec &dst){
    int nP = src.nPoints();
    double x11, x12, x21, x22;
    x11 = a/(a*a+b*b);
    x12 = b/(a*a+b*b);
    x21 = -b/(a*a+b*b);
    x22 = a/(a*a+b*b);

    dst.create(nP<<1, 1);
    double xt, yt;
    for (int i=0; i < nP; i++) {
        xt = src.X(i) - Xt;
        yt = src.Y(i) - Yt;
        dst.X(i) = x11 * xt + x12 * yt;
        dst.Y(i) = x21 * xt + x22 * yt;
    }
}

void SimilarityTrans::transform(const ShapeVec &src, ShapeVec &dst) const{
    int nP = src.nPoints();
    dst.create(nP<<1, 1);
    double xt, yt;
    for (int i=0; i < nP; i++) {
        xt = src.X(i);
        yt = src.Y(i);
        dst.X(i) = a * xt - b * yt + Xt;
        dst.Y(i) = b * xt + a * yt + Yt;
    }
}

void SimilarityTrans::setTransformByAlign(const ShapeVec &x, const ShapeVec &xp) {
    int nP = x.rows / 2;
    a = xp.dot(x) / x.dot(x);
    b = 0;
    for (int i=0; i<nP; i++)
        b += x.X(i) * xp.Y(i) - x.Y(i)*xp.X(i);
    b /= x.dot(x);
    //st.s = sqrt(a*a+b*b);
    //st.theta = atan(b/a);
    double xxm, xym;
    xxm = x.getXMean();
    xym = x.getYMean();
    Xt = -a * xxm + b * xym + xp.getXMean();
    Yt = -b * xxm - a * xym + xp.getYMean();
}

void SimilarityTrans::warpImage(const Mat &imgSrc, Mat &imgDst)
{
    Mat_< double > M(2, 3);
    M<< a, -b, Xt,
        b,  a, Yt;
    cv::warpAffine(imgSrc, imgDst, M, imgSrc.size(), cv::INTER_LINEAR);
}

void SimilarityTrans::warpImgBack(const cv::Mat& imgSrc, Mat& imgDst, bool useDstSize)
{

    Mat_< double > M(2, 3), mV;
    M<< a, -b, Xt,
        b,  a, Yt;
    //cv::invertAffineTransform(M, mV);
    if (useDstSize)
        cv::warpAffine(imgSrc, imgDst, M, imgDst.size(), cv::INTER_LINEAR|cv::WARP_INVERSE_MAP);
    else
        cv::warpAffine(imgSrc, imgDst, M, imgSrc.size(), cv::INTER_LINEAR|cv::WARP_INVERSE_MAP);
}
