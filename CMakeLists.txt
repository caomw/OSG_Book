cmake_minimum_required( VERSION 2.6 )
project( testing )

find_package( OpenThreads )
find_package( osg )
find_package( osgDB )
find_package( osgShadow )
find_package( osgUtil )
find_package( osgGA )
find_package( osgText )
find_package( osgViewer )
find_package( GLUT )
find_package( OpenGL )


INCLUDE_DIRECTORIES(common)
SET(COMMON_FILES
    ${CMAKE_SOURCE_DIR}/common/CommonFunctions.cpp
    ${CMAKE_SOURCE_DIR}/common/CommonFunctions
)


FIND_PACKAGE(Qt)
IF(QT4_FOUND)
    SET(EXAMPLE_NAME cookbook_09_01)
    SET(EXAMPLE_FILES ch09_01/osg_qt.cpp)
    SET(EXTERNAL_INCLUDE_DIR "${QT_INCLUDE_DIR}")
    SET(EXTERNAL_LIBRARIES ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}
                           "debug;osgQt${CMAKE_DEBUG_POSTFIX};optimized;osgQt")
ENDIF(QT4_FOUND)


# my classes
add_library(math scene light shader camera)

macro( config_project PROJNAME LIBNAME )
    include_directories( ${${LIBNAME}_INCLUDE_DIR} )
    target_link_libraries( ${PROJNAME} ${${LIBNAME}_LIBRARY} )
endmacro()


add_executable( testing main.cpp ${COMMON_FILES})
config_project( testing OPENTHREADS )
config_project( testing OSG )
config_project( testing OSGDB )
config_project( testing OSGSHADOW )
config_project( testing OSGUTIL )
config_project( testing OSGGA )
config_project( testing OSGTEXT )
config_project( testing OSGVIEWER )
include_directories( ${OPENGL_INCLUDE_DIR} )

#linking my class
target_link_libraries(testing math)
