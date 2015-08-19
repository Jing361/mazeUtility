#ifndef __LIGHT_HH__
#define __LIGHT_HH__

#include<gl3w.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

class light{
public:
  glm::vec3 m_position;
  glm::vec3 m_color;
  
  light(glm::vec3 position, glm::vec3 color);
  
  void getUniforms(GLuint prog);
};

#endif
