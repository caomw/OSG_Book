#include <osg/Program>

#ifndef SHADER_H
#define SHADER_H

class shader
{
public:
    shader();
    osg::Program* getShader();

protected:
    osg::ref_ptr<osg::Program> _program;
};

#endif // SHADER_H
