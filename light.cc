#include<sstream>
#include"light.hh"

std::string light::calcVarName(std::string name, int index){
  if(index >= 0){
    std::stringstream ss;
    
    ss << index;
    
    name += "[";
    name += ss.str();
    name += "]";
  }
  return name;
}

light::light(glm::vec3 position, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec):
  m_position(position),
  m_ambient(amb),
  m_diffuse(diff),
  m_specular(spec){
}

void light::getUniforms(GLuint prog, int index){
  std::string lightName(calcVarName("lights", index));
  
  GLint lightPosLoc       = glGetUniformLocation(prog, (lightName + ".position").c_str());
  
  GLint lightAmbientLoc   = glGetUniformLocation(prog, (lightName + ".ambient").c_str());
  GLint lightDiffuseLoc   = glGetUniformLocation(prog, (lightName + ".diffuse").c_str());
  GLint lightSpecularLoc  = glGetUniformLocation(prog, (lightName + ".specular").c_str());
  
  GLint lightConstantLoc  = glGetUniformLocation(prog, (lightName + ".constant").c_str());
  GLint lightLinearLoc    = glGetUniformLocation(prog, (lightName + ".linear").c_str());
  GLint lightQuadraticLoc = glGetUniformLocation(prog, (lightName + ".quadratic").c_str());
  
  glUniform3f(lightPosLoc,      m_position.x, m_position.y, m_position.z);
  
  glUniform3f(lightAmbientLoc,  m_ambient.r, m_ambient.g, m_ambient.b);
  glUniform3f(lightDiffuseLoc,  m_diffuse.r, m_diffuse.g, m_diffuse.b);
  glUniform3f(lightSpecularLoc, m_specular.r, m_specular.g, m_specular.b);
  
  glUniform1f(lightConstantLoc,  m_constant);
  glUniform1f(lightLinearLoc,    m_linear);
  glUniform1f(lightQuadraticLoc, m_quadratic);
}
