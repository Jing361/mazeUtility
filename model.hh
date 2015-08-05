#ifndef __MODEL_HH__
#define __MODEL_HH__

#include<gl3w.h>
#include<vector>
#include<string>

class model{
private:
  std::vector<GLfloat> m_vertices;
  std::vector<GLuint> m_indices;
  std::vector<GLuint> m_textures;
  GLuint m_VAO;
  GLuint m_VBO;
  GLuint m_EBO;
  
public:
//somehow handle additional attributes like color
  template<typename vertItr,
  typename idxItr = std::vector<GLfloat>::iterator,
  typename texItr = std::vector<std::string>::iterator>
  model(vertItr firstVert, vertItr lastVert, bool hasColor = false,
        idxItr firstIdx = idxItr(), idxItr lastIdx = idxItr(),
        texItr firstTex = texItr(), texItr lastTex = texItr());
  ~model();
  
  void render(GLuint prog);
};

#include"model.cc"

#endif
