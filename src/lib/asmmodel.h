#ifndef ASMMODEL_H
#define ASMMODEL_H
#include "shapemodel.h"
#include "cv.h"

/*! \mainpage asmlib-opencv Documentation
 *
 * \section intro_sec Introduction
 *
 * An open source Active Shape Model implementation written by C++ using
 * OpenCV 2.0 (or above), no other dependencies.
 *
 * The library has been successfully compiled in Linux (both 32 and 64 bits),
 * Windows(both VC and MinGW) and Mac OS X. Both training and fitting codes
 * are provided. For Windows users, a binary demo is available for download.
 *
 * This doc is for codes and APIs. For general info, see:
 *     http://code.google.com/p/asmlib-opencv/
 *
 * For compiling the codes, running demo and building models, see the
 * <a href="http://code.google.com/p/asmlib-opencv/w/list">Wiki Pages</a>.
 *
 * For APIs, see \ref API "API".
 *
 */

/**
 * \todo Change the magic numbers to configurable variables.
 * \todo Smaller memory consumption while training.
 */

//! Namespace for all ASM related stuffs.
namespace StatModel {

using cv::Point_;
using cv::Mat_;

#define ASM_FIT_VERBOSE_NOVERBOSE 0
#define ASM_FIT_VERBOSE_AT_LEVEL 1
#define ASM_FIT_VERBOSE_AT_ITERATION 2
#define ASM_FIT_VERBOSE_AT_POINT 3

class ASMModel;

/**
 * \defgroup API APIs.
 */

//! Result for ASM fitting.
/**
 * \ingroup API
 */
class ASMFitResult : public FitResult {
private:
    ASMModel *asmModel;
public:
    ASMFitResult(ASMModel *model): asmModel(model) {}

    //! Get the result in landmark point list form.
    void toPointList(vector< Point_<int> > &pV) const;
};

//! Active Shape Model
/**
 * \ingroup API
 */
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
    //! Empty ASM model.
    ASMModel():localFeatureRad(4), ns(6){}

    //! Initialize by a file.
    ASMModel(const string& filename) { loadFromFile(filename); }

    //! Search for objects (e.g. faces) in image, and run ASM on each object.
    /*!
     * This runs OpenCV object detection and fit ASM model on each detected
     * region.
     *
        \param img Input image
        \param detectedObjs Bounding boxes for all objects. (Usually it can be
                              the result of OpenCV object detection)
        \param verbose Verbosity level.
        \return A vector of FitResult's.
    */
    vector< ASMFitResult > fitAll(
        const Mat & img,
        const vector< cv::Rect > & detectedObjs,
        int verbose=0);

    //! Do ASM fitting on an image patch, return the fitted vector
    /*!
     *  I'd recommend use ASMModel::fitAll in most cases.
        \param img An image patch (cropped by object detection).
        \param verbose Verbosity level.
        \return Result of ASM fitting.
    */
    ASMFitResult fit(const Mat & img, int verbose=0);

    //! Build the model
    void buildModel(const string& shapeDefFile, const string& ptsListFile);

    //! Save the model into a file
    void saveToFile(const string& filename);

    //! Load the model from a file
    void loadFromFile(const string& filename);

    //! Show the result in the image
    void showResult(Mat &img, const vector< ASMFitResult > &res);

    //! Find the best parameter and transformation for the given shape.
    /*!
        \param ShapeVec a shape vector
        \param fitResult a ASM result.
    */
    void findParamForShape(const ShapeVec &Y, ASMFitResult & fitResult);

private:
    //! Use Bayesian Tangent Shape Model to find best parameter and transformation for the given shape
    /*!
        \param l level in image pyramid.
    */
    void findParamForShapeBTSM(const ShapeVec &Y, const ShapeVec &Y_old,
                               ASMFitResult & fitResult, ASMFitResult &b_old, int l);

    //! Save the model into a file
    void saveToFile(ModelFile &file);

    //! Load the model from a file
    void loadFromFile(ModelFile &file);
};

} // Namespace
#endif // ASMMODEL_H
