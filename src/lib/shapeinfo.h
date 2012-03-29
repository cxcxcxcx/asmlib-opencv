#ifndef SHAPEINFO_H
#define SHAPEINFO_H

#include "afreader.h"
#include "modelfile.h"
#include "shapevec.h"

using cv::Mat;
using cv::Point;
using std::vector;

namespace StatModel {
//! Defines how landmark points form a "shape".
/*!
 * We typically connect landmark points into some contours to form a real shape.
 *
 * For example, a shape "face" may include several "paths" (contours): two
 * contours of eyes, one contour for the nose, one contour for the lip, etc.
 */
class ShapeInfo
{
private:
    //! Index of the first point of each contour in the landmark point list.
    vector< int > contourStartInd;

    /*!
     * \brief If a contour is closed or not.
     *
     * A contour can be either open (face contour) or closed (eye contour).
     */
    vector< int > contourIsClosed;

    //! The number of contours in a shape.
    int nContours;

public:
    //! Point in shape definition.
    struct PointInfo
    {
        int type;
        int pathId;
        int connectFrom;
        int connectTo;
    };

    //! Information for each landmark point.
    vector< PointInfo > pointInfo;

    //! Dump info to a model file.
    void writeToFile(ModelFile &f) const;

    //! Load shape info from a model file.
    void readFromFile(ModelFile &f);

    //! Load from shape description file, return the number landmark points.
    int loadFromShapeDescFile(AFReader &shapeDefFile);

    //! Draw a list of points on an image.
    Mat drawMarkPointsOnImg(
      Mat &img, const vector< Point > &vP, bool drawInPlace = false) const;
};
} // Namespace
#endif