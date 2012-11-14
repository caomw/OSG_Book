#include "light.h"

light::light() : lightGroup(NULL),lights(8),valid(false){
    for(int i = 0 ; i < lights.size() ; i++)
    {
        lights[i] = NULL;
    }
}

light::light(int lightnum)
{
    _light = new osg::Light(lightnum);
    //_light->setDiffuse();
    //_light->setPosition();
    //_light->setAmbient();
    _lightsource = new osg::LightSource;
    _lightsource->setLight( _light );
}

void light::init(osg::Group* root)
{
    lightGroup = root;
    valid = true;
}


osg::Light* light::getOrCreateLight(light::LIGHTNUMBER number)
{
    if(valid)
    {
        if(lights[number])
        {
        }
        osg::ref_ptr<osg::Light> light = new osg::Light();
        light->setLightNum(number);

        osg::LightSource* lightSource = new osg::LightSource();
        lightSource->setLight( light.get() );

        lightSource->setStateSetModes(*lightGroup->getOrCreateStateSet(),osg::StateAttribute::ON);
        lightGroup->addChild(lightSource);

        return light.release();
    }
    return NULL;
}


bool light::setDiffuse(const osg::Vec4 &diffuse)
{
    try{
         _light->setDiffuse(diffuse);
         return 1;
    }
    catch(int e){
        std::cout<<"Can't load light diffuse error: "<< e << std::endl;
        return 0;
    }
}

bool light::setAmbient(const osg::Vec4 &ambient)
{
    try
    {
        _light->setAmbient(ambient);
        return 1;
    }
    catch(int e){
        std::cout<<"Can't load light ambient error: "<< e << std::endl;
        return 0;
    }
}

bool light::setMatrixTrans(const osg::Vec3 &trans)
{
    try{
        _sourceTrans = new osg::MatrixTransform;
        _sourceTrans->setMatrix( osg::Matrix::translate(trans));
        _sourceTrans->addChild( _lightsource.get() );
        return 1;
    }
    catch (int e){
        std::cout<<"Can't set Matrix error: "<< e << std::endl;
        return 0;
    }
}

osg::Node* light::getMatrixTrans(const osg::Vec3 &trans)
{
    setMatrixTrans(trans);
    return _sourceTrans.release();
}
