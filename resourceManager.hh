#ifndef __RESOURCE_MANAGER_HH__
#define __RESOURCE_MANAGER_HH__

#include<string>
#include<map>
#include<exception>
#include<tuple>
#include<gl3w.h>
//#include"renderer.hh"
class renderer;

class invalideFileNameException:public std::exception{
private:
  std::string message;
  std::string fileName;
  
public:
  invalideFileNameException(std::string msg):
    message(msg){
  }
  
  std::string what(){
    return message;
  }
};

class invalidResourceException:public std::exception{
private:
  std::string message;
  std::string resourceName;
  
public:
  invalidResourceException(std::string msg):
    message(msg){
  }
  
  std::string what(){
    return message;
  }
};

class invalidRendererException:public std::exception{
private:
  std::string message;

public:
  invalidRendererException(std::string msg):
    message(msg){
  }
  
  std::string what(){
    return message;
  }
};

class resourceManager{
public:
  typedef struct{
    GLuint m_diffMap;
    GLuint m_specMap;
    float m_shininess;
  } material;

  typedef struct{
    GLuint m_vbo;
    int m_nVert;
  } mesh;
private:
  std::map<std::string, material> m_materials;
  std::map<std::string, mesh> m_meshes;
  
  void acquireMaterial(std::string name, std::string file, float shine);
  void acquireMesh(std::string name, std::string file);
  static GLint acquireTexture(std::string file);
  static bool checkFile(std::string fileName);
  
public:  
  resourceManager(renderer* renderer);
  ~resourceManager();

  void acquire(std::string name, std::string file, float shine = 32.0);
  material getMaterial(std::string name);
  mesh getMesh(std::string name);
  std::tuple<mesh, material> getResources(std::string name);
};

#endif
