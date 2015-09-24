#ifndef __SPOT_LIGHT_HH__
#define __SPOT_LIGHT_HH__

#include"light.hh"

class spotLight:public light{
public:
  glm::vec3 m_direction;
  float m_cutOff;
  
  spotLight(glm::vec3 position, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 direction, float cutOff = 15);
  
  void getUniforms(GLuint prog, int index = -1);
};

#endif
