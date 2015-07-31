#include<iostream>

template<typename vertItr, typename idxItr>
model::model(vertItr firstVert, vertItr lastVert, bool hasColor, idxItr firstIdx, idxItr lastIdx):
  m_vertices(firstVert, lastVert),
  m_indices(firstIdx, lastIdx){
  
  unsigned int stride;
  if(hasColor){
    stride = 6;
  } else {
    stride = 3;
  }
  
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  if(firstIdx != lastIdx){
    glGenBuffers(1, &m_EBO);
  }
  
  glBindVertexArray(m_VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);
  
  if(firstIdx != lastIdx){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
  }

  //position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  //color
  if(hasColor){
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
  }
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  for(auto it = firstVert; it != lastVert; ++it){
    std::cout << *it << std::endl;
  }
}

model::~model(){
  glDeleteVertexArrays(1, &m_VAO);
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_EBO);
}

void model::render(){
  glBindVertexArray(m_VAO);
  //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
}
