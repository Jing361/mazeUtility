#include<sstream>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"model.hh"

#include<iostream>

/*template<typename vertItr,
typename idxItr,
typename texItr,
typename specItr>
model::model(vertItr firstVert, vertItr lastVert,
      bool hasColor, bool hasNormal,
      idxItr firstIdx, idxItr lastIdx,
      texItr firstTex, texItr lastTex,
      specItr firstSpec, specItr lastSpec):
  m_vertices(firstVert, lastVert),
  m_indices(firstIdx, lastIdx){
  unsigned int stride = 3;
  unsigned int attr = 0;
  unsigned int offset = 0;
  if(hasColor){
    stride += 3;
  }
  if(hasNormal){
    stride += 3;
  }
  if(firstTex != lastTex || firstSpec != lastSpec){
    stride += 2;
  }
  m_nVert = m_vertices.size() / stride;
  
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  if(m_indices.size() > 0){
    glGenBuffers(1, &m_EBO);
  }
  
  glBindVertexArray(m_VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);
  
  if(m_indices.size() > 0){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);
  }

  //position
  {
    unsigned int nData = 3;
    glVertexAttribPointer(attr, nData, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)offset);
    glEnableVertexAttribArray(attr);
    offset += nData;
  }
  ++attr;
  
  //color
  if(hasColor){
    unsigned int nData = 3;
    glVertexAttribPointer(attr, nData, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(offset * sizeof(GLfloat)));
    glEnableVertexAttribArray(attr);
    offset += nData;
  }
  ++attr;
  
  //texture coodinates
  if(firstTex != lastTex || firstSpec != lastSpec){
    unsigned int nData = 2;
    glVertexAttribPointer(attr, nData, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(offset * sizeof(GLfloat)));
    glEnableVertexAttribArray(attr);
    offset += nData;
  }
  ++attr;
  
  //normal vector
  if(hasNormal){
    unsigned int nData = 3;
    glVertexAttribPointer(attr, nData, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(offset * sizeof(GLfloat)));
    glEnableVertexAttribArray(attr);
    offset += nData;
  }
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  //setup textures
  for(auto it = firstTex; it != lastTex; ++it){
    int width, height;
    unsigned char* image;
    GLuint tex;
    
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    image = SOIL_load_image((*it).c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    m_textures.push_back(tex);
  }
  //setup specmaps
  for(auto it = firstSpec; it != lastSpec; ++it){
    int width, height;
    unsigned char* image;
    GLuint tex;
    
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    image = SOIL_load_image((*it).c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    m_specMaps.push_back(tex);
  }
}*/


void model::cleanUp(){
  glDeleteVertexArrays(1, &m_VAO);
  glDeleteBuffers(1, &m_VBO);
  if(m_indices.size() > 0){
    glDeleteBuffers(1, &m_EBO);
  }
}

void model::render(GLuint prog){
  //Put textures in memory
  unsigned int i = 0;
  for(auto it = m_textures.begin(); it != m_textures.end(); ++it){
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, *it);
    glUniform1i(glGetUniformLocation(prog, "material.diffuse"), i);
    ++i;
  }
  for(auto it = m_specMaps.begin(); it != m_specMaps.end(); ++it){
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, *it);
    glUniform1i(glGetUniformLocation(prog, "material.specular"), i);
    ++i;
  }
  //put transform matrix in memory
  GLuint transformLoc = glGetUniformLocation(prog, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m_transform));
  
  if(m_specMaps.size() > 0){
    GLint matShineLoc = glGetUniformLocation(prog, "material.shininess"); 
    glUniform1f(matShineLoc, m_shininess);
  }
  
  glBindVertexArray(m_VAO);
  if(m_indices.size() > 0){
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, m_indices.data());
  } else {
    glDrawArrays(GL_TRIANGLES, 0, m_nVert);
  }
  glBindVertexArray(0);
  
}

float& model::getShininess(){
  return m_shininess;
}

void model::translate(double x, double y, double z){
  m_transform = glm::translate(m_transform, glm::vec3(x, y, z));
}

void model::rotate(float r, double x, double y, double z){
  m_transform = glm::rotate(m_transform, glm::radians(r), glm::vec3(x, y, z));
}

void model::scale(double x, double y, double z){
  m_transform = glm::scale(m_transform, glm::vec3(x, y, z));
}
