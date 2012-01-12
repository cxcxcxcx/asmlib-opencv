#include "asmmodel.h"
#include "modelfile.h"
#include "util.h"
#ifdef __GNUG__
#include <unistd.h>
#else
#include "XGetopt.h"
#endif

#include <cstdio>
#include <string>
#include "highgui.h"
using std::string;
using cv::imshow;

void labelBlock_FaceWhole(ModelImage &mSrc, ModelImage &mDst,
                          Mat_<int> &mLblSrc, Mat_<int> &mLblDst, int &nBlock)
{
    Mat imgSrc, imgDst;
    imgSrc = mSrc.getTrainImage();
    imgDst = mDst.getTrainImage();
    mLblDst.create(imgDst.size());
    mLblSrc.create(imgSrc.size());
    mLblDst.setTo(0);
    mLblSrc.setTo(0);
    fillConvexFromPoints(mLblDst, mDst.points, 1);
    fillConvexFromPoints(mLblSrc, mSrc.points, 1);
    nBlock = 2;
}

void buildASMModel(ASMModel &asmModel,
                   string shapeDef, string ptsList,
                   string modelPath)
{
    ModelFileAscii mf;
    asmModel.loadShapeInfo((shapeDef).c_str());
    asmModel.readTrainData((ptsList).c_str());
    asmModel.buildModel();

    mf.openFile(modelPath.c_str(),"wb");
    asmModel.saveToFile(mf);
    mf.closeFile();
}

void readASMModel(ASMModel &asmModel, string modelPath){
    ModelFileAscii mf;
    mf.openFile(modelPath.c_str(),"rb");
    asmModel.loadFromFile(mf);
    mf.closeFile();
}

int main(int argc, char *argv[])
{
		printf("How to run this?\n Please check: http://code.google.com/p/asmlib-opencv/wiki/RunningDemo\n");
    string modelFile, modelType, action;
    string ptsDefFile, ptsListFile, picPath;
    string faceCascadePath = "haarcascade_frontalface_alt.xml";
    bool colored = false;
    int verboseL=0;
    int ch;
    opterr=0;
    while ((ch=getopt(argc,argv,"cV:p:C:m:t:d:l:a:vbf?"))!=EOF){
        switch (ch){
            case 'm':
                // The path of model file
                modelFile=optarg;
                break;
            case 't':
                // Model type
                modelType=optarg;
                break;
            case 'd':
                // Points definition file
                ptsDefFile=optarg;
                break;
            case 'l':
                ptsListFile=optarg;
                break;
            case 'C':
                // Face detector xml
                faceCascadePath = optarg;
                break;
            case 'c':
                colored = true;
                break;
            case 'p':
                // Path for the picture to fit
                picPath = optarg;
                break;
            case 'a':
                action = optarg;
                break;
            case 'b':
                action = "build";
                break;
            case 'v':
                action = "view";
                break;
            case 'V':
                verboseL = strlen(optarg);
                break;
            case 'f':
                action = "fit";
                break;
        }
    }

    ASMModel asmModel;
    if (action=="build"){
        buildASMModel(asmModel, ptsDefFile, ptsListFile, modelFile);
    }
    else{
        readASMModel(asmModel, modelFile);
        if (action=="view"){
            asmModel.viewShapeModel();
            cvWaitKey();
        }
        else if (action=="fit"){
            Mat img, imgT;
            vector< FitResult > fitResult;
            cv::CascadeClassifier faceCascade;
            faceCascade.load(faceCascadePath);
            if (picPath == "c"){
                cv::VideoCapture capture;
                capture.open(0);
//                     printf("Opened: %d\n", capture.isOpened());
                while (cvWaitKey(5)==-1){
                    capture>>imgT;
                    cv::flip(imgT, img, 1);
//                         imshow("img", img);
//                         cvWaitKey();
                    asmModel.fit(img, fitResult, faceCascade, true, verboseL);
                    asmModel.showResult(img, fitResult);
                }
//                     img = imgT.clone();
            }
            else {
                img = cv::imread(picPath);
                asmModel.fit(img, fitResult, faceCascade, true, verboseL);
                asmModel.showResult(img, fitResult);
                cvWaitKey();
            }

//                 imshow("img", img);
        }
    }
    return 0;
}
