#include"light.hh"

light::light(glm::vec3 position, glm::vec3 color):
  m_position(position),
  m_color(color){
}

void light::getUniforms(GLuint prog){
  GLint lightPosLoc = glGetUniformLocation(prog, "lightPos");
  glUniform3f(lightPosLoc, m_position.x, m_position.y, m_position.z);
  GLint lightColorLoc = glGetUniformLocation(prog, "lightColor");
  glUniform3f(lightColorLoc, m_color.x, m_color.y, m_color.z);
}
