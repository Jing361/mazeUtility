#ifndef __SCENE_NODE_HH__
#define __SCENE_NODE_HH__

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"light.hh"
#include"spotLight.hh"
class entity;

class sceneNode{
private:
  sceneNode* m_parent;
  glm::vec3 m_position;
  glm::mat4 m_transform;

public:
  sceneNode(sceneNode* pNode = nullptr);
  
  //can't clean up child? :o
  sceneNode* createChild();
  void setPosition(glm::vec3 pPosition);
  void translate(double x, double y, double z);
  
  virtual void attachObject(entity* pEnt, GLuint shader);
  virtual void attachPointLight(light* pLight, GLuint shader);
  virtual void attachSpotLight(spotLight* pLight, GLuint shader);
  virtual void attachAmbientLight(glm::vec3 ambientColor, GLuint shader);
  glm::mat4 getTransform();
};

#endif
