#include"glGame.hh"

glGame::glGame(unsigned int width, unsigned int height, std::string name =  "Untitled"){
  unsigned int glMajor = 3;
  unsigned int glminor = 3;
  unsigned int screenWidth = 800;
  unsigned int screenHeight = 600;
  
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
    throw glInitException(std::string("OpenGL " + glMajor + "." + glminor + " not supported"));
  }
  
  versionInfo = std::string("OpenGL ") + std::string(glGetString(GL_VERSION)) + std::string(", GLSL ") + std::string(glGetString(GL_SHADING_LANGUAGE_VERSION));

  glViewport(0, 0, screenWidth, screenHeight);
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
