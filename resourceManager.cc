#include<fstream>
#include<SOIL.h>
#include"fileLoader.hh"
#include"resourceManager.hh"

GLint resourceManager::acquireTexture(std::string file){
  GLint tex;
  unsigned char* image;
  int width, height;
  
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  image = SOIL_load_image(file.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(image);
  
  glBindTexture(GL_TEXTURE_2D, 0);
  
  return tex;
}

static bool resourceManager::checkFile(std::string fileName){
  std::ifstream file(fileName);
  bool ret = file;
  
  file.close();
  
  return ret;
}

void resourceManager::acquireMaterial(std::string name, std::string fileName, float shine){
  material mat;
  
  if(checkFile(fileName)){
    mat.m_diffMap = acquireTexture(fileName);
    mat.m_specMap = -1;
  } else {
    size_t dot = fileName.rfind('.');
    std::string title(fileName.substr(0, dot));
    std::string type(fileName.substr(dot));
    std::string diffName = title + "_diffuse." + type;
    std::string specName = title + "_specular." + type;
    
    if(!checkFile(diffName)){
      //require at least diffuse map
      throw invalideFileNameException("Invalid file name.  Valid diffuse map required.");
    } else {
      mat.m_diffMap = acquireTexture(diffName);
    }
    if(!checkFile(specName)){
      mat.m_specMap = -1;
    } else {
      mat.m_specMap = acquireTexture(specName);
    }
  }
  mat.m_shininess = shine;
  
  m_materials.insert(std::pair<std::string, material>(name, mat));
}

void resourceManager::acquireMesh(std::string name, std::string fileName){
  mesh mes;
  std::vector<GLfloat> verts;
  
  size_t dot = fileName.rfind('.');
  std::string type(fileName.substr(dot));
  if(type == "obj"){
    verts = fileLoader::objLoader(fileName);
  } else if(type == "flat"){
    verts = fileLoader::flatLoader(fileName);
  }
  mes.m_nVert = verts.size();
  
  glGenBuffers(1, &mes.m_vbo);
  
  glBindBuffer(GL_ARRAY_BUFFER, mes.m_vbo);
  glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GLfloat), verts.data(), GL_STATIC_DRAW);
  glBindBuffer(0);
  
  m_meshes.insert(std::pair<std::string, mesh>(name, mes));
}

resourceManager::~resourceManager(){
  for(auto it = m_materials.begin(); it != m_materials.end(); ++it){
    glDeleteTextures(1, &(*it).second.m_diffMap);
    if((*it).second.m_specMap != -1){
      glDeleteTextures(1, &(*it).second.m_specMap);
    }
  }
  
  for(auto it = m_meshes.begin(); it != m_meshes.end()){
    glDeleteBuffers(1, &(*it).second);
  }
}

void resourceManager::acquire(std::string pName, std::string file, float shine){
  size_t slash = pName.find('/');
  std::string type(pName.substr(0, slash));
  std::string name(pName.substr(slash));
  
  if(type == "mesh"){
    acquireMesh(name, file);
  } else if(type == "material" || type == "mat"){
    acquireMaterial(name, file, shine);
  }
}

material resourceManager::getMaterial(std::string name){
  material ret;
  auto itr = m_materials.find(name);
  
  if(itr != m_materials.end()){
    ret = *itr;
  } else {
    throw invalidResourceException("Invalid resource" + name);
  }
  
  return ret;
}

mesh resourceManager::getMesh(std::string name){
  mesh ret;
  auto itr = m_meshes.find(name);
  
  if(itr != m_meshes.end()){
    ret = *itr;
  } else {
    throw invalidResourceException("Invalid resource" + name);
  }
  
  return ret;
}
