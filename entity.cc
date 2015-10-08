#include"entity.hh"

entity::entity(material mat, mesh mes, bool hasNormal, bool hasColor):
  m_mat(mat),
  m_mesh(mes){
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
  
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void entity::render(){
  //Put textures in memory
  unsigned int i = 0;
  glActiveTexture(GL_TEXTURE0 + i);
  glBindTexture(GL_TEXTURE_2D, *it);
  glUniform1i(glGetUniformLocation(prog, "material.diffuse"), i);
  ++i;
  
  glActiveTexture(GL_TEXTURE0 + i);
  glBindTexture(GL_TEXTURE_2D, *it);
  glUniform1i(glGetUniformLocation(prog, "material.specular"), i);
  ++i;
  
  //put transform matrix in memory
  GLuint transformLoc = glGetUniformLocation(prog, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m_transform));
  
  if(m_specMaps.size() > 0){
    GLint matShineLoc = glGetUniformLocation(prog, "material.shininess"); 
    glUniform1f(matShineLoc, m_shininess);
  }
  
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, m_mesh.m_nVert);
  glBindVertexArray(0);
}
