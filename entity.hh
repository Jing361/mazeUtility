#ifndef __ENTITY_HH__
#define __ENTITY_HH__

#include"sceneNode.hh"

class entity{
private:
  GLuint m_vao;
  material m_mat;
  mesh m_mesh;
  sceneNode* m_parent = nullptr;
  
  void attach(sceneNode* pNode);
  
  friend sceneNode;
  
public:
  entity(material mat, mesh mes, bool hasNormal = false, bool hasColor = false);
  
  void render();
};

#endif
