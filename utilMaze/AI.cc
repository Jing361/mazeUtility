#include<queue>
#include"AI.hh"

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
