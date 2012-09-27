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



macro( config_project PROJNAME LIBNAME )
    include_directories( ${${LIBNAME}_INCLUDE_DIR} )
    target_link_libraries( ${PROJNAME} ${${LIBNAME}_LIBRARY} )
endmacro()


add_executable( testing main.cpp )
config_project( testing OPENTHREADS )
config_project( testing OSG )
config_project( testing OSGDB )
config_project( testing SOGSHADOW )
config_project( testing OSGUTIL )
config_project( testing OSGGA )
config_project( testing OSGTEXT )
config_project( testing OSGVIEWER )
