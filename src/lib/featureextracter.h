#ifndef FEATUREEXTRACTER_H
#define FEATUREEXTRACTER_H

#include "shapeinfo.h"

using cv::Point2i;
using cv::Point2d;

namespace StatModel {
typedef double FeatureDataType;

class FeatureExtracter
{
public:
    FeatureExtracter() {
    }

    void setShapeInfo(const ShapeInfo& shapeInfo) {
        shapeInfo_ = &shapeInfo;
    }

    virtual FeatureExtracter *clone() = 0;

    virtual ~FeatureExtracter() { }

    virtual bool loadImage(const Mat& img);
    virtual bool releaseImage();

    /// Given a current shape estimation, find candidates for next iteration
    /*!
     * The feature vector extracted together with each vector is also returned.
        \param shapeVec The current shape estimation
        \param pointId The point to be updated
        \param level position in the pyramid level
        \param candidatePoints Output. A list of candidate points.
        \param features Output. A list of feature vectors, one for eacy entry
            in candidatePoints.
    */
    virtual bool getCandidatesWithFeature(
        const vector< Point_< int > >& points,
        int pointId,
        int level,
        vector< Point_< int > >& candidatePoints,
        vector< Mat_< FeatureDataType > >& features
    ) = 0;

    /// Extract feature at a specific pixel.
    /*!
        \param shapeVec The shape vector.
        \param pointId The point ID in the shape vector.
        \param level position in the pyramid level
    */
    virtual Mat_< FeatureDataType > getFeature(
        const vector< Point_< int > >& points,
        int pointId, int level) = 0;


    Point2d getNormDirection(
        const vector< Point2i >& points, int pointId);
protected:
    /// Image pyramid
    vector< Mat > imgPyramid;
    vector< Mat_<double> > imgPyrGrayDeriv;
    vector< Mat > imgPyrGray;

    /// Information about shapes and paths.
    const ShapeInfo* shapeInfo_;
};
} // Namespace

#endif // FEATUREEXTRACTER_H
