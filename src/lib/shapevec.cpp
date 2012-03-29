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

#include "shapevec.h"

namespace StatModel {
void ShapeVec::zeroGravity()
{
    doTranslate(-getXMean(), -getYMean());
}

void ShapeVec::scaleToOne()
{
    doScale(1/norm(*this));
}

void ShapeVec::alignTo( const ShapeVec &ref )
{
    // Find best scale factor and theta.
    static SimilarityTrans st;
    st.setTransformByAlign(*this, ref);
    st.transform(*this, *this);
//     double b = 0;
//     int n = rows>>1;
//     for (int i=0; i<n; i++)
//         b += X(i) * ref.Y(i) - Y(i) * ref.X(i);
    
    doScale(1/this->dot(ref));
}

void ShapeVec::doTranslate(double vX, double vY)
{
    for (int i=0; i<rows/2; i++)
        (*this)(i, 0) += vX;
    for (int i=rows/2; i<rows; i++)
        (*this)(i, 0) += vY;
//    translateX += vX;
//    translateY += vY;
}

void ShapeVec::doScale(double r)
{
    (*this) *= r;
//    scale *= r;
//    translateX *= r;
//    translateY *= r;
}

void ShapeVec::restoreToPointList(vector< cv::Point_< int > >& points,
                                  const SimilarityTrans& st)
{
    // resize this shape
    points.resize(this->nPoints());
    ShapeVec sv;
    st.transform(*this, sv);
    for (int i=0;i<nPoints();i++){
        points[i].x = sv(i, 0);
        points[i].y = sv.Y(i);
    }
}

cv::Rect_< double > ShapeVec::getBoundRect()
{
    // Estimate scale
    int nP = nPoints();
    double minX=1e10, minY=1e10, maxX=-1e10, maxY=-1e10;
    double x, y;
    for (int i=0;i<nP;i++) {
        x = X(i);
        y = Y(i);
        if (x<minX) minX = x;
        if (x>maxX) maxX = x;
        if (y<minY) minY = y;
        if (y>maxY) maxY = y;
    }
    return cv::Rect_< double >(
        cv::Point_<double>(minX, minY), 
        cv::Size_<double>(maxX-minX, maxY-minY));
}

SimilarityTrans ShapeVec::getShapeTransformFitingSize(
        const cv::Size &rect, 
        double scaleRatio, double xOffset, double yOffset)
{
    // Estimate scale and translate
    cv::Rect_<double> bdRect=getBoundRect();
//         printf("minX: %g, minY: %g, maxX: %g, maxY: %g\n",
//                minX, minY, maxX, maxY);
    double ratioX, ratioY, ratio;
    ratioX = rect.width/bdRect.width;
    ratioY = rect.height/bdRect.height;
    if (ratioX < ratioY)
        ratio = ratioX;
    else
        ratio = ratioY;
    double transX, transY;
    
    ratio *= scaleRatio;
    transX = bdRect.x - bdRect.width*(ratioX/ratio-1 + xOffset)/2;
    transY = bdRect.y - bdRect.height*(ratioY/ratio-1 + yOffset)/2;
    
    SimilarityTrans st;
    st.a = ratio;
    st.b = 0;
    st.Xt = -transX*ratio;
    st.Yt = -transY*ratio;
//     printf("%lf, %lf\n", st.Xt, st.Yt);
    return st;
}

void ShapeVec::fromPointList(const std::vector< cv::Point2i >& v)
{
	this->create(v.size()*2, 1);
    for (int i=0; i<(rows>>1); i++){
        (*this)(i, 0) = v[i].x;
        (*this)(i+(rows>>1), 0) = v[i].y;
//        shapeVec.setTransform(1, 0, 0);
    }
}

} // Namespace