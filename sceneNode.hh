#ifndef __SCENE_NODE_HH__
#define __SCENE_NODE_HH__

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"light.hh"
#include"entity.hh"

class sceneNode{
private:
  sceneNode* m_parent;
  glm::mat4 m_transform;

public:
  sceneNode(sceneNode* pNode = nullptr);
  
  //can't clean up child? :o
  sceneNode* createChild();
  virtual void attachObject(entity* pEnt, GLuint shader);
  virtual void attachObject(light* pLight, GLuint shader);
  glm::mat4 getTransform();
};

#endif
