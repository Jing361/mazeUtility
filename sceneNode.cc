#include"sceneNode.hh"

sceneNode::sceneNode(sceneNode* pNode = nullptr){
  m_parent = pNode;
}

sceneNode* sceneNode::createChild(){
  return new sceneNode(this);
}

void sceneNode::attachObject(entity* pEnt, GLuint shader){
  if(m_parent){
    m_parent->attachObject(pEnt, shader);
  }
  if(pEnt){
    pEnt->attach(this);
  }
}

void sceneNode::attachObject(light* pLight, GLuint shader){
  if(m_parent){
    m_parent->attachObject(pLight, shader);
  }
  if(pLight){
    pLight->attach(this);
  }
}

glm::mat4 sceneNode::getTransform(){
  glm::mat4 transform;
  if(m_parent){
    transform = m_parent->getTransform();
  }
  
  return transform * m_transform;
}
