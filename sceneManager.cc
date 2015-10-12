#include"sceneManager.hh"

void sceneManager::attachObject(entity* pEnt, GLuint shader){
  m_entities.insert(std::pair<GLuint, entity*>(shader, pEnt));
}

void sceneManager::attachObject(light* pLight, GLuint shader){
  m_lights.insert(std::pair<GLuint, light*>(shader, pLight));
}

void sceneManager::render(){
  
}
