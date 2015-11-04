#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"sceneNode.hh"
#include"entity.hh"

void entity::attach(sceneNode* pNode){
  m_parent = pNode;
}

entity::entity(std::tuple<resourceManager::mesh, resourceManager::material> res, bool hasNormal, bool hasColor):
  entity(std::get<0>(res), std::get<1>(res), hasNormal, hasColor){
}

entity::entity(resourceManager::mesh mes, resourceManager::material mat, bool hasNormal, bool hasColor):
  m_mesh(mes),
  m_mat(mat){
  unsigned int stride = 3;
  unsigned int attr = 0;
  unsigned int offset = 0;
  
  if(hasColor){
    stride += 3;
  }
  if(hasNormal){
    stride += 3;
  }
  if(mat.m_diffMap != -1 || mat.m_specMap != -1){
    stride += 2;
  }
  m_mesh.m_nVert /= stride;
  
  glGenVertexArrays(1, &m_vao);
  
  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_mesh.m_vbo);
  
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
  if(mat.m_diffMap != -1 || mat.m_specMap != -1){
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
  
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void entity::render(GLuint prog){
  glm::mat4 transform;
  
  if(m_parent){
    transform = m_parent->getTransform();
  }
  
  //Put textures in memory
  unsigned int i = 0;
  glActiveTexture(GL_TEXTURE0 + i);
  glBindTexture(GL_TEXTURE_2D, m_mat.m_diffMap);
  glUniform1i(glGetUniformLocation(prog, "material.diffuse"), i);
  ++i;
  
  glActiveTexture(GL_TEXTURE0 + i);
  glBindTexture(GL_TEXTURE_2D, m_mat.m_specMap);
  glUniform1i(glGetUniformLocation(prog, "material.specular"), i);
  ++i;
  
  GLint matShineLoc = glGetUniformLocation(prog, "material.shininess"); 
  glUniform1f(matShineLoc, m_mat.m_shininess);

  //put transform matrix in memory
  GLuint transformLoc = glGetUniformLocation(prog, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
  
  //should bind vbo, not vao..
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, m_mesh.m_nVert);
  glBindVertexArray(0);
}
