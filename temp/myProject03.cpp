#include <iostream>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include <osgDB/ReadFile>
#include <osg/BlendFunc>
#include <osg/Camera>
#include <osg/Material>
#include <osgGA/GUIEventHandler>
#include <osgUtil/LineSegmentIntersector>
#include <osgViewer/Viewer>
#include <osgUtil/SmoothingVisitor>
#include <osgUtil/Simplifier>



class ModelController : public osgGA::GUIEventHandler
{
public:
    ModelController( osg::MatrixTransform* node ) : _model(node) {}
    ModelController( osg::Camera* cam) : _cam(cam) {}

    virtual bool handle( const osgGA::GUIEventAdapter&  ea,
                               osgGA::GUIActionAdapter& aa );

protected:
    osg::ref_ptr<osg::MatrixTransform> _model;
    osg::ref_ptr<osg::Camera> _cam;
};

bool ModelController::handle(const osgGA::GUIEventAdapter &ea,
                                   osgGA::GUIActionAdapter &aa)
{
    if ( !_model ) return false;
    osg::Matrix matrix = _model->getMatrix();

    switch ( ea.getEventType() )
    {
        case osgGA::GUIEventAdapter::KEYDOWN:
        switch ( ea.getKey() )
        {
        case 'a': case 'A':
            matrix *= osg::Matrix::rotate( -0.1f, osg::Z_AXIS );
            break;
        case 'd': case 'D':
            matrix *= osg::Matrix::rotate(  0.1f, osg::Z_AXIS );
            break;
        case 'w': case 'W':
            matrix *= osg::Matrix::rotate( -0.1f, osg::X_AXIS );
            break;
        case 's': case 'S':
            matrix *= osg::Matrix::rotate(  0.1f, osg::X_AXIS );
            break;
        default:
            break;
        }
        _model->setMatrix( matrix );
        break;
    default:
        break;
    }
    return false;
}

class PickHandler : public osgGA::GUIEventHandler
{
public:
    osg::Node* getOrCreateSelectionBox()
    {
        if ( !_selectionBox )
        {
            osg::ref_ptr<osg::Geode> geode = new osg::Geode;
            geode->addDrawable(
                new osg::ShapeDrawable(new osg::Box(osg::Vec3(), 1.0f)) );

            _selectionBox = new osg::MatrixTransform;
            _selectionBox->setNodeMask( 0x1 );
            _selectionBox->addChild( geode.get() );

            osg::StateSet* ss = _selectionBox->getOrCreateStateSet();
            ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
            ss->setAttributeAndModes( new osg::PolygonMode(
                osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE) );
        }
        return _selectionBox.get();
    }

    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
    {
        if ( ea.getEventType()!=osgGA::GUIEventAdapter::RELEASE ||
             ea.getButton()!=osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON ||
             !(ea.getModKeyMask()&osgGA::GUIEventAdapter::MODKEY_CTRL) )
            return false;

        osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
        if ( viewer )
        {
            osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
                new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
            osgUtil::IntersectionVisitor iv( intersector.get() );
            iv.setTraversalMask( ~0x1 );
            viewer->getCamera()->accept( iv );

            if ( intersector->containsIntersections() )
            {
                const osgUtil::LineSegmentIntersector::Intersection& result =
                    *(intersector->getIntersections().begin());

                osg::BoundingBox bb = result.drawable->getBound();
                osg::Vec3 worldCenter = bb.center() * osg::computeLocalToWorld(result.nodePath);
                _selectionBox->setMatrix(
                    osg::Matrix::scale(bb.xMax()-bb.xMin(), bb.yMax()-bb.yMin(), bb.zMax()-bb.zMin()) *
                    osg::Matrix::translate(worldCenter) );
            }
        }
        return false;
    }

protected:
    osg::ref_ptr<osg::MatrixTransform> _selectionBox;
};

