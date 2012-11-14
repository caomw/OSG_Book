#include <iostream>
#include <osg/PolygonMode>
#include <osgText/Font>
#include <osgViewer/View>
#include <osg/AnimationPath>
#include <osg/Texture>
#include <osg/Camera>
#include <osgGA/GUIEventHandler>
#include <osgText/Text>
#include <osgUtil/LineSegmentIntersector>

//acording to file
#include <osg/ShapeDrawable>
#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

namespace osgCookBook
{

    extern osg::AnimationPathCallback* createAnimationPathCallback( float radius, float time );
    extern osg::Camera* createRTTCamera( osg::Camera::BufferComponent buffer, osg::Texture* tex, bool isAbsolute=false );
    extern osg::Camera* createHUDCamera( double left, double right, double bottom, double top );
    extern osg::Geode* createScreenQuad( float width, float height, float scale=1.0f );
    extern osgText::Text* createText( const osg::Vec3& pos, const std::string& content, float size );

    extern float randomValue( float min, float max );
    extern osg::Vec3 randomVector( float min, float max );
    extern osg::Matrix randomMatrix( float min, float max );

    class PickHandler : public osgGA::GUIEventHandler
    {
    public:
        virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
        virtual void doUserOperations( osgUtil::LineSegmentIntersector::Intersection& result ) = 0;
    };

    osg::ref_ptr<osgText::Font> g_font = osgText::readFontFile("fonts/arial.ttf");

        osg::AnimationPathCallback* createAnimationPathCallback( float radius, float time )
        {
            osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
            path->setLoopMode( osg::AnimationPath::LOOP );

            unsigned int numSamples = 32;
            float delta_yaw = 2.0f * osg::PI/((float)numSamples - 1.0f);
            float delta_time = time / (float)numSamples;
            for ( unsigned int i=0; i<numSamples; ++i )
            {
                float yaw = delta_yaw * (float)i;
                osg::Vec3 pos( sinf(yaw)*radius, cosf(yaw)*radius, 0.0f );
                osg::Quat rot( -yaw, osg::Z_AXIS );
                path->insert( delta_time * (float)i, osg::AnimationPath::ControlPoint(pos, rot) );
            }

            osg::ref_ptr<osg::AnimationPathCallback> apcb = new osg::AnimationPathCallback;
            apcb->setAnimationPath( path.get() );
            return apcb.release();
        }

        osg::Camera* createRTTCamera( osg::Camera::BufferComponent buffer, osg::Texture* tex, bool isAbsolute )
        {
            osg::ref_ptr<osg::Camera> camera = new osg::Camera;
            camera->setClearColor( osg::Vec4() );
            camera->setClearMask( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
            camera->setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );
            camera->setRenderOrder( osg::Camera::PRE_RENDER );
            if ( tex )
            {
                tex->setFilter( osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR );
                tex->setFilter( osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR );
                camera->setViewport( 0, 0, tex->getTextureWidth(), tex->getTextureHeight() );
                camera->attach( buffer, tex );
            }

            if ( isAbsolute )
            {
                camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
                camera->setProjectionMatrix( osg::Matrix::ortho2D(0.0, 1.0, 0.0, 1.0) );
                camera->setViewMatrix( osg::Matrix::identity() );
                camera->addChild( createScreenQuad(1.0f, 1.0f) );
            }
            return camera.release();
        }

        osg::Camera* createHUDCamera( double left, double right, double bottom, double top )
        {
            osg::ref_ptr<osg::Camera> camera = new osg::Camera;
            camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
            camera->setClearMask( GL_DEPTH_BUFFER_BIT );
            camera->setRenderOrder( osg::Camera::POST_RENDER );
            camera->setAllowEventFocus( false );
            camera->setProjectionMatrix( osg::Matrix::ortho2D(left, right, bottom, top) );
            camera->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
            return camera.release();
        }

        osg::Geode* createScreenQuad( float width, float height, float scale )
        {
            osg::Geometry* geom = osg::createTexturedQuadGeometry(
                osg::Vec3(), osg::Vec3(width,0.0f,0.0f), osg::Vec3(0.0f,height,0.0f),
                0.0f, 0.0f, width*scale, height*scale );
            osg::ref_ptr<osg::Geode> quad = new osg::Geode;
            quad->addDrawable( geom );

            int values = osg::StateAttribute::OFF|osg::StateAttribute::PROTECTED;
            quad->getOrCreateStateSet()->setAttribute(
                new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL), values );
            quad->getOrCreateStateSet()->setMode( GL_LIGHTING, values );
            return quad.release();
        }

        osgText::Text* createText( const osg::Vec3& pos, const std::string& content, float size )
        {
            osg::ref_ptr<osgText::Text> text = new osgText::Text;
            text->setDataVariance( osg::Object::DYNAMIC );
            text->setFont( g_font.get() );
            text->setCharacterSize( size );
            text->setAxisAlignment( osgText::TextBase::XY_PLANE );
            text->setPosition( pos );
            text->setText( content );
            return text.release();
        }

        float randomValue( float min, float max )
        {
            return (min + (float)rand()/(RAND_MAX+1.0f) * (max - min));
        }

        osg::Vec3 randomVector( float min, float max )
        {
            return osg::Vec3( randomValue(min, max),
                              randomValue(min, max),
                              randomValue(min, max) );
        }

        osg::Matrix randomMatrix( float min, float max )
        {
            osg::Vec3 rot = randomVector(-osg::PI, osg::PI);
            osg::Vec3 pos = randomVector(min, max);
            return osg::Matrix::rotate(rot[0], osg::X_AXIS, rot[1], osg::Y_AXIS, rot[2], osg::Z_AXIS) *
                   osg::Matrix::translate(pos);
        }

        bool PickHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
        {
            if ( ea.getEventType()!=osgGA::GUIEventAdapter::RELEASE ||
                 ea.getButton()!=osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON ||
                 !(ea.getModKeyMask()&osgGA::GUIEventAdapter::MODKEY_CTRL) )
                return false;
            osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
            if ( viewer )
            {
                osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
                    new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
                osgUtil::IntersectionVisitor iv( intersector.get() );
                viewer->getCamera()->accept( iv );

                if ( intersector->containsIntersections() )
                {
                    osgUtil::LineSegmentIntersector::Intersection result = *(intersector->getIntersections().begin());
                    doUserOperations( result );
                }
            }
            return false;
        }

}



