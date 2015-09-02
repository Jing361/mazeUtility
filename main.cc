#include<gl3w.h>
#include<glfw3.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<tuple>
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

typedef std::tuple<GLfloat, GLfloat, GLfloat> triple;
triple extractData(std::string line, bool check = true){
  triple ret;
  GLfloat value;
  std::string token;
  std::stringstream ss(line);
  //dump first value
  ss >> token;
  
  ss >> value;
  std::get<0>(ret) = value;
  ss >> value;
  std::get<1>(ret) = value;
  if(check){
    ss >> value;
    std::get<2>(ret) = value;
  }
  
  return ret;
}

std::vector<GLfloat> objLoader(std::string fileName){
  std::ifstream file(fileName);
  std::string line;
  std::vector<GLfloat> object;
  std::vector<triple> vertices;
  std::vector<std::tuple<GLfloat, GLfloat>> texCoords;
  std::vector<triple> normals;
  std::vector<triple> color;
  std::vector<std::string> faces;
  
  while(std::getline(file, line)){
    if(line[0] != '#'){
      if(line[0] == 'v'){
        if(line[1] == 'n'){
          normals.push_back(extractData(line));
        } else if(line[1] == 't'){
          triple vals = extractData(line, false);
          std::tuple<GLfloat, GLfloat> actual = std::make_tuple(std::get<0>(vals), std::get<1>(vals));
          texCoords.push_back(actual);
        } else {
          vertices.push_back(extractData(line));
        }
      } else if(line[0] == 'f'){
        faces.push_back(line);
      }
    }
  }
  
  for(auto it = faces.begin(); it != faces.end(); ++it){
    std::stringstream ss(*it);
    std::string token;
    //dump first value; 'f'
    ss >> token;
    
    while(ss >> token){
      std::string value;
      std::stringstream iss(token);
      std::getline(iss, value, '/');
      std::stringstream jss(value);
      int idx;
      jss >> idx;
      --idx;
      
      //position
      {
        auto it = vertices[idx];
        object.push_back(std::get<0>(it));
        object.push_back(std::get<1>(it));
        object.push_back(std::get<2>(it));
      }
      //color
      if(color.size() > 0){
        auto it = color[idx];
        object.push_back(std::get<0>(it));
        object.push_back(std::get<1>(it));
        object.push_back(std::get<2>(it));
      }
      //texture
      if(texCoords.size() > 0){
        auto it = texCoords[idx];
        object.push_back(std::get<0>(it));
        object.push_back(std::get<1>(it));
      }
      //normal
      if(normals.size() > 0){
        auto it = normals[idx];
        object.push_back(std::get<0>(it));
        object.push_back(std::get<1>(it));
        object.push_back(std::get<2>(it));
      }
    }
  }
  return object;
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
  GLfloat vertices[] = {
    //Positions           //Texture    //Normals
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f,   0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,   0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
  };
  
  std::vector<std::string> textures;
  std::vector<std::string> specMaps;
  textures.push_back(std::string("data/container2.png"));
  specMaps.push_back(std::string("data/container2_specular.png"));
  auto monkeyData = objLoader(std::string("data/monkey.obj"));
  
  model monkey(monkeyData.begin(), monkeyData.end(),
               false, true);
  model tri(vertices, vertices+(sizeof(vertices) / sizeof(GLfloat)),
            false, true,
            std::vector<GLuint>::iterator(), std::vector<GLuint>::iterator(),
            textures.begin(), textures.end(),
            specMaps.begin(), specMaps.end());
  model camBox(vertices, vertices+(sizeof(vertices) / sizeof(GLfloat)),
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
