#ifndef MODELIMAGE_H
#define MODELIMAGE_H
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
//#include "highgui.h"
#include "shapevec.h"
#include "shapeinfo.h"
using std::string;
using std::vector;
using cv::Point_;
using cv::Mat_;
using cv::Mat;
using cv::imread;
using cv::Size;

namespace StatModel {

//! Image and image related operations.
class ModelImage
{
protected:
    /// The number of landmark points.
    int nMarkPoints;

    /// Optional 'host image' filename including full path.
    string hostImageName;


    /// Is the image loaded?
    bool imgLoaded;

    /// Information about shapes and paths.
    ShapeInfo *shapeInfo;


public:
    /// Landmark points
    vector< Point_< int > > points;

    /// Shape vectors
    ShapeVec shapeVec;

    /// the training image
    Mat imgdata;

    /// Load training image from saved host image
    bool loadTrainImage();

    /// Load training image
    bool loadTrainImage(const Mat &img);

    void setShapeInfo(ShapeInfo *si){ shapeInfo = si; }

    /// Release Training Image
    bool releaseTrainImage();

    //! Get the image saved.
    Mat & getTrainImage();

    /// Host image (if any).
    inline const string &HostImage() const { return hostImageName; }

    void buildFromShapeVec(SimilarityTrans &trans);

    /// Set the host image.
    void setHostImage( const char * hostImageFilename ) {
        hostImageName = hostImageFilename;
    }

    /// The number of shape points.
    inline int NPoints() const { return nMarkPoints; }

    /// Read mark points information from a PTS file
    bool readPTS( const char * filename );

    /// Set mark points information from a vector of points
    void initPointsByVector( const vector< cv::Point2i > &V);

    //! Show the image interactively
    Mat show(int level = 0, int pId = -1, bool showInWin = true, int highLight = 0);

    ModelImage();

private:
    bool tryReadIBugPts(const char* filename);
    bool tryReadOldPts(const char* filename);
};

} // Namespace
#endif // MODELIMAGE_H
