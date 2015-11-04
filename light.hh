#ifndef __LIGHT_HH__
#define __LIGHT_HH__

#include<gl3w.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<string>

class light{
protected:
  std::string calcVarName(std::string name, int index);

public:
  glm::vec3 m_position;
  
  glm::vec3 m_diffuse;
  glm::vec3 m_specular;
  
  //3250 distance
  float m_constant = 1.0;
  float m_linear = 0.0014;
  float m_quadratic = 0.000007;
  //7 distance
  /*float m_constant = 1.0;
  float m_linear = 0.7;
  float m_quadratic = 1.8;*/
  //13 distance
  /*float m_constant = 1.0;
  float m_linear = 0.35;
  float m_quadratic = 0.44;*/
  //20 distance
  /*float m_constant = 1.0;
  float m_linear = 0.22;
  float m_quadratic = 0.20;*/
  //32 distance
  /*float m_constant = 1.0;
  float m_linear = 0.14;
  float m_quadratic = 0.07;*/
  
  light(glm::vec3 position, glm::vec3 diff, glm::vec3 spec);
  
  void getUniforms(GLuint prog, int index = -1);
};

#endif
