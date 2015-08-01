#ifndef __MODEL_HH__
#define __MODEL_HH__

#include<gl3w.h>
#include<vector>

class model{
private:
  std::vector<GLfloat> m_vertices;
  std::vector<GLuint> m_indices;
  std::vector<GLfloat> m_texture;
  GLuint m_VAO;
  GLuint m_VBO;
  GLuint m_EBO;
  GLuint m_tex;
  
public:
//somehow handle additional attributes like color
  template<typename vertItr,
  typename texItr = std::vector<GLuint>::iterator,
  typename idxItr = std::vector<GLfloat>::iterator>
  model(vertItr firstVert, vertItr lastVert, bool hasColor = false,
        idxItr firstIdx = idxItr(), idxItr lastIdx = idxItr(),
        texItr firstTex = texItr(), texItr lastTex = texItr());
  ~model();
  
  void render(GLuint prog);
};

#include"model.cc"

#endif
