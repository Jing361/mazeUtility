#ifndef __SCENE_MANAGER_HH__
#define __SCENE_MANAGER_HH__

#include<map>
#include"sceneNode.hh"
#include"entity.hh"
#include"light.hh"

class sceneManager{
private:
  class rootNode:public sceneNode{
  private:
    sceneManager* m_manager;
  
  public:
    void attachObject(entity* pEnt, GLuint shader){
      m_manager->attachObject(pEnt, shader);
    }
    
    void attachObject(light* pLight, GLuint shader){
      m_manager->attachObject(pLight, shader);
    }
  };
  
  std::map<GLuint, entity*> m_entities;
  std::map<GLuint, light*> m_lights;
  std::map<GLuint, spotLight*> m_spots;
  rootNode m_root;
  
  void attachObject(entity* pEnt, GLuint shader);
  void attachObject(light* pLight, GLuint shader);
  void attachObject(spotLight* pLight, GLuint shader);
  
  friend rootNode;

public:
  void render();
};

#endif
