#ifndef __ENTITY_HH__
#define __ENTITY_HH__

//causes recursive class definitions
//#include"sceneNode.hh"
#include<tuple>
#include"resourceManager.hh"
class sceneNode;

class entity{
private:
  GLuint m_vao;
  resourceManager::material m_mat;
  resourceManager::mesh m_mesh;
  sceneNode* m_parent = nullptr;
  
  void attach(sceneNode* pNode);
  
  friend sceneNode;
  
public:
  entity(std::tuple<resourceManager::mesh, resourceManager::material> res, bool hasNormal = false, bool hasColor = false);
  entity(resourceManager::mesh mes, resourceManager::material mat, bool hasNormal = false, bool hasColor = false);
  
  void render();
};

#endif
