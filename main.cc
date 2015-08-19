#include<gl3w.h>
#include<glfw3.h>
#include<iostream>
#include<cmath>
#include<functional>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"shader.hh"
#include"model.hh"
#include"camera.hh"
#include"light.hh"

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
/*  if (firstMouse){
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }*/
  xoffset = xpos - lastX;
  yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
  if(fov >= 1.0f && fov <= maxFov){
  	fov -= yoffset;
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

  glViewport(0, 0, 800, 600);
  
  shader program("vertex.glsl", "fragment.glsl");
  
  //rectangle
  /*GLfloat vertices[] = {
    //vertices           //Colors           //Texture coords
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // Top Right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom Right
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom Left
   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // Top Left 
  };
  
  GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
  };*/
  //cube with fucked texture coordinates
  /*GLfloat vertices[] = {
    -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 1.0f,
     
    -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 0.0f
  };
  
  GLuint indices[] = {
    0, 1, 2,
    1, 2, 3,
    4, 5, 6,
    5, 6, 7,
    0, 4, 5,
    0, 1, 5,
    1, 3, 5,
    3, 5, 7,
    2, 3, 6,
    3, 6, 7,
    0, 4, 6,
    0, 2, 6
  };*/
  //textured cube
  /*GLfloat vertices[] = {
    //bottom
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    //top
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    //left
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    //right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     //front
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    //back
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };*/
  //normalled cube
  GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };
  
  std::vector<std::string> textures;
  //textures.push_back(std::string("container.jpg"));
  //textures.push_back(std::string("awesomeface.png"));
  model tri(vertices, vertices+(sizeof(vertices) / sizeof(GLfloat)),
            false, true,
            //indices, indices+(sizeof(indices) / sizeof(GLuint)),
            std::vector<GLuint>::iterator(), std::vector<GLuint>::iterator(),
            textures.begin(), textures.end());
  model camBox(vertices, vertices+(sizeof(vertices) / sizeof(GLfloat)),
               false, true,
               //indices, indices+(sizeof(indices) / sizeof(GLuint)),
               std::vector<GLuint>::iterator(), std::vector<GLuint>::iterator(),
               textures.begin(), textures.end());
  camera cam(glm::vec3(0.0, 0.0, 3.0));

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //Wireframe mode
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glm::mat4 view;
  glm::mat4 projection;
  view = cam.getMatrix();
  projection = glm::perspective(glm::radians(fov), (float)screenWidth/screenHeight, 0.1f, 100.0f);
  
  //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  tri.rotate(-55, 1.0, 0.0, 0.0);
  light lite(glm::vec3(0.0, 3.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
  camBox.translate(0.0, 3.0, 0.0);
  while(!glfwWindowShouldClose(window)){
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program();
    
    curFrame = glfwGetTime();
    dTime = curFrame - lastFrame;
    lastFrame = curFrame;
    
    GLfloat greenValue = (sin(curFrame) / 2) + 0.5f;
    GLint objColorLoc = glGetUniformLocation(program.getTarget(), "objColor");
    glUniform3f(objColorLoc, 0.0f, greenValue, 0.0f);
    //glUniform3f(objColorLoc, 1.0f, 1.0, 1.0f);
    
    moveCam(cam, dTime);
    //stifle remnant offsets.
    //  offset doesn't get set to 0 when there is no mouse movement.
    xoffset /= 5;
    yoffset /= 5;
    view = cam.getMatrix();
    projection = glm::perspective(glm::radians(fov), (float)screenWidth/screenHeight, 0.1f, 100.0f);
    
    tri.render(program.getTarget());
    camBox.render(program.getTarget());
    
    GLint viewLoc = glGetUniformLocation(program.getTarget(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    GLint projLoc = glGetUniformLocation(program.getTarget(), "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    GLint viewPosLoc = glGetUniformLocation(program.getTarget(), "viewPos");
    glUniform3f(viewPosLoc, cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);
    
    lite.getUniforms(program.getTarget());
    
    glfwSwapBuffers(window);
  }
  
  glfwTerminate();
  return 0;
}