osg::MatrixTransform* createTransformNode( osg::Drawable* shape, const osg::Matrix& matrix )
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable( shape );

    osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform;
    trans->addChild( geode.get() );
    trans->setMatrix( matrix );
    return trans.release();
}

osg::AnimationPathCallback* createWheelAnimation( const osg::Vec3& base )
{
    osg::ref_ptr<osg::AnimationPath> wheelPath = new osg::AnimationPath;
    wheelPath->setLoopMode( osg::AnimationPath::LOOP );
    wheelPath->insert( 0.0, osg::AnimationPath::ControlPoint(base, osg::Quat()) );
    wheelPath->insert( 0.01, osg::AnimationPath::ControlPoint(
        base + osg::Vec3(0.0f, 0.02f, 0.0f), osg::Quat(osg::PI_2, osg::Z_AXIS)) );
    wheelPath->insert( 0.02, osg::AnimationPath::ControlPoint(
        base + osg::Vec3(0.0f,-0.02f, 0.0f), osg::Quat(osg::PI, osg::Z_AXIS)) );

    osg::ref_ptr<osg::AnimationPathCallback> apcb = new osg::AnimationPathCallback;
    apcb->setAnimationPath( wheelPath.get() );
    return apcb.release();
}

int main( int argc, char** argv )
{
    // Shapes
    osg::ref_ptr<osg::ShapeDrawable> mainRodShape =
        new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(), 0.4f, 10.0f) );
    osg::ref_ptr<osg::ShapeDrawable> wheelRodShape =
        new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(), 0.4f, 8.0f) );
    osg::ref_ptr<osg::ShapeDrawable> wheelShape =
        new osg::ShapeDrawable( new osg::Cylinder(osg::Vec3(), 2.0f, 1.0f) );
    osg::ref_ptr<osg::ShapeDrawable> bodyShape =
        new osg::ShapeDrawable( new osg::Box(osg::Vec3(), 6.0f, 4.0f, 14.0f) );

    // The scene graph
    osg::MatrixTransform* wheel1 = createTransformNode(
        wheelShape.get(), osg::Matrix::translate(0.0f, 0.0f,-4.0f) );
    wheel1->addUpdateCallback( createWheelAnimation(osg::Vec3(0.0f, 0.0f,-4.0f)) );

    osg::MatrixTransform* wheel2 = createTransformNode(
        wheelShape.get(), osg::Matrix::translate(0.0f, 0.0f, 4.0f) );
    wheel2->addUpdateCallback( createWheelAnimation(osg::Vec3(0.0f, 0.0f, 4.0f)) );

    osg::MatrixTransform* wheelRod1 = createTransformNode( wheelRodShape.get(),
        osg::Matrix::rotate(osg::Z_AXIS, osg::X_AXIS) * osg::Matrix::translate(0.0f, 0.0f,-5.0f) );
    wheelRod1->addChild( wheel1 );
    wheelRod1->addChild( wheel2 );

    osg::MatrixTransform* wheelRod2 = static_cast<osg::MatrixTransform*>( wheelRod1->clone(osg::CopyOp::SHALLOW_COPY) );
    wheelRod2->setMatrix( osg::Matrix::rotate(osg::Z_AXIS, osg::X_AXIS) * osg::Matrix::translate(0.0f, 0.0f, 5.0f) );

    osg::MatrixTransform* body = createTransformNode(
        bodyShape.get(), osg::Matrix::translate(0.0f, 2.2f, 0.0f) );

    osg::MatrixTransform* mainRod = createTransformNode( mainRodShape.get(), osg::Matrix::identity() );
    mainRod->addChild( wheelRod1 );
    mainRod->addChild( wheelRod2 );
    mainRod->addChild( body );

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild( mainRod );

    // Start the viewer
    osgViewer::Viewer viewer;
    viewer.setSceneData( root.get() );
    return viewer.run();
}
