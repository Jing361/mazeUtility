#ifndef __SCENE_MANAGER_HH__
#define __SCENE_MANAGER_HH__

#include<map>
#include<vector>
#include"sceneNode.hh"
#include"entity.hh"
#include"light.hh"
#include"spotLight.hh"
#include"renderer.hh"

class sceneManager{
private:
  class rootNode:public sceneNode{
  private:
    sceneManager* m_manager;
  
  public:
    rootNode(sceneManager* pManager):
      m_manager(pManager){
    }
    
    void attachObject(entity* pEnt, GLuint shader){
      m_manager->attachObject(pEnt, shader);
    }
    
    void attachPointLight(light* pLight, GLuint shader){
      m_manager->attachPointLight(pLight, shader);
    }
  
    void attachAmbientLight(glm::vec3 ambientColor, GLuint shader){
      m_manager->attachAmbientLight(ambientColor, shader);
    }
  };
  
  std::map<GLuint, entity*> m_entities;
  std::map<GLuint, light*> m_lights;
  std::map<GLuint, spotLight*> m_spots;
  std::map<GLuint, glm::vec3> m_ambient;
  rootNode m_root;
  
  void attachObject(entity* pEnt, GLuint shader);
  void attachPointLight(light* pLight, GLuint shader);
  void attachSpotLight(spotLight* pLight, GLuint shader);
  void attachAmbientLight(glm::vec3 ambientColor, GLuint shader);
  
  friend rootNode;

public:
  sceneManager(renderer* pRenderer);
  
  void render();
  sceneNode* getRootNode();
};

#endif
