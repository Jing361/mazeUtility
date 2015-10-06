#ifndef __RESOURCE_MANAGER_HH__
#define __RESOURCE_MANAGER_HH__

#include<string>
#include<map>
#include<exception>

class invalideFileNameException:public std::exception{
private:
  std::string message;
  
public:
  invalideFileNameException(std::string msg):
    message(msg){
  }
  
  std::string what(){
    return message;
  }
};

typedef struct{
  GLuint m_diffMap;
  GLuint m_specMap;
  float m_shininess;
} material;

class resourceManager{
private:
  std::map<std::string, material> m_materials;
  std::map<std::string, GLuint> m_meshes;
  
  void acquireMaterial(std::string name, std::string file, float shine);
  void acquireMesh(std::string name, std::string file);
  static GLint acquireTexture(std::string file);
  static void checkFile(std::string fileName);
  
public:
  ~resourceManager();

  void acquire(std::string name, std::string file, float shine = 32.0);
};

#endif
