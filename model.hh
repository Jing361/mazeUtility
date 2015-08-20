#ifndef __MODEL_HH__
#define __MODEL_HH__

#include<gl3w.h>
#include<glm/glm.hpp>
#include<vector>
#include<string>

struct Material{
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
};

class model{
private:
  std::vector<GLfloat> m_vertices;
  std::vector<GLuint> m_indices;
  std::vector<GLuint> m_textures;
  GLuint m_VAO;
  GLuint m_VBO;
  GLuint m_EBO;
  unsigned int m_nVert;
  
  glm::mat4 m_transform;
  Material m_mat;
  
public:
//somehow handle additional attributes like color
  template<typename vertItr,
  typename idxItr = std::vector<GLfloat>::iterator,
  typename texItr = std::vector<std::string>::iterator>
  model(vertItr firstVert, vertItr lastVert,
        bool hasColor = false, bool hasNormal = false,
        idxItr firstIdx = idxItr(), idxItr lastIdx = idxItr(),
        texItr firstTex = texItr(), texItr lastTex = texItr());
  ~model();
  
  void render(GLuint prog);
  Material& getMaterial();
  void translate(double x, double y, double z);
  void rotate(float r, double x, double y, double z);
  void scale(double x, double y, double z);
};

#include"model.cc"

#endif
