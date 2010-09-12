PROJECT( asmlib-opencv )
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
FIND_PACKAGE( OpenCV REQUIRED )
SET(LIB_SRC_LIST util.cpp modelfile.cpp asmmodel.cpp shapevec.cpp similaritytrans.cpp afreader.cpp modelimage.cpp  shapemodel.cpp)

#add_subdirectory(imgwarp)

#INCLUDE_DIRECTORIES( ${OPENCV_INCLUDE_DIR} imgwarp/ )
INCLUDE_DIRECTORIES( ${OPENCV_INCLUDE_DIR} )

ADD_LIBRARY( asm STATIC ${LIB_SRC_LIST} )
#TARGET_LINK_LIBRARIES( aam imgwarp ${OPENCV_LIBRARIES} )
TARGET_LINK_LIBRARIES( asm ${OPENCV_LIBRARIES} )

add_subdirectory(test)