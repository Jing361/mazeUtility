#include"sceneManager.hh"

sceneManager::sceneManager(glRenderer* renderer):
  m_root(this){
}

void sceneManager::attachObject(entity* pEnt, GLuint shader){
  m_entities.insert(std::pair<GLuint, entity*>(shader, pEnt));
}

void sceneManager::attachObject(light* pLight, GLuint shader){
  m_lights.insert(std::pair<GLuint, light*>(shader, pLight));
}

void sceneManager::attachObject(spotLight* pLight, GLuint shader){
  m_spots.insert(std::pair<GLuint, spotLight*>(shader, pLight));
}

void sceneManager::render(){
  GLint viewLoc = -1;
  GLint projLoc = -1;
  GLint viewPosLoc = -1;
  GLint nlightsLoc = -1;
  GLint nspotsLoc = -1;
  
  GLint prog = -1;
  for(auto it = m_entities.begin(); it != m_entities.end(); ++it){
    if((*it).first != prog){
      prog = (*it).first;
      glUseProgram(prog);
      viewLoc = glGetUniformLocation(prog, "view");
      projLoc = glGetUniformLocation(prog, "projection");
      viewPosLoc = glGetUniformLocation(prog, "viewPos");
      nlightsLoc = glGetUniformLocation(prog, "nLights");
      nspotsLoc = glGetUniformLocation(prog, "nSpots");
      
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
      glUniform3f(viewPosLoc, cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);
      glUniform1i(nlightsLoc, m_lights.size());
      glUniform1i(nspotsLoc, m_spots.size());
      
      int i = 0;
      for(auto jt = m_lights.lower_bound(prog); jt != m_lights.upper_bound(prog); ++jt){
        (*jt).second.getUniforms(prog, i++);
      }
      i = 0;
      for(auto jt = m_spots.lower_bound(prog); jt != m_spots.upper_bound(prog); ++jt){
        (*jt).second.getUniforms(prog, i++);
      }
    }
    (*it).second.render(prog);
  }
}

sceneNode* sceneManager::getRootNode(){
  return &m_root;
}
