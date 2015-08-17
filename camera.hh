#ifndef __CAMERA_HH__
#define __CAMERA_HH__

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

class camera{
private:
  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_worldUp;
  glm::vec3 m_right;
  glm::vec3 m_up;
  float m_speed;

public:
  enum direction{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
  };
  camera(glm::vec3 position, glm::vec3 front = glm::vec3(0.0, 0.0, -1.0), glm::vec3 up = glm::vec3(0.0, 1.0, 0.0), float speed = 0.05f);

  glm::mat4 getMatrix();
  void update();
  void tick(direction dir, float dTime);
};

#endif
