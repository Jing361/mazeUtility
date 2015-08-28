#ifndef __GLGAME_HH__
#define __GLGAME_HH__

#include<gl3w.h>
#include<glfw3.h>
#include<string>
#include<exception>

class glInitException:public std::exception{
private:
  std::string message;
  
public:
  glInitException(std::string msg):
    message(msg){
  }
  
  std::string what(){
    return message;
  }
};

class glGame{
private:
  bool keys[1024];
  GLfloat xoffset;
  GLfloat yoffset;
  float lastX;
  float lastY;
  GLfloat curFrame;
  GLfloat lastFrame;
  GLFWwindow* window;
  std::string versionInfo;
  
public:
  glGame(unsigned int width, unsigned int height, std::string name =  "Untitled");
};

#endif
