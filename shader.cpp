#include "shader.h"

shader::shader()
{
    static const char* vertSource = {
        "void main()\n"
        "{\n"
            "gl_Position = ftransform();\n"
        "}\n"
    };

    static const char* fragSource = {
        "void main()\n"
        "{\n"
            "gl_FragColor = vec4(1.);\n"
        "}\n"
    };

    //osg::ref_ptr<osg::Shader> vertShader = new osg::Shader( osg::Shader::VERTEX, vertSource );
    //osg::ref_ptr<osg::Shader> fragShader = new osg::Shader( osg::Shader::FRAGMENT, fragSource );

    osg::ref_ptr<osg::Shader> vertShader = new osg::Shader( osg::Shader::VERTEX, "shader/vertShader.vert" );
    osg::ref_ptr<osg::Shader> fragShader = new osg::Shader( osg::Shader::FRAGMENT, "shader/fragShader.frag" );

    _program = new osg::Program;
    _program->addShader( vertShader.get() );
    _program->addShader( fragShader.get() );
}

osg::Program* shader::getShader()
{
    return _program;
}
