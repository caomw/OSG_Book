#include "scene.h"

//static osg::Matrix3 toNormalMatrix( const osg::Matrix& mat )
//{
//    osg::Matrix normVMat = osg::Matrix::orthoNormal(
//    osg::Matrix::transpose( osg::Matrix::inverse( mat ) ) );
//    osg::Matrix3 normalViewMatrix3x3( normVMat(0,0), normVMat(0,1),
//    normVMat(0,2),
//    normVMat(1,0), normVMat(1,1),
//    normVMat(1,2),
//    normVMat(2,0), normVMat(2,1),
//    normVMat(2,2) );
//    return normalViewMatrix3x3;
//}

bool scene::setMatrix(const osg::Vec3& vec)
{
    _mt->setMatrix(osg::Matrix::translate(vec));
    return true;
}
osg::MatrixTransform* scene::getMatrix()
{
    return _mt;
}
void scene::setSmooth()
{
    _model->accept( _sv );
}
void scene::setBlending()
{
    _blendFunc = new osg::BlendFunc;
    _stateset = _model->getOrCreateStateSet();
    _blendFunc->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    _stateset->setAttributeAndModes( _blendFunc );
}
void scene::setMaterial()
{
    _nodess = _model->getOrCreateStateSet();
    _nodeMaterial = new osg::Material;
    _nodeMaterial->setDiffuse( osg::Material::FRONT, osg::Vec4(0.5f,0.5f,0.5f,1.0f));
    _nodeMaterial->setAmbient( osg::Material::FRONT, osg::Vec4(2.0f,0.0f,0.0f,1.0f) );
    _nodeMaterial->setTransparency(osg::Material::FRONT , 0.25f);
    _nodess->setAttribute( _nodeMaterial.get() );
}
osg::MatrixTransform* scene::start()
{
    osg::ref_ptr<osg::Geometry> geom = dynamic_cast<osg::Geometry*>(_model.get());
    //setShader();
    scene::setSmooth();
    scene::setSimplifer();
    scene::setBlending();
    //scene::setMaterial();
    return _mt;
}

osg::MatrixTransform* scene::start(osg::Camera *camera)
{
    osg::ref_ptr<osg::Geometry> geom = dynamic_cast<osg::Geometry*>(_model.get());
    _camera = camera;
    setShader();
    scene::setSmooth();
    scene::setSimplifer();
    scene::setBlending();
    //scene::setMaterial();
    return _mt;
}

void scene::setSimplifer()
{
    // search for automatic simplifer
}

void scene::setShader()
{
    shader _shader;
    _stateset = _model->getOrCreateStateSet();
    _stateset->setAttributeAndModes( _shader.getShader() );
    // for vertexShader
    _stateset->addUniform( new osg::Uniform("LightPosition" ,osg::Vec4(10.0f,10.0f,10.0f,1.0f) ));
    _stateset->addUniform( new osg::Uniform("LightIntensity" ,osg::Vec3(1.0f,1.0f,1.0f) ));

    _stateset->addUniform( new osg::Uniform("Kd" ,osg::Vec3(1.0f,1.0f,1.0f) ));
    _stateset->addUniform( new osg::Uniform("Ka" ,osg::Vec3(1.0f,1.0f,1.0f) ));
    _stateset->addUniform( new osg::Uniform("Ks" ,osg::Vec3(1.0f,1.0f,1.0f) ));
    _stateset->addUniform( new osg::Uniform("Shininess" ,1 ));

    _stateset->addUniform( new osg::Uniform("ModelViewMatrix",_camera->getViewport()));
    //_stateset->addUniform( new osg::Uniform("NormalMatrix", toNormalMatrix( _camera->getViewport())));
    _stateset->addUniform( new osg::Uniform("ProjectionMatrix",_camera->getProjectionMatrix()));



    // for fragShader
    //_stateset->addUniform( new osg::Uniform("BrickColor" , osg::Vec3f(1.0f, 0.0f, 0.0f)));
    //_stateset->addUniform( new osg::Uniform("MortarColor" , osg::Vec3f(0.0f, 0.0f, 1.0f)));
    //_stateset->addUniform( new osg::Uniform("BrickSize" , osg::Vec2f(1.0f, 2.0f)));
    //_stateset->addUniform( new osg::Uniform("BrickPct" , osg::Vec2f(2.0f, 1.0f)));

}

void scene::getShader()
{

}

