#ifndef __GLGAME_HH__
#define __GLGAME_HH__

#include<gl3w.h>
#include<glfw3.h>
#include<string>
#include<map>
#include<exception>
#include<glm/glm.hpp>
#include"camera.hh"
#include"model.hh"
#include"light.hh"

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
  GLfloat curFrame;
  GLfloat lastFrame;
  unsigned int screenWidth;
  unsigned int screenHeight;
  float fov = 80;
  GLFWwindow* window;
  glm::mat4 view;
  glm::mat4 projection;
  camera cam;
  std::multimap<GLuint, model> models;
  std::multimap<GLuint, light> lights;
  void(*moveCam)(camera&, const float);

public:
  glGame(glm::vec3 position, unsigned int width, unsigned int height, std::string name =  "Untitled");
  ~glGame();
  
  void registerObject(GLuint target, model& obj);
  void registerLight(GLuint target, light& lite);
  void setKeyCallback(void(GLFWwindow*, int, int, int, int));
  void setCursorCallback(void(GLFWwindow*, double, double));
  void setScrollCallback(void(GLFWwindow*, double, double));
  void setCameraCallback(void(camera&, const float));
  void run();
  void loop();
};

#endif
