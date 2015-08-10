#include<gl3w.h>
#include<glfw3.h>
#include<iostream>
#include<cmath>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"shader.hh"
#include"model.hh"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
  (void)scancode;//UNUSED
  (void)mode;//UNUSED
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

int main(){
  unsigned int glMajor = 3;
  unsigned int glminor = 3;
  unsigned int screenWidth = 800;
  unsigned int screenHeight = 600;
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
  
  GLfloat vertices[] = {
    //vertices           //Colors           //Texture coords
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // Top Right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom Right
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom Left
   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // Top Left 
  };
  /*GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };*/
  GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
  };
  
  std::vector<std::string> textures;
  textures.push_back(std::string("container.jpg"));
  textures.push_back(std::string("awesomeface.png"));
  model tri(vertices, vertices+(sizeof(vertices) / sizeof(GLfloat)), true,
            indices, indices+(sizeof(indices) / sizeof(GLuint)),
            //indices, indices,
            textures.begin(), textures.end());

  glfwSetKeyCallback(window, key_callback);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //Wireframe mode
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glm::mat4 view;
  glm::mat4 projection;
  
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  projection = glm::perspective(glm::radians(45.0f), (float)screenWidth/screenHeight, 0.1f, 100.0f);
  tri.rotate(-55, 1.0, 0.0, 0.0);
  while(!glfwWindowShouldClose(window)){
    //GLfloat timeValue = glfwGetTime();
    //GLfloat greenValue = (sin(timeValue) / 2) + 0.5f;
    //GLint vertexColorLocation = glGetUniformLocation(program.getTarget(), "ourColor");
    
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
  
    program();
    //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    tri.render(program.getTarget());
    
    GLuint viewLoc = glGetUniformLocation(program.getTarget(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    GLuint projLoc = glGetUniformLocation(program.getTarget(), "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
  
    glfwSwapBuffers(window);
  }
  
  glfwTerminate();
  return 0;
}
