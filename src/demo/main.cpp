// vim: ci:ts=4:sw=4:et
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
using std::cerr;
using std::endl;

void buildASMModel(ASMModel & asmModel,
                   string shapeDef, string ptsList, string modelPath)
{
    ModelFileAscii mf;
    asmModel.loadShapeInfo((shapeDef).c_str());
    asmModel.readTrainData((ptsList).c_str());
    asmModel.buildModel();

    mf.openFile(modelPath.c_str(), "wb");
    asmModel.saveToFile(mf);
    mf.closeFile();
}

void readASMModel(ASMModel & asmModel, string modelPath)
{
    ModelFileAscii mf;
    mf.openFile(modelPath.c_str(), "rb");
    asmModel.loadFromFile(mf);
    mf.closeFile();
}

void showHelp()
{
    cerr << "Arguments: " << endl
        << "===========" << endl
        << "Tasks: " << endl
        << "   -b : Build a model!" << endl
        << "   -v : View a model." << endl
        << "   -f : Fit images to a model." << endl
        << endl
        << "General options:" << endl
        << "   -m <path_to_model> : Path to the model file." << endl
        << "   -V <number>        : Verbose level." << endl
        << endl
        << "Build specific options:" << endl
        << "   -d <def_file>  : Model definition file, see wiki." << endl
        << "   -l <list_file> : List of labelled points, see wiki." << endl
        << endl
        << "Fitting specific options: " << endl
        << "   -C <detector_xml> : Face/Object detector XML (OpenCV)." << endl
        << "   -p <image>        : Path to an image" << endl
        << "   -pc               : Run on your webcam!" << endl
        << endl
        << "For details and examples, please check: " << endl
        << "  http://code.google.com/p/asmlib-opencv/wiki/RunningDemo" << endl
        << endl;
}

int main(int argc, char *argv[])
{
    string modelFile, modelType, action;
    string ptsDefFile, ptsListFile, picPath;
    string faceCascadePath = "haarcascade_frontalface_alt.xml";
    int verboseL = 0;
    int ch;
    opterr = 0;
    while ((ch = getopt(argc, argv, "cV:p:C:m:t:d:l:a:vbf?")) != EOF) {
        switch (ch) {
          // General options:
          case 'm':
              // The path of model file
              modelFile = optarg;
              break;
          case 'V':
              // Verbose level.
              verboseL = strlen(optarg);
              break;

          // Actions:
          case 'v':
              // View a model.
              action = "view";
              break;
          case 'b':
              // Build a model.
              action = "build";
              break;
          case 'f':
              // Use the model to fit images.
              action = "fit";
              break;

          // Options related to build a model:
          case 'd':
              // Points definition file
              ptsDefFile = optarg;
              break;
          case 'l':
              // A list of PTS files.
              ptsListFile = optarg;
              break;


          // Options to fit the model to an image:
          case 'C':
              // Face detector xml
              faceCascadePath = optarg;
              break;
          case 'p':
              // Path for the picture to fit
              picPath = optarg;
              break;
        }
    }

    if (action == "") {
        showHelp();
        cerr << "!!! You have to specify a task!" << endl << endl;
        exit(1);
    }

    if (modelFile == "") {
        showHelp();
        cerr << "!!! You have to specify a model file!" << endl << endl;
        exit(1);
    }

    ASMModel asmModel;
    if (action == "build") {
        if (ptsDefFile == "") {
            showHelp();
            cerr << "!!! def file is necessary for building a model!" << endl;
            exit(1);
        }
        if (ptsListFile == "") {
            showHelp();
            cerr << "!!! list file is necessary for building a model!" << endl;
            exit(1);
        }
        buildASMModel(asmModel, ptsDefFile, ptsListFile, modelFile);
    } else {
        readASMModel(asmModel, modelFile);
        if (action == "view") {
            asmModel.viewShapeModel();
            cvWaitKey();
        } else if (action == "fit") {
            if (picPath == "") {
                showHelp();
                cerr << "You need to either specify an image or webcam mode"
                     << endl;
                exit(1);
            }

            Mat img, imgT;
            vector < FitResult > fitResult;
            cv::CascadeClassifier faceCascade;
            if (!faceCascade.load(faceCascadePath)) {
                showHelp();
                cerr << "Now, a (face) detector is needed. "
                     << "Please check the URL above."
                     << endl;
                exit(1);
            }
            if (picPath == "c") {
                cv::VideoCapture capture;
                capture.open(0);
                if (!capture.isOpened()) {
                    cerr << "Failed to open your webcam." << endl;
                    exit(2);
                }
                while (cvWaitKey(5) == -1) {
                    capture >> imgT;
                    cv::flip(imgT, img, 1);
                    asmModel.fit(img, fitResult, faceCascade, true,
                                 verboseL);
                    asmModel.showResult(img, fitResult);
                }
            } else {
                img = cv::imread(picPath);
                if (img.empty()) {
                    cerr << "Load image '" << picPath << "' failed." << endl;
                    exit(2);
                }
                asmModel.fit(img, fitResult, faceCascade, true, verboseL);
                asmModel.showResult(img, fitResult);
                cvWaitKey();
            }
        }
    }
    return 0;
}
