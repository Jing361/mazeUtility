#include"shader.hh"
#include<fstream>
#include<iostream>

shader::shader(const GLchar* vertexPath, const GLchar* fragmentPath):
  m_program(glCreateProgram()){
  GLint success;
  GLuint vertexSource = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentSource = glCreateShader(GL_FRAGMENT_SHADER);

  attach(vertexSource, vertexPath);
  attach(fragmentSource, fragmentPath);

  glLinkProgram(m_program);
  glGetProgramiv(m_program, GL_LINK_STATUS, &success);
  if(!success){
    GLchar infoLog[512];
    glGetProgramInfoLog(m_program, 512, NULL, infoLog);
    std::cerr << "ERROR:SHADER:PROGRAM:LINKING_FAILED\n" << infoLog << std::endl;
  }
  
  glDeleteShader(vertexSource);
  glDeleteShader(fragmentSource);
}

void shader::operator()(){
  glUseProgram(m_program);
}

GLuint shader::getTarget(){
  return m_program;
}

std::string shader::loadShader(const GLchar* fileName){
  std::string source;
  std::string line;
  std::ifstream file(fileName);
  
  while(std::getline(file, line)){
    source += line + '\n';
  }
  source += '\0';
  return source;
}

void shader::attach(GLuint shade, const GLchar* path){
  GLint success;
  std::string source = loadShader(path);
  const GLchar* ss = source.c_str();

  glShaderSource(shade, 1, &ss, NULL);
  glCompileShader(shade);
  
  glGetShaderiv(shade, GL_COMPILE_STATUS, &success);
  if(!success){
    GLchar infoLog[512];
    glGetShaderInfoLog(shade, 512, NULL, infoLog);
    std::cerr << "ERROR:SHADER:COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  
  glAttachShader(m_program, shade);
}
