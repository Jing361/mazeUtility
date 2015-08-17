#include<cmath>
#include"camera.hh"

camera::camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed):
  m_position(position),
  m_front(front),
  m_worldUp(up),
  m_right(glm::normalize(glm::cross(m_front, m_worldUp))),
  m_up(glm::normalize(glm::cross(m_right, m_front))),
  m_speed(speed){
  update();
}

glm::mat4 camera::getMatrix(){
  return glm::lookAt(m_position,
                     m_position + m_front,
                     m_up);
}

void camera::tick(direction dir, float dTime){
  float velocity = m_speed * dTime;
  if(dir == FORWARD){
    m_position += m_front * velocity;
  } else if(dir == BACKWARD){
    m_position-= m_front * velocity;
  } else if(dir == LEFT){
    m_position -= m_right * velocity;
  } else if(dir == RIGHT){
    m_position += m_right * velocity;
  }
}

void camera::update(){
//  glm::vec3  front;
//  front.x = cos(glm::radians())
}
