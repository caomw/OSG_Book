#include "camera.h"

osg::Camera* camera::setCamera()
{
    _camera->setClearMask( GL_DEPTH_BUFFER_BIT );
    _camera->setRenderOrder( osg::Camera::POST_RENDER );
    _camera->setReferenceFrame( osg::Camera::ABSOLUTE_RF );
    _camera->setViewMatrixAsLookAt( osg::Vec3d(-1.0f,0.0f,0.0f) , osg::Vec3d(0.0,0.0,0.0) , osg::Vec3d(0.0f,1.0f,0.0f) );
    return _camera;
}
