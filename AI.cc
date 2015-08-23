#include<queue>
#include<random>
#include<chrono>
#include"AI.hh"

#include<iostream>

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
  std::vector<std::pair<position, position>> frontier;
  std::vector<position> explored;
  
  w->clear();
  
  while(w->start == w->end){
    w->placeEnds();
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

  frontier.push_back(std::pair<position, position>(w->start, temp));
  while(!frontier.empty()){
    std::uniform_int_distribution<unsigned int> dDist(0, frontier.size() - 1);
    unsigned int idx = dDist(engine);
    std::pair<position, position> thisNode = frontier[idx];
    coord x1 = std::get<0>(thisNode.first);
    coord x2 = std::get<0>(thisNode.second);
    coord y1 = std::get<1>(thisNode.first);
    coord y2 = std::get<1>(thisNode.second);
    coord z1 = std::get<2>(thisNode.first);
    coord z2 = std::get<2>(thisNode.second);
    
    frontier.erase(frontier.begin() + idx);
    
    if(std::find(explored.begin(), explored.end(), thisNode.second) != explored.end()){
      continue;
    }
    if(x1 == x2 && y1 == y2 && z1 == z2){
      continue;
    }
    std::vector<position> nodes = expandGenerate(thisNode.second);
    for(auto it = nodes.begin(); it != nodes.end(); ++it){
      //if new node is not already explored, add it to be explored
      if(std::find(explored.begin(), explored.end(), *it) == explored.end()){
        frontier.push_back(std::pair<position, position>(thisNode.second, *it));
      }
    }
    explored.push_back(thisNode.second);

    //remove wall
    w->maze[(x1 + x2) / 2][y1][z1] = EMPTY;
    w->maze[x1][(y1 + y2) / 2][z1] = EMPTY;
    if(z1 != z2){
      w->maze[x1][y1][z1] = STAIR;
      w->maze[x1][y1][z2] = STAIR;
    }
  }
  //put start back.  it will have been deleted.
  //  also put end back.  it sometimes gets deleted
  w->maze[std::get<0>(w->start)][std::get<1>(w->start)][std::get<2>(w->start)] = START;
  w->maze[std::get<0>(w->end)][std::get<1>(w->end)][std::get<2>(w->end)] = END;
}
