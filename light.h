#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include <vector>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/MatrixTransform>

class light
{
public:
    enum LIGHTNUMBER{
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT
    };
    light();
    void init(osg::Group* root);
    osg::Light* getOrCreateLight(light::LIGHTNUMBER number);

    light(int lightnum);
    bool setMatrixTrans(const osg::Vec3& mat );
    osg::Node* getMatrixTrans(const osg::Vec3 &trans);
    bool setDiffuse(const osg::Vec4& diffuse);
    bool setAmbient(const osg::Vec4& ambient);

protected:
    osg::Group* lightGroup;
    std::vector<osg::LightSource*> lights;
    bool valid;

    osg::Light* _light;
    osg::ref_ptr<osg::LightSource> _lightsource;
    osg::ref_ptr<osg::MatrixTransform> _sourceTrans;
};

#endif // LIGHT_H
