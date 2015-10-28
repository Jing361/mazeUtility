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
  glRenderer renderer;
  //parameter confirms existence of valid renderer
  resourceManager rm(&renderer);
  //parameter confirms existence of valid renderer, and tells renderer to use this manager for scene
  sceneManager manager(&renderer);
  shader basicShader("fragment.glsl", "vertex.glsl");
  camera cam(glm::vec3(0,0,3), 1900, 1000);
  
  renderer.attachCamera(&cam);
  
  rm.acquire("crate", "mesh/crate.obj");
  rm.acquire("crate", "mat/crateA.png", 32);
  
  entity crate(rm.getMesh("crate"), rm.getMaterial("crate"));
  entity crateA(rm.getResources("crate"));
  
  ambientLight glow(glm::vec3(ambientColor));
  light lamp(glm::vec3(diffuse), glm::vec3(specular));
  
  manager.getRootNode()->attachAmbient(&glow);
  
  sceneNode* node = manager.getRootNode()->createChild();
  node->setPosition(glm::vec3(0,0,0));
  node->attachObject(&crate, basicShader.getTarget());
  node->attachLight(&lamp);
  
  renderer.run();
  
  return 0;
}
