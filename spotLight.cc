#include"spotLight.hh"

spotLight::spotLight(glm::vec3 position, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 direction, float cutOff):
  light(position, amb, diff, spec),
  m_direction(direction),
  m_cutOff(cutOff){
}

std::string spotLight::getUniforms(GLuint prog, int index){
  std::string lightName = light::getUniforms(prog, index);
  
  GLint directionLoc = glGetUniformLocation(prog, (lightName + ".direction").c_str());
  GLint cutOffLoc = glGetUniformLocation(prog, (lightName + ".cutOff").c_str());
  
  glUniform3f(directionLoc, m_direction.x, m_direction.y, m_direction.z);
  glUniform1f(cutOffLoc, m_cutOff);
  
  return lightName;
}
