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

#ifndef ASMMODEL_H
#define ASMMODEL_H
#include "shapemodel.h"
#include "cv.h"
using cv::Point_;
using cv::Mat_;

#define ASM_FIT_VERBOSE_NOVERBOSE 0
#define ASM_FIT_VERBOSE_AT_LEVEL 1
#define ASM_FIT_VERBOSE_AT_ITERATION 2
#define ASM_FIT_VERBOSE_AT_POINT 3

class ASMModel : public ShapeModel
{
private:
    //! Inverted Covariance matrix pyramids for each landmark point
    vector< vector< Mat_< double > > > iCovarG;

    //! Mean vector pyramids for each landmark point
    vector< vector< Mat_< double > > > meanG;
    
    //! parameter k for ASM
    int localFeatureRad;
    
    //! parameter ns for ASM
    int ns;
    
    //! Build statistic models for local difference at each landmark point.
    /// \note This is used when calculating Mahalanobis distance
    void buildLocalDiffStructure();
    
    PCA pcaPyr[3];
    double sigma2Pyr[3];
    
public:
    ASMModel():localFeatureRad(4), ns(6){}
    
    //! Do ASM search on a image patch, return the fitted vector
    /*!
        \param img Grayscale image.
        \param fitResult ASM fit result.
    */
    void doSearch(const Mat & img, FitResult &fitResult, int verbose);
    
    //! Use ASM model to fit faces in the image, return the fitted vectors
    /*!
        \param img Input image
        \param fitResult ASM fit result.
    */
    void fit(const Mat & img, vector< FitResult > &fitResult, 
             cv::CascadeClassifier &faceCascade, bool biggestOnly=false, int verbose=0);
    
    //! Build the model
    void buildModel();
    
    //! Save the model into a file
    void saveToFile(ModelFile &file);
    
    //! Load the model from a file
    void loadFromFile(ModelFile &file);
    
    //! Show the result in the image
    void showResult(Mat &img, vector< FitResult > &res);
    
    //! Project a fit result to a vector of points
    void resultToPointList(const FitResult& fitResult, vector< Point_<int> > &pV);

    //! Find the best parameter and transformation for the given shape.
    /*!
        \param ShapeVec a shape vector 
        \param fitResult a ASM result.
    */
    void findParamForShape(const ShapeVec &Y, FitResult & fitResult);
    
    //! Use Bayesian Tangent Shape Model to find best parameter and transformation for the given shape
    /*!
        \param l level in image pyramid.
    */
    void findParamForShapeBTSM(const ShapeVec &Y, const ShapeVec &Y_old,
                               FitResult & fitResult, FitResult &b_old, int l);
};

#endif // ASMMODEL_H
