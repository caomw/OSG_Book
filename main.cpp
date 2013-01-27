#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgUtil/SmoothingVisitor>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <iostream>
#include <osg/NodeVisitor>

// including my ca
#include "scene.h"
#include "light.h"
#include "camera.h"
#include "shader.h"

osg::Group* createLightGroup(osg::Group* root,int num,osg::Vec4Array* pos)
{
    osg::LightSource* lightsource[8];
    osg::Light* myLight[8];
    osg::ref_ptr<osg::Group> lightGrp = new osg::Group;
    osg::ref_ptr<osg::StateSet> lightss = root->getOrCreateStateSet();

    osg::ref_ptr<osg::Geode> lightMarkerGeode (new osg::Geode);

    for(int i = 0 ; i < num ; i++)
    {
    lightsource[i] = new osg::LightSource;

    myLight[i] = new osg::Light(i);
    myLight[i]->setPosition((*pos)[i]);
    //myLight[i]->setDiffuse((*pos)[i+2]);
    myLight[i]->setConstantAttenuation(1.0);

    lightsource[i]->setLight(myLight[i]);
    lightsource[i]->setLocalStateSetModes(osg::StateAttribute::ON);
    lightsource[i]->setStateSetModes(*lightss,osg::StateAttribute::ON);

    lightGrp->addChild(lightsource[i]);
    lightMarkerGeode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3f((*pos)[i][0],(*pos)[i][1],(*pos)[i][2]),0.5f)));
    }
    lightGrp->addChild(lightMarkerGeode.get());
    return lightGrp.release();
}

osg::Group* start()
{
    // create object name of model which take file and name
    scene model1 ("Neuron.obj","Neuron01");
    model1.setMatrix(osg::Vec3( 10.0f,0.0f, 0.0f));
    model1.start();

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild( model1.getMatrix() );

    return root.release();
}

int main( int argc, char** argv )
{
    // create viewer to display data
    osgViewer::Viewer viewer;
    viewer.setSceneData( start() );
    //Stats Event Handler s key
    viewer.addEventHandler(new osgViewer::StatsHandler);
    return viewer.run();
}
