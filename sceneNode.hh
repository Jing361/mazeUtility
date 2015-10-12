#ifndef __SCENE_NODE_HH__
#define __SCENE_NODE_HH__

#include"light.hh"
#include"entity.hh"

class sceneNode{
private:
  sceneNode* m_parent;
  glm::mat4 m_transform;

public:
  sceneNode(sceneNode* pNode = nullptr);
  
  sceneNode* createChild();
  virtual void attachObject(entity* pEnt, GLuint shader);
  virtual void attachObject(light* pLight, GLuint shader);
};

#endif
