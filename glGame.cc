#include<iostream>
#include"glGame.hh"

glGame::glGame(glm::vec3 position, unsigned int width, unsigned int height, std::string name):
  screenWidth(width),
  screenHeight(height),
  cam(position){
  unsigned int glMajor = 3;
  unsigned int glminor = 3;
  
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glminor);
  
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  
  window = glfwCreateWindow(screenWidth, screenHeight, name.c_str(), nullptr, nullptr);
  if(window == nullptr){
    glfwTerminate();
    throw glInitException("Failed to create GLFW window");
  }
  glfwMakeContextCurrent(window);
  
  if(gl3wInit()){
    glfwTerminate();
    throw glInitException("Failed to initialize GL3W");
  }
  if(!gl3wIsSupported(glMajor, glminor)){
    glfwTerminate();
    std::string msg("OpenGL ");
    msg += glMajor;
    msg += ".";
    msg += glminor;
    msg += " not supported.";
    throw glInitException(msg);
  }
  
  std::string versionInfo = std::string("OpenGL ") + std::string((const char*)glGetString(GL_VERSION)) + std::string(", GLSL ") + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
  std::cout << versionInfo << std::endl;

  glViewport(0, 0, screenWidth, screenHeight);
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //Wireframe mode
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

glGame::~glGame(){
  for(auto it = models.begin(); it != models.end(); ++it){
    (*it).second.cleanUp();
  }
  glfwTerminate();
}

void glGame::registerObject(GLuint target, model& obj){
  models.insert(std::pair<GLuint, model>(target, obj));
}

void glGame::registerLight(GLuint target, light& lite){
  lights.insert(std::pair<GLuint, light>(target, lite));
}

void glGame::setKeyCallback(void(*key_callback)(GLFWwindow*, int, int, int, int)){
  glfwSetKeyCallback(window, key_callback);
}

void glGame::setCursorCallback(void(*mouse_callback)(GLFWwindow*, double, double)){
  glfwSetCursorPosCallback(window, mouse_callback);
}

void glGame::setScrollCallback(void(*scroll_callback)(GLFWwindow*, double, double)){
  glfwSetScrollCallback(window, scroll_callback);
}

void glGame::setCameraCallback(void(*camera_callback)(camera&, const float)){
  moveCam = camera_callback;
}

  void glGame::run(){
  while(!glfwWindowShouldClose(window)){
    loop();
  }
}

void glGame::loop(){
  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  curFrame = glfwGetTime();
  GLfloat dTime = curFrame - lastFrame;
  lastFrame = curFrame;
  
  moveCam(cam, dTime);
  
  view = cam.getMatrix();
  projection = glm::perspective(glm::radians(fov), (float)screenWidth/screenHeight, 0.1f, 100.0f);
  
  GLint viewLoc = -1;
  GLint projLoc = -1;
  GLint viewPosLoc = -1;
  
  GLint prog = -1;
  for(auto it = models.begin(); it != models.end(); ++it){
    if((*it).first != prog){
      prog = (*it).first;
      glUseProgram(prog);
      viewLoc = glGetUniformLocation(prog, "view");
      projLoc = glGetUniformLocation(prog, "projection");
      viewPosLoc = glGetUniformLocation(prog, "viewPos");
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
      glUniform3f(viewPosLoc, cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);
      auto low = lights.lower_bound(prog);
      auto high = lights.upper_bound(prog);
      int i = 0;
      for(auto jt = low; jt != high; ++jt){
        (*jt).second.getUniforms(prog, i++);
      }
    }
    (*it).second.render(prog);
  }
  
  glfwSwapBuffers(window);
}
