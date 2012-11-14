#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osg/BlendFunc>
#include <osg/Material>
#include <osg/Program>
#include <osgGA/GUIEventHandler>
#include <osgUtil/SmoothingVisitor>
#include <osg/Camera>
#include <osgViewer/Viewer>

#include "shader.h"

#ifndef SCENE_H
#define SCENE_H

class scene
{
public:
    scene( std::string  modelName , std::string name )
    {
        _model = osgDB::readNodeFile(modelName);
        _model->setName(name);
        _mt =  new osg::MatrixTransform;
        _mt->addChild( _model.get() );
    }
    bool setMatrix(const osg::Vec3& vec);
    osg::MatrixTransform* getMatrix();
    void setSmooth();
    void setBlending();
    void setMaterial();
    osg::MatrixTransform* start();
    osg::MatrixTransform* start(osg::Camera *camera);
    void setSimplifer();
    void getShader();
    void setShader();
    virtual ~scene() {}

private:
    osg::ref_ptr<osg::Node>             _model;
    osg::ref_ptr<osg::MatrixTransform>  _mt ;
    osgUtil::SmoothingVisitor           _sv;
    osg::StateSet*                      _stateset;
    osg::ref_ptr<osg::BlendFunc>        _blendFunc;
    osg::ref_ptr<osg::StateSet>         _nodess;
    osg::ref_ptr<osg::Material>         _nodeMaterial;
    osg::ref_ptr<osg::Group>            _root;
    osg::Program*                       _program;
    osg::Camera*                         _camera;
    osgViewer::Viewer                   _viewer;
};

#endif // SCENE_H
