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
  float m_sensitivity = .15;
  float m_yaw = 0.0;
  float m_pitch = 0.0;
  float m_roll = 0.0;
  bool m_movePlanar = false;

public:
  enum direction{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
  };
  camera(glm::vec3 position, float speed = 0.8f, glm::vec3 front = glm::vec3(0.0, 0.0, -1.0), glm::vec3 up = glm::vec3(0.0, 1.0, 0.0));

  glm::mat4 getMatrix();
  void tick(direction dir, float dTime);
  void look(float yaw, float pitch, float roll);
  void look(float xoffset, float yoffset);
  glm::vec3 getPosition();
  bool toggleMovement();
};

#endif
