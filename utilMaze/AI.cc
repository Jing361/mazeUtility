#include<queue>
#include<random>
#include<chrono>
#include"AI.hh"

#include<iostream>

std::vector<Node> AI::expand(Node node){
  std::vector<Node> ret;
  coord x = std::get<0>(node.back());
  coord y = std::get<1>(node.back());
  coord z = std::get<2>(node.back());
  if(x > 0 && w->maze[x - 1][y][z] != WALL){
    Node n = node;
    n.push_back(position(x - 1, y, z));
    ret.push_back(n);
  }
  if(y > 0 && w->maze[x][y - 1][z] != WALL){
    Node n = node;
    n.push_back(position(x, y - 1, z));
    ret.push_back(n);
  }
  if(z > 0 && w->maze[x][y][z] == STAIR && w->maze[x][y][z - 1] == STAIR){
    Node n = node;
    n.push_back(position(x, y, z - 1));
    ret.push_back(n);
  }
  if(x < w->width - 1 && w->maze[x + 1][y][z] != WALL){
    Node n = node;
    n.push_back(position(x + 1, y, z));
    ret.push_back(n);
  }
  if(y < w->height - 1 && w->maze[x][y + 1][z] != WALL){
    Node n = node;
    n.push_back(position(x, y + 1, z));
    ret.push_back(n);
  }
  if(z < w->depth && w->maze[x][y][z] == STAIR && w->maze[x][y][z + 1] == STAIR){
    Node n = node;
    n.push_back(position(x, y, z + 1));
    ret.push_back(n);
  }
  
  return ret;
}

std::vector<Node> AI::operator()(){
  heuristic h(w);
  std::priority_queue<Node, std::vector<Node>, heuristic> frontier(h);
  std::vector<Node> explored;
  std::vector<Node> results;
  
  frontier.push(Node{ w->start });
  while(!frontier.empty()){
    Node thisNode = frontier.top();
    frontier.pop();
    
    //if it found solution
    if(thisNode.back() == w->end){
      results.push_back(thisNode);
      continue;
    }
    
    std::vector<Node> nodes = expand(thisNode);
    
    for(auto it = nodes.begin(); it != nodes.end(); ++it){
      if(std::find(explored.begin(), explored.end(), *it) != explored.end()){
        continue;
      }
      if(std::find((*it).begin(), (*it).end(), (*it).back()) != --(*it).end()){
        continue;
      }
      frontier.push(*it);
    }
    explored.push_back(thisNode);
  }
  return results;
}

void AI::generate(){
  std::mt19937::result_type seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 engine(seed);
  std::uniform_int_distribution<unsigned int> dDist(0, 3);//direction distribution
  std::uniform_int_distribution<unsigned int> sDist(0, 1);//start distribution
  std::vector<position> frontier;
  std::vector<position> explored;
  
  //initialize world
  for(unsigned int i = 0; i < w->depth; ++i){
    //perimeter walls
    for(unsigned int j = 0; j < w->width; ++j){
      w->maze[j][0][i] = WALL;
      w->maze[j][w->height - 1][i] = WALL;
    }
    for(unsigned int j = 0; j < w->height; ++j){
      w->maze[0][j][i] = WALL;
      w->maze[w->width - 1][j][i] = WALL;
    }
    
    //insert alternating wall and open space
    for(unsigned int j = 1; j < w->width - 1; ++j){
      for(unsigned int k = 1; k < w->height - 1; ++k){
        if(j % 2 == 0 || k % 2 == 0){
          w->maze[j][k][i] = WALL;
        } else {
          w->maze[j][k][i] = EMPTY;
        }
      }
    }
  }
  
  //select start position
  //0 selects side walls
  if(sDist(engine) == 0){
    std::uniform_int_distribution<unsigned int> iDist(0, w->width);//idx distribution
    std::uniform_int_distribution<unsigned int> deepDist(0, w->depth);//depth distribution
    unsigned int W = iDist(engine);
    unsigned int H;
    unsigned int D = deepDist(engine);
    if(sDist(engine) == 0){
      H = 0;
    } else {
      H = w->height;
    }
    w->maze[W][H][D] = START;
  } else {
    std::uniform_int_distribution<unsigned int> iDist(0, w->height);//idx distribution
    std::uniform_int_distribution<unsigned int> deepDist(0, w->depth);//depth distribution
    unsigned int H = iDist(engine);
    unsigned int W;
    unsigned int D = deepDist(engine);
    if(sDist(engine) == 0){
      W = 0;
    } else {
      W = w->width;
    }
    w->maze[W][H][D] = START;
  }
  
  if(sDist(engine) == 0){
    std::uniform_int_distribution<unsigned int> iDist(0, w->width);//idx distribution
    std::uniform_int_distribution<unsigned int> deepDist(0, w->depth);//depth distribution
    unsigned int W = iDist(engine);
    unsigned int H;
    unsigned int D = deepDist(engine);
    if(sDist(engine) == 0){
      H = 0;
    } else {
      H = w->height;
    }
    w->maze[W][H][D] = END;
  } else {
    std::uniform_int_distribution<unsigned int> iDist(0, w->height);//idx distribution
    std::uniform_int_distribution<unsigned int> deepDist(0, w->depth);//depth distribution
    unsigned int H = iDist(engine);
    unsigned int W;
    unsigned int D = deepDist(engine);
    if(sDist(engine) == 0){
      W = 0;
    } else {
      W = w->width;
    }
    w->maze[W][H][D] = END;
  }
  
  
  for(unsigned int k = 0; k < w->depth; ++k){
    for(unsigned int i = 0; i < w->width; ++i){
      for(unsigned int j = 0; j < w->height; ++j){
        switch(w->maze[i][j][k]){
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
