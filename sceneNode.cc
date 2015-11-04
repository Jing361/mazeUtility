#include"entity.hh"
#include"sceneNode.hh"

sceneNode::sceneNode(sceneNode* pNode = nullptr):
  m_parent(pNode){
}

sceneNode* sceneNode::createChild(){
  return new sceneNode(this);
}

void sceneNode::setPosition(glm::vec3 pPosition){
  m_transform = glm::translate(m_transform, pPosition - m_position);
}

void sceneNode::translate(double x, double y, double z){
  m_transform = glm::translate(m_transform, glm::vec3(x, y, z));
}

void sceneNode::attachObject(entity* pEnt, GLuint shader){
  if(m_parent){
    m_parent->attachObject(pEnt, shader);
  }
  if(pEnt){
    pEnt->attach(this);
  }
}

void sceneNode::attachPointLight(light* pLight, GLuint shader){
  if(m_parent){
    m_parent->attachPointLight(pLight, shader);
  }
  if(pLight){
    pLight->attach(this);
  }
}

void sceneNode::attachSpotLight(spotLight* pLight, GLuint shader){
  if(m_parent){
    m_parent->attachSpotLight(pLight, shader);
  }
  if(pLight){
    pLight->attach(this);
  }
}

void sceneNode::attachAmbientLight(glm::vec3 ambientColor, GLuint shader){
  if(m_parent){
    m_parent->attachAmbientLight(ambientColor, shader);
  }
}

glm::mat4 sceneNode::getTransform(){
  glm::mat4 transform;
  if(m_parent){
    transform = m_parent->getTransform();
  }
  
  return transform * m_transform;
}
