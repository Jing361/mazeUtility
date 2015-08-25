#include<cmath>
#include"camera.hh"

camera::camera(glm::vec3 position, float speed, glm::vec3 front, glm::vec3 up):
  m_position(position),
  m_front(front),
  m_worldUp(up),
  m_right(glm::normalize(glm::cross(m_front, m_worldUp))),
  m_up(glm::normalize(glm::cross(m_right, m_front))),
  m_speed(speed){
}

glm::mat4 camera::getMatrix(){
  return glm::lookAt(m_position,
                     m_position + m_front,
                     m_up);
}

void camera::tick(direction dir, float dTime){
  float velocity = m_speed * dTime;
  glm::vec3 front;
  if(!m_movePlanar){
    front = m_front;
  } else {
    front = glm::cross(m_worldUp, m_right);
  }
  if(dir == FORWARD){
    m_position += front * velocity;
  } else if(dir == BACKWARD){
    m_position -= front * velocity;
  } else if(dir == LEFT){
    m_position -= m_right * velocity;
  } else if(dir == RIGHT){
    m_position += m_right * velocity;
  }
}

void camera::look(float yaw, float pitch, float roll){
  
}

void camera::look(float xoffset, float yoffset){
  xoffset *= m_sensitivity;
  yoffset *= m_sensitivity;
  
  m_yaw += xoffset;
  m_pitch += yoffset;
  
  if(m_pitch > 89.0f){
    m_pitch = 89.0f;
  }
  if(m_pitch < -89.0f){
    m_pitch = -89.0f;
  }

  glm::vec3 front;
  front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  front.y = sin(glm::radians(m_pitch));
  front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_front = glm::normalize(front);
  m_right = glm::normalize(glm::cross(m_front, m_worldUp));
}

glm::vec3 camera::getPosition(){
  return m_position;
}

bool camera::toggleMovement(){
  return (m_movePlanar = !m_movePlanar);
}
