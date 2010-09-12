#include "modelimage.h"
#include "afreader.h"
//#include "delaunay.h"

ModelImage::ModelImage()
{
    //this->points.push_back(Point_<double>(1,1));
    imgLoaded = false;
    this->shapeInfo = NULL;
}

bool ModelImage::ReadAF(const char *filename)
{
    AFReader r(filename);

    if ( !r.IsValid() ) {
        return false;
    }

    int npoints, path_id, type, point_nb, from, to;
    float x_rel, y_rel;

    // read nb points
    r.Sync();
    fscanf( r.FH(), "%i", &npoints );
    printf("Points: %d\n", npoints);

    // resize this shape
    this->points.resize(npoints);
//    pointInfo.resize(npoints);
    this->shapeVec.create(npoints * 2, 1);

    // read all point data
    r.Sync();
    int nelem = 0;
    float user1, user2, user3;
    for(int i=0;i<npoints;i++) {
        // read point data
        // format: <path#> <type> <x rel.> <y rel.> <point#> <connects from> <connects to>
        fscanf( r.FH(), "%i %i %f %f %i %i %i", &path_id, &type, &x_rel, &y_rel, &point_nb, &from, &to );

        if ( r.MoreNonWhiteSpaceOnLine() ) {
            nelem = fscanf( r.FH(), "%f %f %f", &user1, &user2, &user3 );
        }

        r.Sync();

        // save point data
        //SetPoint( i, x_rel, y_rel );
        points[i].x = x_rel;
        points[i].y = y_rel;
        shapeVec(i, 0) = x_rel;
        shapeVec(i+npoints, 0) = y_rel;
    }

    // read host image
    r.Sync();
    char buf[300]={0};
    fscanf( r.FH(), "%s", buf );
    puts(buf);
    //this->hostImage = string(buf);
//
//    // we're now hold relative coordinates
//    m_bAbsPointCoordinates = false;

    return true;
}

bool ModelImage::readPTS( const char * filename )
{
    AFReader r(filename);

    if ( !r.IsValid() ) {
		printf("File %s not found!\n");
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
//    pointInfo.resize(npoints);
    //this->shapeVec.create(npoints * 2, 1);

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
//     this->genShapeVecFromPoints();


    // File name of the host image is the same as the name pts file
    string sFname(filename);
    this->hostImageName = sFname.substr(0, sFname.rfind('.'))+".jpg";
    FILE *fp = fopen(hostImageName.data(),"rb");
    if (!fp)
        this->hostImageName = sFname.substr(0, sFname.rfind('.'));
    else
        fclose(fp);
//     printf("%s\n", hostImageName.data());
//    // we're now hold relative coordinates
//    m_bAbsPointCoordinates = false;

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
        loadTrainImage(imread(this->hostImageName));
    }
    return imgLoaded;
}

void ModelImage::buildFromShapeVec(SimilarityTrans& trans)
{
    nMarkPoints = shapeVec.nPoints();
    shapeVec.restoreToPointList(points, trans);
}


bool ModelImage::loadTrainImage(const Mat &img)
{
    imgdata = img;
//         imshow("a", img);
//         cvWaitKey(0);
    cv::buildPyramid(imgdata, this->imgPyramid, 3);
    if (imgdata.channels() == 3){
        this->imgPyrGray.resize(4);
        for (int i=0; i<=3; i++)
            cv::cvtColor(imgPyramid[i], imgPyrGray[i], CV_BGR2GRAY);
    }
    else {
        this->imgPyrGray.resize(4);
        for (int i=0; i<=3; i++)
            imgPyrGray[i] = imgPyramid[i];
    }
    imgPyrGrayDeriv.resize(4);
    for (int i=0; i<=3; i++){
        imgPyrGray[i].convertTo(imgPyrGrayDeriv[i], CV_64F);
        cv::Sobel(imgPyrGrayDeriv[i], imgPyrGrayDeriv[i], CV_64F, 1, 1);
    }
    this->imgLoaded = true;
    return imgLoaded;
}

bool ModelImage::releaseTrainImage()
{
    if (imgLoaded){
        imgdata.release();
        for (int i=0; i<=3; i++){
            imgPyramid[i].release();
            imgPyrGray[i].release();
            imgPyrGrayDeriv[i].release();
        }
        imgLoaded = false;
    }
    return !imgLoaded;
}

Mat_< double > ModelImage::getLocalStruct(int pId, int k, int level, double step)
{
    this->loadTrainImage();
    vector< Point_< int > > pV;
//     this->getPointsOnNorm(pId, k+1, level, pV);
//     Mat_< double > v(2*k+3, 1), diffV(2*k+1, 1);
    
//     for (int i=k+1;i>=-k-1;i--){
//         v(i+(k+1), 0) = this->imgPyrGray[level](pV[i+(k+1)]);
//     }
//     double absSum = 0;
//     for (int i=k;i>=-k;i--){
//         diffV(i+k, 0) = (v(i+1+(k+1), 0) - v(i-1+(k+1), 0));
//         absSum += fabs(diffV(i+k, 0));
//     }
//     diffV *= 1/absSum;
    this->getPointsOnNorm(pId, k, level, pV, step);
    Mat_< double > diffV(2*k+1, 1);
    double absSum = 0;
    for (int i=k;i>=-k;i--){
        diffV(i+k, 0) = imgPyrGrayDeriv[level](pV[i+k]);
        absSum += fabs(diffV(i+k, 0));
    }
    if (absSum==0){
        printf("Warning: absSum=0....Level: %d, pID: %d\n", level, pId);
//         show(level, pId);
    }
    else
        diffV *= 1/absSum;

    return diffV;
}

