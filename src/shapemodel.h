#ifndef SHAPEMODEL_H
#define SHAPEMODEL_H

#include "modelimage.h"
#include "cv.h"
#include "modelfile.h"
#include <vector>
using std::vector;
using cv::PCA;

//! Used for viewing model
struct ModelViewInfo
{
    vector< int > vList;
    int curParam;
    void *pModel;
};


//! ASM/AAM fitting result
struct FitResult{
    //! Parameters for the model
    Mat_< double > params;
    
    //! The similarity transformation needed to recover shape
    SimilarityTrans transformation;
	
	SimilarityTrans appTrans;
};

//! Shape model for training sets.
class ShapeModel
{
public:
    ShapeModel();
    
    //! Save the model into a file
    virtual void saveToFile(ModelFile &file);
    
    //! Load the model from a file
    virtual void loadFromFile(ModelFile &file);
    
    //! Load the model from a given path
    /*!
        \param path the path of the file.
        \param bin whether the file is a binary file or not.
    */
    void load(const string &path, bool bin=false);
    
    //! Save the model to a given path
    //! Load the model from a given path
    /*!
        \param path the path of the file.
        \param bin whether the file is a binary file or not.
    */
    void save(const string &path, bool bin=false);

    //! Align the shapes and build a model.
    void buildModel();
    
    //! File names are stored in the list file
    void readTrainData(const char *listFileName);
    
    //! Load shape information
    void loadShapeInfo(const char *shapeFileName);

    //! Set the level for the image pyramid
    /*!
        \param l Image from level 0 to l will be considered during training
                    and searching.
    */
    void setPyramidLevel(int l){ pyramidLevel = l; }

    //! Project a parameter vector to a shape
    /*!
        \param paramVec parameter vector.
        \param shapeVec the shape corresponding to the parameter vector
    */
    void projectParamToShape(const Mat_<double> & paramVec, ShapeVec &shapeVec);
    
    //! Project a shape to a parameter vector
    /*!
        \param shapeVec the shape corresponding to the parameter vector
        \param paramVec parameter vector.
    */
    void projectShapeToParam(const ShapeVec & shapeVec, Mat_<double> &paramVec);

    void viewShapeModel();
    void viewShapeModelUpdate(ModelViewInfo *pInfo);
    
    void getShapeParam(ModelImage &mi, FitResult &res);
	
	ShapeInfo & getShapeInfo(){ return shapeInfo;}
    
    //! Normalize an parameter vector(0..1)
    Mat_< double > normalizeParam(const Mat_<double> &p){
        Mat_<double> n = p.clone();
        for (int i=0; i<p.rows; i++)
            n(i, 0)/= 3*sqrt(pcaShape->eigenvalues.at<double>(i, 0));
        return n;
    }
    
    //! Reconstruct parameter vector from normalized vector
    Mat_< double > reConFromNorm(const Mat_<double> &p){
        Mat_<double> n = p.clone();
        for (int i=0; i<p.rows; i++)
            n(i, 0)*= 3*sqrt(pcaShape->eigenvalues.at<double>(i, 0));
        return n;        
    }
protected:
    //! level for the image pyramid.
    int pyramidLevel;

    //! Number of landmark points in a image.
    int nMarkPoints;
    
    //! Number of training images.
    int nTrain;

    //! All the images, with labelled markpoints.
    vector<ModelImage> imageSet;
    
    //! Path info for shapes;
    ShapeInfo shapeInfo;

    //! Mean shape after aligning
    ShapeVec meanShape;
    
    //! Refine a parameter vector by clamping.
    void clampParamVec( Mat_< double > &paramVec );
    
    //! PCA model for shapes.
    PCA *pcaShape;
    //! Number of eigen vectors reserved for shape model.
    int nShapeParams;
    
    //! Data for BTSM: \sigma^2
    double sigma2;
    
    //! Data for BTSM: Full \phi
    PCA *pcaFullShape;

private:
    //! Build PCA model for shapes
    void buildPCA();
    
    //! Align shapes iteratively
    void alignShapes();
    
    //! Find patterns~
    void preparePatterns();
    
protected:
    //! r.y -= r.height*?
    double searchYOffset;
    
    //! r.x -= r.width*?
    double searchXOffset;
    
    //! r.width *= ?
    double searchWScale;
    //! r.height *= ?
    double searchHScale;

    //! step: ?*100/sqrt(area)
    double searchStepAreaRatio;

    //! init scale ratio when searching
    double searchScaleRatio;
    //! init X offset when searching
    double searchInitXOffset;
    //! init Y offset when searching
    double searchInitYOffset;
};

#endif // SHAPEMODEL_H
