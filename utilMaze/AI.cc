#include<queue>
#include<random>
#include<chrono>
#include"AI.hh"

std::vector<Node> AI::expandSearch(Node node){
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

std::vector<position> AI::expandGenerate(position pos){
  std::vector<position> ret;
  coord x = std::get<0>(pos);
  coord y = std::get<1>(pos);
  coord z = std::get<2>(pos);
  
  if(x > 1){
    ret.push_back(position(x - 2, y, z));
  }
  if(y > 1){
    ret.push_back(position(x, y - 2, z));
  }
  if(z > 0){
    ret.push_back(position(x, y, z - 1));
  }
  if(x < w->width - 2){
    ret.push_back(position(x + 2, y, z));
  }
  if(y < w->height - 2){
    ret.push_back(position(x, y + 2, z));
  }
  if(z < w->depth - 1){
    ret.push_back(position(x, y, z + 1));
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
    
    std::vector<Node> nodes = expandSearch(thisNode);
    
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
  std::uniform_int_distribution<unsigned int> sDist(0, 1);//start distribution
  std::vector<std::pair<position, position>> frontier;
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
    std::uniform_int_distribution<unsigned int> iDist(0, (w->width - 1) / 2);//idx distribution
    std::uniform_int_distribution<unsigned int> deepDist(0, w->depth - 1);//depth distribution
    unsigned int W = (iDist(engine) * 2) - 1;
    unsigned int H;
    unsigned int D = deepDist(engine);
    if(sDist(engine) == 0){
      H = 0;
    } else {
      H = w->height - 1;
    }
    w->maze[W][H][D] = START;
    w->start = position(W, H, D);
  } else {
    std::uniform_int_distribution<unsigned int> iDist(0, (w->height - 1) / 2);//idx distribution
    std::uniform_int_distribution<unsigned int> deepDist(0, w->depth - 1);//depth distribution
    unsigned int H = (iDist(engine) * 2) - 1;
    unsigned int W;
    unsigned int D = deepDist(engine);
    if(sDist(engine) == 0){
      W = 0;
    } else {
      W = w->width - 1;
    }
    w->maze[W][H][D] = START;
    w->start = position(W, H, D);
  }
  
  //select end position
  //0 selects side walls
  if(sDist(engine) == 0){
    std::uniform_int_distribution<unsigned int> iDist(0, (w->width - 1) / 2);//idx distribution
    std::uniform_int_distribution<unsigned int> deepDist(0, w->depth - 1);//depth distribution
    unsigned int W = (iDist(engine) * 2) - 1;
    unsigned int H;
    unsigned int D = deepDist(engine);
    if(sDist(engine) == 0){
      H = 0;
    } else {
      H = w->height - 1;
    }
    w->maze[W][H][D] = END;
    w->end = position(W, H, D);
  } else {
    std::uniform_int_distribution<unsigned int> iDist(0, (w->height - 1) / 2);//idx distribution
    std::uniform_int_distribution<unsigned int> deepDist(0, w->depth - 1);//depth distribution
    unsigned int H = (iDist(engine) * 2) - 1;
    unsigned int W;
    unsigned int D = deepDist(engine);
    if(sDist(engine) == 0){
      W = 0;
    } else {
      W = w->width - 1;
    }
    w->maze[W][H][D] = END;
    w->end = position(W, H, D);
  }

  position temp = w->start;
  //push seed into maze
  if(std::get<0>(temp) == 0){
    std::get<0>(temp) = 1;
  } else if(std::get<0>(temp) == w->width - 1){
    std::get<0>(temp) = w->width - 2;
  }
  if(std::get<1>(temp) == 0){
    std::get<1>(temp) = 1;
  } else if(std::get<1>(temp) == w->height - 1){
    std::get<1>(temp) = w->height - 2;
  }

  frontier.push_back(std::pair<position, position>(temp, temp));
  while(!frontier.empty()){
    std::uniform_int_distribution<unsigned int> dDist(0, frontier.size() - 1);
    unsigned int idx = dDist(engine);
    std::pair<position, position> thisNode = frontier[idx];
    frontier.erase(frontier.begin() + idx);
    
    if(std::find(explored.begin(), explored.end(), thisNode.second) != explored.end()){
      continue;
    }
    std::vector<position> nodes = expandGenerate(thisNode.second);
    for(auto it = nodes.begin(); it != nodes.end(); ++it){
      if(std::find(explored.begin(), explored.end(), *it) != explored.end()){
        continue;
      }
      frontier.push_back(std::pair<position, position>(thisNode.second, *it));
    }
    explored.push_back(thisNode.second);
    
    //remove wall
    coord x1 = std::get<0>(thisNode.first);
    coord x2 = std::get<0>(thisNode.second);
    coord y1 = std::get<1>(thisNode.first);
    coord y2 = std::get<1>(thisNode.second);
    coord z1 = std::get<2>(thisNode.first);
    coord z2 = std::get<2>(thisNode.second);
    w->maze[(x1 + x2) / 2][y1][z1] = EMPTY;
    w->maze[x1][(y1 + y2) / 2][z1] = EMPTY;
    if(z1 != z2){
      w->maze[x1][y1][z1] = STAIR;
      w->maze[x1][y1][z2] = STAIR;
    }
  }
  w->print();
}
