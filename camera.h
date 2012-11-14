#include <osg/Camera>

#ifndef CAMERA_H
#define CAMERA_H

class camera
{

public:
    camera(): _camera( new osg::Camera ) {}
    osg::Camera* setCamera();

protected:
    osg::ref_ptr<osg::Camera> _camera;
};

#endif // CAMERA_H
