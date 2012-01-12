CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT( asmlib-opencv )
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
FIND_PACKAGE( OpenCV REQUIRED )
SET(LIB_SRC_LIST util.cpp modelfile.cpp asmmodel.cpp shapevec.cpp similaritytrans.cpp afreader.cpp modelimage.cpp  shapemodel.cpp)

#INCLUDE_DIRECTORIES( ${OPENCV_INCLUDE_DIR} )
OPTION(ENABLE_ANNOTATOR "Compile the GUI point annotator (Qt Needed)" OFF)

IF (ENABLE_ANNOTATOR)
	ADD_SUBDIRECTORY (annotation)
ENDIF (ENABLE_ANNOTATOR)

ADD_LIBRARY( asm STATIC ${LIB_SRC_LIST} )

ADD_SUBDIRECTORY(test)
