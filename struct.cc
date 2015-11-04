#include<iostream>
#include"resourceManager.hh"
#include"sceneManager.hh"
#include"sceneNode.hh"
#include"renderer.hh"
#include"shader.hh"
#include"camera.hh"
#include"entity.hh"
#include"light.hh"

int main(){
  //camera could be defined using ctor arguments
  //glRenderer renderer(glm::vec3(0,0,3), 1900, 1000);
  camera cam(glm::vec3(0,0,3), 1.5);
  renderer engine(&cam, 1900, 1000);
  //parameter confirms existence of valid renderer
  resourceManager rm(&engine);
  //parameter confirms existence of valid renderer, and tells renderer to use this manager for scene
  sceneManager manager(&engine);
  shader basicShader("fragment.glsl", "vertex.glsl");
  
  rm.acquire("crate", "mesh/crate.obj");
  rm.acquire("crate", "mat/crateA.png", 32);
  
  entity crate(rm.getMesh("crate"), rm.getMaterial("crate"));
  entity crateA(rm.getResources("crate"));
  
  glm::vec3 glow(0, .1, 0);
  light lamp(glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,0,1));
  
  manager.getRootNode()->attachAmbientLight(glow, basicShader.getTarget());
  
  sceneNode* node = manager.getRootNode()->createChild();
  node->setPosition(glm::vec3(0,0,0));
  node->attachObject(&crate, basicShader.getTarget());
  node->attachPointLight(&lamp, basicShader.getTarget());
  
  engine.run();
  
  return 0;
}
