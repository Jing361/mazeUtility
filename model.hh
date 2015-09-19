#ifndef __MODEL_HH__
#define __MODEL_HH__

#include<gl3w.h>
#include<glm/glm.hpp>
#include<vector>
#include<string>
#include<SOIL.h>

class model{
private:
  std::vector<GLfloat> m_vertices;
  std::vector<GLuint> m_indices;
  std::vector<GLuint> m_textures;
  std::vector<GLuint> m_specMaps;
  GLuint m_VAO;
  GLuint m_VBO;
  GLuint m_EBO;
  unsigned int m_nVert;
  
  glm::mat4 m_transform;
  float m_shininess;
  
public:
//somehow handle additional attributes like color
  template<typename vertItr,
  typename idxItr = std::vector<GLfloat>::iterator,
  typename texItr = std::vector<std::string>::iterator,
  typename specItr = std::vector<std::string>::iterator>
  model(vertItr firstVert, vertItr lastVert,
        bool hasColor = false, bool hasNormal = false,
        idxItr firstIdx = idxItr(), idxItr lastIdx = idxItr(),
        texItr firstTex = texItr(), texItr lastTex = texItr(),
        specItr firstSpec = specItr(), specItr lastSpec = specItr()):
    m_vertices(firstVert, lastVert),
    m_indices(firstIdx, lastIdx){
    unsigned int stride = 3;
    unsigned int attr = 0;
    unsigned int offset = 0;
    if(hasColor){
      stride += 3;
    }
    if(hasNormal){
      stride += 3;
    }
    if(firstTex != lastTex || firstSpec != lastSpec){
      stride += 2;
    }
    m_nVert = m_vertices.size() / stride;
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    if(m_indices.size() > 0){
      glGenBuffers(1, &m_EBO);
    }
    
    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);
    
    if(m_indices.size() > 0){
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);
    }

    //position
    {
      unsigned int nData = 3;
      glVertexAttribPointer(attr, nData, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)offset);
      glEnableVertexAttribArray(attr);
      offset += nData;
    }
    ++attr;
    
    //color
    if(hasColor){
      unsigned int nData = 3;
      glVertexAttribPointer(attr, nData, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(offset * sizeof(GLfloat)));
      glEnableVertexAttribArray(attr);
      offset += nData;
    }
    ++attr;
    
    //texture coodinates
    if(firstTex != lastTex || firstSpec != lastSpec){
      unsigned int nData = 2;
      glVertexAttribPointer(attr, nData, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(offset * sizeof(GLfloat)));
      glEnableVertexAttribArray(attr);
      offset += nData;
    }
    ++attr;
    
    //normal vector
    if(hasNormal){
      unsigned int nData = 3;
      glVertexAttribPointer(attr, nData, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(offset * sizeof(GLfloat)));
      glEnableVertexAttribArray(attr);
      offset += nData;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    //setup textures
    for(auto it = firstTex; it != lastTex; ++it){
      int width, height;
      unsigned char* image;
      GLuint tex;
      
      glGenTextures(1, &tex);
      glBindTexture(GL_TEXTURE_2D, tex);
      
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      
      image = SOIL_load_image((*it).c_str(), &width, &height, 0, SOIL_LOAD_RGB);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
      glGenerateMipmap(GL_TEXTURE_2D);
      SOIL_free_image_data(image);
      
      glBindTexture(GL_TEXTURE_2D, 0);
      
      m_textures.push_back(tex);
    }
    //setup specmaps
    for(auto it = firstSpec; it != lastSpec; ++it){
      int width, height;
      unsigned char* image;
      GLuint tex;
      
      glGenTextures(1, &tex);
      glBindTexture(GL_TEXTURE_2D, tex);
      
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      
      image = SOIL_load_image((*it).c_str(), &width, &height, 0, SOIL_LOAD_RGB);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
      glGenerateMipmap(GL_TEXTURE_2D);
      SOIL_free_image_data(image);
      
      glBindTexture(GL_TEXTURE_2D, 0);
      
      m_specMaps.push_back(tex);
    }
  }
  
  void render(GLuint prog);
  float& getShininess();
  void translate(double x, double y, double z);
  void rotate(float r, double x, double y, double z);
  void scale(double x, double y, double z);
  
  void cleanUp();
};

//#include"model.cc"

#endif
