# -------------------------------------------------
# Project created by QtCreator 2010-02-23T09:42:00
# -------------------------------------------------
TARGET = annotation
TEMPLATE = app
INCLUDEPATH += /usr/include/opencv
LIBS += -lcxcore \
    -lcv \
    -lhighgui \
    -lcvaux \
    -lml
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