Mat & ModelImage::getTrainImage(int level, bool gray){
    if (gray)
        return imgPyrGray[level];
    else
        return this->imgPyramid[level];
}

void ModelImage::getPointsOnNorm(int pId, int k, int level,
                            vector< Point_< int > > &V, double step,
                            int pOffset)
{
    // Find the norm direction
    Point_< double > va, vb, vDirection;
    va = points[shapeInfo->pointInfo[pId].connectFrom] - points[pId];
    vb = points[shapeInfo->pointInfo[pId].connectTo] - points[pId];
    double td;
    td = norm(va);
    if (td>1e-10) va *= 1 / td;
    td = norm(vb);
    if (td>1e-10) vb *= 1 / td;
    vDirection.x = - va.y + vb.y;
    vDirection.y = va.x - vb.x;
    if (norm(vDirection)<1e-10){
        if (norm(va)>1e-10)
            vDirection = - va;
        else
            vDirection.x = 1, vDirection.y = 0;
    }
    else
        vDirection *= 1 / norm(vDirection);

    int i, j;
    int nx, ny;
    int offsetX, offsetY;
    int prevX, prevY;
    
    // Find the center point, here step===1
    prevX = 0;
    prevY = 0;
    nx = ny = 0;
    j = 1;
    for (i=1; i<=abs(pOffset);i++){
        do {
            nx = cvRound(j*vDirection.x);
            ny = cvRound(j*vDirection.y);
            j++;
        } while (nx == prevX && ny == prevY);

        prevX = nx;
        prevY = ny;
    }
    j--;
//     printf("%d, %d\n", nx, ny);
    if (pOffset>0)
        offsetX = nx, offsetY = ny;
    else
        offsetX = -nx, offsetY = -ny;
//     offsetX = offsetY = 0;
    
    // Apply the "step", and find points
    vDirection *= step;
    prevX = 0;
    prevY = 0;
    nx = ny = 0;

    // Test best j
    j = 1;
    for (i=1; i<=k;i++){
        do {
            nx = cvRound(j*vDirection.x);
            ny = cvRound(j*vDirection.y);
            j++;
        } while (nx == prevX && ny == prevY);

        prevX = nx;
        prevY = ny;
    }
    j--;

    V.resize(2*k+1);
    int rX, rY;
    for (i=k;i>=-k;i--){
        rX = (points[pId].x>>level) + nx + offsetX;
        rY = (points[pId].y>>level) + ny + offsetY;
        if (rX<0) rX = 0;
        if (rY<0) rY = 0;
        if (rX >= (imgdata.cols>>level)) rX = (imgdata.cols>>level) - 1;
        if (rY >= (imgdata.rows>>level)) rY = (imgdata.rows>>level) - 1;
        V[i+k] = Point_< int >(rX, rY);
        //v(i+(k+1), 0) = this->imgdata.at< char >(nx, ny);
        do {
            nx = cvRound(j*vDirection.x);
            ny = cvRound(j*vDirection.y);
            j--;
        } while (nx == prevX && ny == prevY);
        prevX = nx;
        prevY = ny;
    }
}

Mat ModelImage::show(int l, int pId, bool showInWin, int highLight)
{
    Mat mb;// = curSearch.getTrainImage().clone();
    if (imgPyramid[0].channels()==1)
        cv::cvtColor(imgPyramid[0], mb, CV_GRAY2RGB);
    else
        mb = imgPyramid[0].clone();
    
    for (int i=0;i<nMarkPoints;i++){
//         printf("%d\n", i);
        cv::circle(mb, points[i], 3, cv::Scalar(25, 50, 255), 1, CV_AA);
    }
    for (int i=0;i<shapeInfo->nPath;i++){
        for (int j=shapeInfo->pathSeg[i]; j<shapeInfo->pathSeg[i+1]; j++){
            cv::line(mb, points[shapeInfo->pointInfo[j].connectFrom],
                     points[j],
                     cv::Scalar(200, 30, 80), 1, CV_AA);
        }
    }

    for (int i=0;i<nMarkPoints;i++){
        if (pId==-1 || pId!=i)
            continue;
        vector< Point_< int > > pV;
        // Fixme: the step 1.7
        getPointsOnNorm(i, 4, l, pV, 2, highLight);
        for (int j=0; j<9; j++)
            if (highLight==100/*j-3*/){
                cv::circle(mb, Point_< int >(pV[j].x<<l, pV[j].y<<l), 3, CV_RGB(230, 100, 50),
                       1, CV_AA);
            }
            else
                cv::circle(mb, Point_< int >(pV[j].x<<l, pV[j].y<<l), 3, CV_RGB(50, 230, 100),
                       1, CV_AA);
            //cv::line(mb, Point_< int >(pV[j].x<<l, pV[j].y<<l),
            //         Point_< int >(pV[j].x<<l, pV[j].y<<l), cv::Scalar(50, 230, 100));
//            mb.at< cv::Vec3b >(pV[j]) = cv::Vec3b(50, 230, 100);
    }
    if (showInWin){
        cvNamedWindow("hoho", CV_WINDOW_AUTOSIZE);
        cv::imshow("hoho", mb);
        while (1){
            unsigned char cc = cv::waitKey(100);
    //                printf("%d\n", cc);
            if (cc=='a')
                break;
        }
    }
    return mb;
}
