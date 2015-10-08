#ifndef __ENTITY_HH__
#define __ENTITY_HH__

class entity{
private:
  GLuint m_vao;
  material m_mat;
  mesh m_mesh;
  
public:
  entity(material mat, mesh mes, bool hasNormal = false, bool hasColor = false);
  
  void render();
};

#endif
