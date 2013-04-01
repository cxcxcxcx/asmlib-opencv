#include "modelimage.h"
#include "afreader.h"
using std::cerr;

namespace StatModel {

ModelImage::ModelImage()
{
    imgLoaded = false;
    this->shapeInfo = NULL;
}

bool ModelImage::readPTS( const char * filename )
{
    AFReader r(filename);

    if ( !r.IsValid() ) {
        printf("File %s not found!\n", filename);
        throw "";
        return false;
    }

    int npoints;

    // read nb points
    r.Sync();
    int c;
    do {
        c=fgetc(r.FH());
    }while (c<'0'||c>'9');
    ungetc(c, r.FH());
    fscanf( r.FH(), "%i", &npoints );
    fgetc(r.FH());

    this->nMarkPoints = npoints;

    // resize this shape
    this->points.resize(npoints);

    // read all point data
    r.Sync();

    // read point data
    int ix, iy;
    int i;
    for(i=0;i<npoints;i++) {
        fscanf( r.FH(), "%d %d", &ix, &iy);
        r.Sync();
        // save point data
        points[i].x = ix;
        points[i].y = iy;
    }
    shapeVec.fromPointList(points);


    // File name of the host image is the same as the name pts file
    string sFname(filename);
    this->hostImageName = sFname.substr(0, sFname.rfind('.'))+".jpg";
    FILE *fp = fopen(hostImageName.data(),"rb");
    if (!fp)
        this->hostImageName = sFname.substr(0, sFname.rfind('.'));
    else
        fclose(fp);

    return true;
}

void ModelImage::initPointsByVector(const std::vector< cv::Point2i >& V)
{
    this->nMarkPoints = V.size();

    this->points = V;
    this->shapeVec.fromPointList(V);
}


bool ModelImage::loadTrainImage()
{
    if (!imgLoaded){
        cerr << this->hostImageName << endl;
        Mat img = imread(this->hostImageName);
        if (img.empty()) {
            cerr << "(EE) Loading image " + this->hostImageName + " failed!" << endl;
            throw;
        }
        loadTrainImage(img);
    }
    return imgLoaded;
}

bool ModelImage::loadTrainImage(const Mat& img)
{
    imgdata = img;
}

void ModelImage::buildFromShapeVec(SimilarityTrans& trans)
{
    nMarkPoints = shapeVec.nPoints();
    shapeVec.restoreToPointList(points, trans);
}


bool ModelImage::releaseTrainImage()
{
    if (imgLoaded){
        imgdata.release();
        imgLoaded = false;
    }
    return !imgLoaded;
}

Mat ModelImage::show(int l, int pId, bool showInWin, int highLight)
{
    Mat mb;
    if (imgdata.channels()==1)
        cv::cvtColor(imgdata, mb, CV_GRAY2RGB);
    else
        mb = imgdata.clone();

    shapeInfo->drawMarkPointsOnImg(mb, points, true);

//     for (int i=0;i<nMarkPoints;i++){
//         if (pId==-1 || pId!=i)
//             continue;
//         vector< Point_< int > > pV;
//         getPointsOnNorm(i, 4, l, pV, 2, highLight);
//         for (int j=0; j<9; j++)
//             if (highLight==100/*j-3*/){
//                 cv::circle(mb, Point_< int >(pV[j].x<<l, pV[j].y<<l), 3, CV_RGB(230, 100, 50),
//                        1, CV_AA);
//             }
//             else
//                 cv::circle(mb, Point_< int >(pV[j].x<<l, pV[j].y<<l), 3, CV_RGB(50, 230, 100),
//                        1, CV_AA);
//     }
    if (showInWin){
        cvNamedWindow("hoho", CV_WINDOW_AUTOSIZE);
        cv::imshow("hoho", mb);

        printf("Press any key to continue...\n");
        cv::waitKey();
    }
    return mb;
}
} // Namespace