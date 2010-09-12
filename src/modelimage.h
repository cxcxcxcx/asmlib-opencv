#ifndef SHAPE_H
#define SHAPE_H
#include <string>
#include <vector>
#include "cv.h"
#include "highgui.h"
#include "shapevec.h"
using std::string;
using std::vector;
using cv::Point_;
using cv::Mat_;
using cv::Mat;
using cv::imread;
using cv::Size;

struct PointInfo
{
    int type;
    int pathId;
    int connectFrom;
    int connectTo;
};

# include "modelfile.h"
struct ShapeInfo
{
    vector< int > pathSeg;
    vector< int > pathType;
    int nPath;

    vector< PointInfo > pointInfo;
    
    void writeToFile(ModelFile &f){
        f.writeInt(nPath);
        for (int i=0;i<nPath+1;i++)
            f.writeInt(pathSeg[i]);
        for (int i=0;i<nPath;i++)
            f.writeInt(pathType[i]);
        f.writeInt(pointInfo.size());
        for (size_t i=0;i<pointInfo.size();i++){
            f.writeInt(pointInfo[i].connectFrom);
            f.writeInt(pointInfo[i].connectTo);
            f.writeInt(pointInfo[i].type);
            f.writeInt(pointInfo[i].pathId);
        }
    }
    
    void readFromFile(ModelFile &f){
        f.readInt(nPath);
        int t;
        pathSeg.resize(nPath+1);
        for (int i=0;i<nPath+1;i++)
            pathSeg[i] = f.readInt(t);
        pathType.resize(nPath);
        for (int i=0;i<nPath;i++)
            pathType[i] = f.readInt(t);
        pointInfo.resize(f.readInt(t));
        for (size_t i=0;i<pointInfo.size();i++){
            f.readInt(pointInfo[i].connectFrom);
            f.readInt(pointInfo[i].connectTo);
            f.readInt(pointInfo[i].type);
            f.readInt(pointInfo[i].pathId);
        }
    }
};

class ModelImage
{
protected:
    /// The number of landmark points.
    int nMarkPoints;

    /// Optional 'host image' filename including full path.
    string hostImageName;

    /// the training image
    Mat imgdata;

    /// Image pyramid
    vector< Mat > imgPyramid;
    vector< Mat_<double> > imgPyrGrayDeriv;
    vector< Mat_<uchar> > imgPyrGray;

    /// Is the image loaded?
    bool imgLoaded;

    /// Information about shapes and paths.
    ShapeInfo *shapeInfo;

public:
    /// Landmark points
    vector< Point_< int > > points;
    
    /// Shape vectors
    ShapeVec shapeVec;

    /// Load training image from saved host image
    bool loadTrainImage();
    
    /// Load training image
    bool loadTrainImage(const Mat &img);

    void setShapeInfo(ShapeInfo *si){ shapeInfo = si; }


    /// Release Training Image
    bool releaseTrainImage();

    /// Get local structure
    /*!
        \param pId id of the point
        \param k how many points to get on either direction
        \param level position in the pyramid level
        \param step VERY IMPORTANT, for a image with area of 10000, 1.0 may be a good choice
    */    
    Mat_< double > getLocalStruct(int pId, int k, int level, double step);

    /// Get the coordinates of points at normal direction of a landmark point
    /*!
        \param pId id of the point
        \param k how many points to get on either direction
        \param level position in the pyramid level
        \param V the vector that save results.
        \param step VERY IMPORTANT, for a image with area of 10000, 1.0 may be a good choice
        \param pOffset when searching for best points, use the p'th  point along the profile as the center
    */
    void getPointsOnNorm(int pId, int k, int level,
                         vector< Point_< int > > &V, 
                         double step, int pOffset=0);

    //! Get the image saved at specified level
    Mat & getTrainImage(int level=0, bool gray=false);
    
    /// Return the derivative image at specified level;
    Mat & getDerivImage(int level){ return imgPyrGrayDeriv[level]; }

    /// Host image (if any).
    inline const string &HostImage() const { return hostImageName; }

    void buildFromShapeVec(SimilarityTrans &trans);
//     void genShapeVecFromPoints();
    
    /// Set the host image.
    void setHostImage( const char * hostImageFilename ) {
        hostImageName = hostImageFilename;
    }

    /// The number of shape points.
    inline int NPoints() const { return nMarkPoints; }

    bool ReadAF( const char * filename );

    /// Read mark points information from a PTS file
    bool readPTS( const char * filename );
    
    /// Set mark points information from a vector of points
    void initPointsByVector( const vector< cv::Point2i > &V);
    
    
    //bool WriteAF( const char * filename, const int image_width, const int image_height );

    
    void delaunayDiv();
    
    //! Show the image interactively
    Mat show(int level = 0, int pId = -1, bool showInWin = true, int highLight = 0);

    ModelImage();
};

#endif // SHAPE_H
