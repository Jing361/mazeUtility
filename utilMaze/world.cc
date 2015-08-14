#include<iostream>
#include<sstream>
#include"world.hh"

world::world(std::string mazeStr){
  std::stringstream ss(mazeStr);
  std::string line;
  
  std::getline(ss, line);
  width = line.size();
  height = (mazeStr.size() / (line.size() + 1));
  depth = 1;
  while(std::getline(ss, line)){
    if(line == ""){
      ++depth;
    }
  }
  
  maze = new space**[width];
  for(unsigned int i = 0; i < width; ++i){
    maze[i] = new space*[height];
    for(unsigned int j = 0; j < height; ++j){
      maze[i][j] = new space[depth];
    }
  }

  //reset ss
  ss.str("");
  ss.clear();
  ss.str(mazeStr);
  
  unsigned int W = 0;
  unsigned int H = 0;
  unsigned int D = 0;
  while(std::getline(ss, line)){
    if(line == ""){
      ++D;
      W = 0;
      H = 0;
    } else {
      for(unsigned int i = 0; i < width; ++i){
        switch(line[i]){
        case ' ':
          maze[W][H][D] = EMPTY;
        break;
        case 'S':
          maze[W][H][D] = START;
          start = position(W, H, D);
        break;
        case 'E':
          maze[W][H][D] = END;
          end = position(W, H, D);
        break;
        case 'T':
          maze[W][H][D] = STAIR;
        break;
        case '|':
        case '+':
        default:
          maze[W][H][D] = WALL;
        break;
        };
        ++W;
      }
      ++H;
      W = 0;
    }
  }
}

world::~world(){
/*  for(unsigned int i = 0; i < width; ++i){
    std::cout << "i " << i << std::endl;
    for(unsigned int j = 0; i < height; ++j){
      std::cout << "j " << j << std::endl;
      delete[] maze[i][j];
    }
    delete[] maze[i];
    std::cout << "i deleted" << std::endl;
  }
  delete[] maze;
  std::cout << "deleted" << std::endl;*/
}

void world::print(){
  for(unsigned int k = 0; k < depth; ++k){
    for(unsigned int i = 0; i < width; ++i){
      for(unsigned int j = 0; j < height; ++j){
        switch(maze[i][j][k]){
        case WALL:
          std::cout << "+";
        break;
        case EMPTY:
          std::cout << " ";
        break;
        case START:
          std::cout << "S";
        break;
        case END:
          std::cout << "E";
        break;
        case STAIR:
          std::cout << "T";
        break;
        };
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}