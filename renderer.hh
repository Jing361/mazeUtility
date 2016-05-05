#ifndef __RENDERER_HH__
#define __RENDERER_HH__

#include<gl3w.h>
#include<glfw3.h>
#include<string>
#include<map>
#include<exception>
#include<memory>
#include<glm/glm.hpp>
#include"camera.hh"
#include"entity.hh"
#include"light.hh"
#include"spotLight.hh"
#include"sceneManager.hh"

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

class renderer{
private:
  GLfloat curFrame;
  GLfloat lastFrame;
  unsigned int screenWidth;
  unsigned int screenHeight;
  float fov = 80;
  GLFWwindow* window;
  glm::mat4 view;
  glm::mat4 projection;
  camera* cam;
  sceneManager* manager;
  std::multimap<GLint, entity> models;
  std::multimap<GLint, light> lights;
  std::multimap<GLint, spotLight> spots;
  void(*moveCam)(camera&, const float);

public:
  renderer(unsigned int width, unsigned int height, std::string name =  "Untitled");
  ~renderer();
  
  void attachCamera(camera* pCam);
  void attachSceneManager(sceneManager* pMan);
  
  void registerObject(GLint target, entity& obj);
  void registerLight(GLint target, light& lite);
  void registerSpotLight(GLint target, spotLight& lite);
  
  void setKeyCallback(void(GLFWwindow*, int, int, int, int));
  void setCursorCallback(void(GLFWwindow*, double, double));
  void setScrollCallback(void(GLFWwindow*, double, double));
  void setCameraCallback(void(camera&, const float));
  
  void run();
  void loop();
};

#endif
