#include"light.hh"

light::light(glm::vec3 position, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec):
  m_position(position),
  m_ambient(amb),
  m_diffuse(diff),
  m_specular(spec){
}

void light::getUniforms(GLuint prog){
  GLint lightPosLoc      = glGetUniformLocation(prog, "light.position");
  GLint lightAmbientLoc  = glGetUniformLocation(prog, "light.ambient");
  GLint lightDiffuseLoc  = glGetUniformLocation(prog, "light.diffuse");
  GLint lightSpecularLoc = glGetUniformLocation(prog, "light.specular");
  
  glUniform3f(lightPosLoc, m_position.x, m_position.y, m_position.z);
  glUniform3f(lightAmbientLoc,  m_ambient.r, m_ambient.g, m_ambient.b);
  glUniform3f(lightDiffuseLoc,  m_diffuse.r, m_diffuse.g, m_diffuse.b);
  glUniform3f(lightSpecularLoc, m_specular.r, m_specular.g, m_specular.b);
}
