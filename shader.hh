#ifndef __SHADER_HH__
#define __SHADER_HH__

#include<gl3w.h>
#include<string>

class shader{
private:
  GLuint m_program;
  
  std::string loadShader(const char* fileName);
  void attach(GLuint shade, const GLchar* path);

public:
  shader(const GLchar* vertexPath, const GLchar* fragmentPath);
  
  void operator()();
  GLuint getTarget();
};

#endif
