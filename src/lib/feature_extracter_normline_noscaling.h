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


#ifndef FEATURE_EXTRACTER_NORMLINE_NOSCALING_H
#define FEATURE_EXTRACTER_NORMLINE_NOSCALING_H

#include "featureextracter.h"

namespace StatModel {
class FeatureExtracterNormLineNoScaling
    : public FeatureExtracter {
public:
    FeatureExtracterNormLineNoScaling(
            int pointsPerDirection, int searchPointsPerDirection)
        : pointsPerDirection_(pointsPerDirection),
          searchPointsPerDirection_(searchPointsPerDirection) {
    };

    virtual FeatureExtracter *clone() {
        return new FeatureExtracterNormLineNoScaling(*this);
    }

    /// See base clase for documentation.
    virtual bool getCandidatesWithFeature(
        const vector< Point2i >& points,
        int pointId,
        int level,
        vector< Point2i >& candidatePoints,
        vector< Mat_< FeatureDataType > >& features
    );

    /// See base clase for documentation.
    virtual Mat_< FeatureDataType > getFeature(
        const vector< Point_< int > >& points,
        int pointId, int level);

    // TODO: Get rid of this...
    //! step: ?*100/sqrt(area)
    double searchStepAreaRatio_;
protected:
    void getPointsOnNorm(
        const vector< Point2i >& points, int pointId,
        int level,
        vector< Point2i >& outputPoints,
        double step, int pOffset);
    int pointsPerDirection_;
    int searchPointsPerDirection_;
};
} // Namespace

#endif // FEATURE_EXTRACTER_NORMLINE_NOSCALING_H
