#include<gl3w.h>
#include<glfw3.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cmath>
#include<functional>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"shader.hh"
#include"model.hh"
#include"camera.hh"
#include"light.hh"
#include"fileLoader.hh"
#include"glGame.hh"

bool keys[1024];
GLfloat xoffset;
GLfloat yoffset;
float lastX;
float lastY;
float maxFov = 100;
float fov = 80;

void moveCam(camera& cam, const float dTime){
  if(keys[GLFW_KEY_W]){
    cam.tick(camera::FORWARD, dTime);
  }
  if(keys[GLFW_KEY_S]){
    cam.tick(camera::BACKWARD, dTime);
  }
  if(keys[GLFW_KEY_A]){
    cam.tick(camera::LEFT, dTime);
  }
  if(keys[GLFW_KEY_D]){
    cam.tick(camera::RIGHT, dTime);
  }
  cam.look(xoffset, yoffset);
  xoffset = 0;
  yoffset = 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
  (void)scancode;//UNUSED
  (void)mode;//UNUSED
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
  if (key >= 0 && key < 1024){
    if(action == GLFW_PRESS){
      keys[key] = true;
    }else if(action == GLFW_RELEASE){
      keys[key] = false;
    }
  }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
  (void)window;//UNUSED
  xoffset = xpos - lastX;
  yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xchange, double ychange){
  (void)window;//UNUSED
  (void)xchange;//UNUSED
  if(fov >= 1.0f && fov <= maxFov){
  	fov -= ychange;
  }
  if(fov <= 1.0f){
  	fov = 1.0f;
  }
  if(fov >= maxFov){
  	fov = maxFov;
  }
}

int main(){
  glGame game(glm::vec3(0.0, 0.0, 3.0),
              800, 600);
  shader program("vertex.glsl", "fragment.glsl");
  
  game.setKeyCallback(key_callback);
  game.setCursorCallback(mouse_callback);
  game.setScrollCallback(scroll_callback);
  game.setCameraCallback(moveCam);
  
  //Cube, with texture coordinates
  auto vertices = fileLoader::flatLoader("data/cubePTN.flat");
  
  std::vector<std::string> textures;
  std::vector<std::string> specMaps;
  textures.push_back(std::string("data/container2.png"));
  specMaps.push_back(std::string("data/container2_specular.png"));
  auto monkeyData = fileLoader::objLoader(std::string("data/monkey.obj"));
  
  model monkey(monkeyData.begin(), monkeyData.end(),
               false, true);
  model tri(vertices.begin(), vertices.end(),
            false, true,
            std::vector<GLuint>::iterator(), std::vector<GLuint>::iterator(),
            textures.begin(), textures.end(),
            specMaps.begin(), specMaps.end());
  model camBox(vertices.begin(), vertices.end(),
               false, true,
               std::vector<GLuint>::iterator(), std::vector<GLuint>::iterator(),
               textures.begin(), textures.end());
  light lite(glm::vec3(0.0, 3.0, 0.0),
             glm::vec3(0.3f, 0.3f, 0.3f),
             glm::vec3(0.8f, 0.8f, 0.8f),
             glm::vec3(1.0f, 1.0f, 1.0f));
  
  monkey.translate(0, -2, 0);
  
  tri.getShininess() = 32.0f;
  tri.rotate(-55, 1.0, 0.0, 0.0);
  
  camBox.translate(0.0, 3.0, 0.0);
  camBox.scale(0.2, 0.2, 0.2);
  
  game.registerObject(program.getTarget(), monkey);
  game.registerObject(program.getTarget(), tri);
  game.registerObject(program.getTarget(), camBox);
  game.registerLight(program.getTarget(), lite);
  game.run();
  
  return 0;
}