// Class which used to setup node fast
// take file name and name to set on
class myModel{
public:
    myModel( std::string  modelName , std::string name )
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
    void setSimplifer();
    virtual ~myModel() {}

private:
    osg::ref_ptr<osg::Node>             _model;
    osg::ref_ptr<osg::MatrixTransform>  _mt ;
    osgUtil::SmoothingVisitor           _sv;
    osg::StateSet*                      _stateset;
    osg::ref_ptr<osg::BlendFunc>        _blendFunc;
    osg::ref_ptr<osg::StateSet>         _nodess;
    osg::ref_ptr<osg::Material>         _nodeMaterial;
    osg::ref_ptr<osg::Group>            _root;
    osg::ref_ptr<PickHandler>           _picker;
    osg::ref_ptr<ModelController>       _ctrler;
    osgViewer::Viewer                   _viewer;

};
bool myModel::setMatrix(const osg::Vec3& vec)
{
    _mt->setMatrix(osg::Matrix::translate(vec));
    return true;
}
osg::MatrixTransform* myModel::getMatrix()
{
    return _mt;
}
void myModel::setSmooth()
{
    _model->accept( _sv );
}
void myModel::setBlending()
{
    _blendFunc = new osg::BlendFunc;
    _stateset = _model->getOrCreateStateSet();
    _blendFunc->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    _stateset->setAttributeAndModes( _blendFunc );
}
void myModel::setMaterial()
{
    _nodess = _model->getOrCreateStateSet();
    _nodeMaterial = new osg::Material;
    _nodeMaterial->setDiffuse( osg::Material::FRONT, osg::Vec4(0.5f,0.5f,0.5f,1.0f));
    _nodeMaterial->setAmbient( osg::Material::FRONT, osg::Vec4(2.0f,0.0f,0.0f,1.0f) );
    _nodeMaterial->setTransparency(osg::Material::FRONT , 0.25f);
    _nodess->setAttribute( _nodeMaterial.get() );
}
osg::MatrixTransform* myModel::start()
{
    myModel::setSmooth();
    myModel::setSimplifer();
    myModel::setBlending();
    myModel::setMaterial();
    return _mt;
}
void myModel::setSimplifer()
{
    // search for automatic simplifer
}


class myCamera{
public:
    myCamera() : _camera( new osg::Camera ) {}
    osg::Camera* setCamera();

protected:
    osg::ref_ptr<osg::Camera> _camera;
};
osg::Camera* myCamera::setCamera()
{
    _camera->setClearMask( GL_DEPTH_BUFFER_BIT );
    _camera->setRenderOrder( osg::Camera::POST_RENDER );
    _camera->setReferenceFrame( osg::Camera::ABSOLUTE_RF );
    _camera->setViewMatrixAsLookAt( osg::Vec3d(-1.0f,0.0f,0.0f) , osg::Vec3d(0.0,0.0,0.0) , osg::Vec3d(0.0f,1.0f,0.0f) );
    return _camera;
}



int main()
{
    using namespace osg;
    // create object name of model which take file and name
    myModel model1 ("Neuron.obj","Neuron01");
    model1.start();
    model1.setMatrix(osg::Vec3( 10.0f,0.0f, 0.0f));

    myModel model2 ("Neuron.obj","Neuron02");
    model2.start();
    model2.setMatrix(osg::Vec3(-10.0f,0.0f, 0.0f));

    myModel model3 ("Neuron.obj","Neuron03");
    model3.start();
    model3.setMatrix(osg::Vec3( 0.0f,0.0f,-10.0f));

    // create camera
    myCamera dCamera;
    osg::ref_ptr<osg::Camera> camera = dCamera.setCamera();

    // create root node
    ref_ptr<Group> root = new Group;
    // add child to it
    root->addChild( model1.getMatrix() );
    root->addChild( model2.getMatrix() );
    root->addChild( model3.getMatrix() );
    root->addChild( camera.get() );

    ref_ptr<PickHandler> picker = new PickHandler;
    root->addChild( picker->getOrCreateSelectionBox() );

    //ref_ptr<ModelController> ctrler = new ModelController( mt.get() );

    // create viewer to display data
    osgViewer::Viewer viewer;
    viewer.addEventHandler( picker.get() );
    //viewer.addEventHandler( ctrler.get() );
    viewer.setSceneData( root.get() );
    return viewer.run();
}
