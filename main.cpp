#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgUtil/SmoothingVisitor>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgUtil/PrintVisitor>
#include <iostream>
#include <osg/Group>
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

class  CcolorVisitor : public osg::NodeVisitor {
public :
    CcolorVisitor() : NodeVisitor( NodeVisitor::TRAVERSE_ALL_CHILDREN ) {
    // ---------------------------------------------------------------
    //
    // Default Ctors overide the default node visitor mode so all
    // children are visited
    //
    // ---------------------------------------------------------------
        //
        // Default to a white color
        //
        m_color.set( 1.0, 1.0, 1.0, 1.0 );
        m_colorArrays = new osg::Vec4Array;
        m_colorArrays->push_back( m_color );
        };

    CcolorVisitor( const osg::Vec4 &color ) : NodeVisitor( NodeVisitor::TRAVERSE_ALL_CHILDREN ) {
    // -------------------------------------------------------------------
    //
    // Overloaded Ctor initialised with the Color
    // Also override the visitor to traverse all the nodes children
    //
    // -------------------------------------------------------------------
        m_color = m_color;
        m_colorArrays = new osg::Vec4Array;
        m_colorArrays->push_back( m_color );
    };

    virtual ~CcolorVisitor(){};

    virtual void apply ( osg::Node &node ){
    // --------------------------------------------
    //
    //  Handle traversal of osg::Node node types
    //
    // --------------------------------------------
    traverse( node );
    } // apply( osg::Node &node )

    virtual void apply( osg::Geode &geode ){
    // ------------------------------------------------
    //
    //  Handle traversal of osg::Geode node types
    //
    // ------------------------------------------------
    osg::StateSet *state   = NULL;
    unsigned int    vertNum = 0;
    //
    //  We need to iterate through all the drawables check if
    //  the contain any geometry that we will need to process
    //
    unsigned int numGeoms = geode.getNumDrawables();

    for( unsigned int geodeIdx = 0; geodeIdx < numGeoms; geodeIdx++ ) {
        //
        // Use 'asGeometry' as its supposed to be faster than a dynamic_cast
        // every little saving counts
        //
       osg::Geometry *curGeom = geode.getDrawable( geodeIdx )->asGeometry();
        //
        // Only process if the drawable is geometry
        //
        if ( curGeom ) {
           osg::Vec4Array *colorArrays = dynamic_cast< osg::Vec4Array *>(curGeom->getColorArray());
           if ( colorArrays ) {
               for ( unsigned int i = 0; i < colorArrays->size(); i++ ) {
                    osg::Vec4 *color = &colorArrays->operator [](i);
                    //
                    // could also use *color = m_color
                    //
                    color->set( m_color._v[0], m_color._v[1], m_color._v[2], m_color._v[3]);
                    }
                }
                else{
                    curGeom->setColorArray( m_colorArrays.get());
                    curGeom->setColorBinding( osg::Geometry::BIND_OVERALL );
                    }
                }
            }
    } // apply( osg::Geode

    void setColor( const float r, const float g, const float b, const float a = 1.0f ){
    // -------------------------------------------------------------------
    //
    // Set the color to change apply to the nodes geometry
    //
    // -------------------------------------------------------------------
        osg::Vec4 *c = &m_colorArrays->operator []( 0 );
        m_color.set( r,g,b,a );
        *c = m_color;
       } // setColor( r,g,b,a )

    void setColor( const osg::Vec4 &color  ){
    // -------------------------------------------------------------------
    //
    // Set the color to change apply to the nodes geometry
    //
    // -------------------------------------------------------------------
        osg::Vec4 *c = &m_colorArrays->operator []( 0 );
        m_color = color;
        *c = m_color;
       } // setColor( vec4 )


private :
    osg::Vec4 m_color;
    osg::ref_ptr< osg::Vec4Array > m_colorArrays;
 }; // class CcolorVisitor


osg::ref_ptr<osg::Geometry> geom ;

class InfoVisitor : public osg::NodeVisitor
{
public:
    InfoVisitor():_level (0)
    { setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);}
    std::string spaces()
    {
        return std::string(_level*2 ,' ' );
    }
    virtual void apply( osg::Node& node );
    virtual void apply( osg::Geode& geode );
protected:
    unsigned int _level ;
};

void InfoVisitor::apply( osg::Node& node )
{
    std::cout << spaces() << node.libraryName() << ": :"
    << node.className() << std::endl ;

    _level++;
    traverse( node );
    _level--;
}

void InfoVisitor::apply( osg::Geode& geode ){
    std::cout << spaces() << geode.libraryName() << "::"
                 << geode.className() << std::endl;
    _level++;
    for ( unsigned int i = 0 ; i < geode.getNumDrawables() ; ++i )
    {
        osg::Drawable* drawable = geode.getDrawable(i);
        std::cout << spaces() << drawable->libraryName() << "::"
                     << drawable->className() << std::endl;
        geom = drawable->asGeometry();
    }

    traverse( geode );
    _level--;
}



int main( int argc, char** argv )
{
    using namespace osg;
    // create viewer to display data
    osgViewer::Viewer viewer;

    // create object name of model which take file and name
    scene model1 ("Neuron.obj","Neuron01");
    model1.setMatrix(osg::Vec3( 10.0f,0.0f, 0.0f));
    model1.start();

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild( model1.getMatrix() );
    //root->addChild(createLightGroup(root,1,pos));

    viewer.setSceneData( root.get() );
    //Stats Event Handler s key
    viewer.addEventHandler(new osgViewer::StatsHandler);
    return viewer.run();
}
