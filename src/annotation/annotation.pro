# -------------------------------------------------
# Project created by QtCreator 2010-02-23T09:42:00
# -------------------------------------------------
TARGET = annotation
TEMPLATE = app
INCLUDEPATH += /usr/include/opencv
;LIBS += -lcxcore \
    ;-lcv \
    ;-lhighgui \
    ;-lcvaux \
    ;-lml
LIBS += /usr/lib/libopencv_calib3d.so /usr/lib/libopencv_contrib.so /usr/lib/libopencv_core.so /usr/lib/libopencv_features2d.so /usr/lib/libopencv_flann.so /usr/lib/libopencv_gpu.so /usr/lib/libopencv_highgui.so /usr/lib/libopencv_imgproc.so /usr/lib/libopencv_legacy.so /usr/lib/libopencv_ml.so /usr/lib/libopencv_nonfree.so /usr/lib/libopencv_objdetect.so /usr/lib/libopencv_photo.so /usr/lib/libopencv_stitching.so /usr/lib/libopencv_ts.so /usr/lib/libopencv_video.so /usr/lib/libopencv_videostab.so  

SOURCES += main.cpp \
    mainwindow.cpp \
    singlephotoview.cpp \
    pointspaint.cpp \
    photoview.cpp
HEADERS += mainwindow.h \
    singlephotoview.h \
    pointspaint.h \
    photoview.h
FORMS += mainwindow.ui
