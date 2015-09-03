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

bool keys[1024];
GLfloat xoffset;
GLfloat yoffset;
float lastX;
float lastY;
float maxFov = 100;
float fov = 80;

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

//bool firstMouse = true;
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
}

int main(){
  unsigned int glMajor = 3;
  unsigned int glminor = 3;
  unsigned int screenWidth = 800;
  unsigned int screenHeight = 600;
  GLfloat curFrame = glfwGetTime();
  GLfloat lastFrame = glfwGetTime();
  GLfloat dTime = curFrame - lastFrame;
  glfwInit();
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glminor);
  
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);
  if(window == nullptr){
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  
  if(gl3wInit()){
    std::cerr << "Failed to initialize GL3W" << std::endl;
    glfwTerminate();
    return -1;
  }
  if(!gl3wIsSupported(glMajor, glminor)){
    std::cerr << "OpenGL " << glMajor << "." << glminor << " not supported" << std::endl;
    glfwTerminate();
    return -1;
  }
  
  std::cout << "OpenGL " << glGetString(GL_VERSION) << ", GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  glViewport(0, 0, screenWidth, screenHeight);
  
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //Wireframe mode
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  
  shader program("vertex.glsl", "fragment.glsl");
  
  //Cube, with texture coordinates
  auto vertices = fileLoader::flatLoader("data/cubePTN.flat");
  
  std::vector<std::string> textures;
  std::vector<std::string> specMaps;
  textures.push_back(std::string("data/container2.png"));
  specMaps.push_back(std::string("data/container2_specular.png"));
  auto monkeyData = fileLoader::objLoader(std::string("data/monkey.obj"));
  
  model monkey(monkeyData.begin(), monkeyData.end(),
               false, true);
  //model tri(vertices, vertices+(sizeof(vertices) / sizeof(GLfloat)),
  model tri(vertices.begin(), vertices.end(),
            false, true,
            std::vector<GLuint>::iterator(), std::vector<GLuint>::iterator(),
            textures.begin(), textures.end(),
            specMaps.begin(), specMaps.end());
  //model camBox(vertices, vertices+(sizeof(vertices) / sizeof(GLfloat)),
  model camBox(vertices.begin(), vertices.end(),
               false, true,
               std::vector<GLuint>::iterator(), std::vector<GLuint>::iterator(),
               textures.begin(), textures.end());
  camera cam(glm::vec3(0.0, 0.0, 3.0));
  light lite(glm::vec3(0.0, 3.0, 0.0),
             glm::vec3(0.3f, 0.3f, 0.3f),
             glm::vec3(0.8f, 0.8f, 0.8f),
             glm::vec3(1.0f, 1.0f, 1.0f));
  
  tri.getShininess() = 32.0f;
  tri.rotate(-55, 1.0, 0.0, 0.0);
  
  camBox.translate(0.0, 3.0, 0.0);
  camBox.scale(0.2, 0.2, 0.2);
  
  glm::mat4 view;
  glm::mat4 projection;
  view = cam.getMatrix();
  projection = glm::perspective(glm::radians(fov), (float)screenWidth/screenHeight, 0.1f, 100.0f);
  while(!glfwWindowShouldClose(window)){
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program();
    
    curFrame = glfwGetTime();
    dTime = curFrame - lastFrame;
    lastFrame = curFrame;
    
    moveCam(cam, dTime);
    //stifle remnant offsets.
    //  offset doesn't get set to 0 when there is no mouse movement.
    xoffset /= 5;
    yoffset /= 5;
    view = cam.getMatrix();
    projection = glm::perspective(glm::radians(fov), (float)screenWidth/screenHeight, 0.1f, 100.0f);
    
    GLint viewLoc = glGetUniformLocation(program.getTarget(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    GLint projLoc = glGetUniformLocation(program.getTarget(), "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    GLint viewPosLoc = glGetUniformLocation(program.getTarget(), "viewPos");
    glUniform3f(viewPosLoc, cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);
    
    lite.getUniforms(program.getTarget());
    //tri.render(program.getTarget());
    camBox.render(program.getTarget());
    monkey.render(program.getTarget());
    
    glfwSwapBuffers(window);
  }
  
  glfwTerminate();
  return 0;
}
