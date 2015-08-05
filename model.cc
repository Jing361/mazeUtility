#include<iostream>
#include"SOIL.h"

template<typename vertItr,
typename idxItr>
model::model(vertItr firstVert, vertItr lastVert, bool hasColor,
      idxItr firstIdx, idxItr lastIdx,
      const char* imageFile):
  m_vertices(firstVert, lastVert),
  m_indices(firstIdx, lastIdx),
  m_hasTexture(imageFile != 0){
  unsigned int stride = 3;
  unsigned int attr = 0;
  if(hasColor){
    stride += 3;
  }
  if(m_hasTexture){
    stride += 2;
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);
  }

  //position
  glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(attr);
  ++attr;
  //color
  if(hasColor){
    glVertexAttribPointer(attr, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(attr);
  }
  ++attr;
  
  if(m_hasTexture){
    glVertexAttribPointer(attr, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(attr);
  }
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  if(m_hasTexture){
    int width, height;
    unsigned char* image;
    
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    image = SOIL_load_image(imageFile, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

model::~model(){
  glDeleteVertexArrays(1, &m_VAO);
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_EBO);
}

void model::render(GLuint prog){
  if(m_hasTexture){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glUniform1i(glGetUniformLocation(prog, "ourTexture"), 0);
  }
  glBindVertexArray(m_VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  //glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
}
