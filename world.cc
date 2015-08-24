#include<iostream>
#include<sstream>
#include<random>
#include<chrono>
#include"world.hh"

world::world(unsigned int x, unsigned int y, unsigned int z):
  width(x % 2 == 0?x + 1:x),
  height(y % 2 == 0?x + 1:y),
  depth(z){
  maze = new space**[width];
  for(unsigned int i = 0; i < width; ++i){
    maze[i] = new space*[height];
    for(unsigned int j = 0; j < height; ++j){
      maze[i][j] = new space[depth];
    }
  }
}

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
  std::cout << depth << " floors\n";
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

void world::clear(){
  //initialize world
  for(unsigned int i = 0; i < depth; ++i){
    //perimeter walls
    for(unsigned int j = 0; j < width; ++j){
      maze[j][0][i] = WALL;
      maze[j][height - 1][i] = WALL;
    }
    for(unsigned int j = 0; j < height; ++j){
      maze[0][j][i] = WALL;
      maze[width - 1][j][i] = WALL;
    }
    
    //insert alternating wall and open space
    for(unsigned int j = 1; j < width - 1; ++j){
      for(unsigned int k = 1; k < height - 1; ++k){
        if(j % 2 == 0 || k % 2 == 0){
          maze[j][k][i] = WALL;
        } else {
          maze[j][k][i] = EMPTY;
        }
      }
    }
  }
  
  placeEnds();
}

void world::place(space sp, position& pos){
  std::mt19937::result_type seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 engine(seed);
  std::uniform_int_distribution<unsigned int> sDist(0, 1);//start distribution
  
  //select start position
  //0 selects side walls
  if(sDist(engine) == 0){
    std::uniform_int_distribution<unsigned int> iDist(1, (width - 1) / 2);//idx distribution
    std::uniform_int_distribution<unsigned int> deepDist(0, depth - 1);//depth distribution
    unsigned int W = (iDist(engine) * 2) - 1;
    unsigned int H;
    unsigned int D = deepDist(engine);
    if(sDist(engine) == 0){
      H = 0;
    } else {
      H = height - 1;
    }
    maze[W][H][D] = sp;
    pos = position(W, H, D);
  } else {
    std::uniform_int_distribution<unsigned int> iDist(1, (height - 1) / 2);//idx distribution
    std::uniform_int_distribution<unsigned int> deepDist(0, depth - 1);//depth distribution
    unsigned int H = (iDist(engine) * 2) - 1;
    unsigned int W;
    unsigned int D = deepDist(engine);
    if(sDist(engine) == 0){
      W = 0;
    } else {
      W = width - 1;
    }
    maze[W][H][D] = sp;
    pos = position(W, H, D);
  }
}

void world::placeEnds(){
  //select start position
  place(START, start);
  
  //select end position
  place(END, end);
}

space& world::getSpace(coord x, coord y, coord z){
  return maze[x][y][z];
}

space& world::getSpace(position pos){
  return getSpace(std::get<0>(pos), std::get<1>(pos), std::get<2>(pos));
}
