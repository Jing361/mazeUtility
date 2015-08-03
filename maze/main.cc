#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<queue>
#include<tuple>
#include<algorithm>

enum space{ WALL, EMPTY, START, END };
enum action{ UP, DOWN, LEFT, RIGHT };

typedef std::pair<unsigned int, unsigned int> position;
typedef std::vector<position> Node;

struct world{
  space** maze;
  unsigned int width;
  unsigned int height;
};

std::vector<Node> expand(Node node, world w){
  std::vector<Node> ret;
  unsigned int x = node.back().first;
  unsigned int y = node.back().second;
  if(x > 0 && w.maze[x - 1][y] != WALL){
    Node n = node;
    n.push_back(position(x - 1, y));
    ret.push_back(n);
  }
  if(y > 0 && w.maze[x][y - 1] != WALL){
    Node n = node;
    n.push_back(position(x, y - 1));
    ret.push_back(n);
  }
  if(x < w.width - 1 && w.maze[x + 1][y] != WALL){
    Node n = node;
    n.push_back(position(x + 1, y));
    ret.push_back(n);
  }
  if(y < w.height - 1 && w.maze[x][y + 1] != WALL){
    Node n = node;
    n.push_back(position(x, y + 1));
    ret.push_back(n);
  }
  
  return ret;
}

int main(){
  std::queue<Node> frontier;
  std::vector<Node> results;
  std::vector<Node> explored;
  world w;
  position start;
  position end;
  
  std::string mazeStr = "+-+-+-+-+-+\n"
                        "S |       |\n"
                        "+ +-+-+ + +\n"
                        "| |     | |\n"
                        "+ + + + + +\n"
                        "|   | |   |\n"
                        "+-+-+ +-+-+\n"
                        "|     |   |\n"
                        "+ + +-+ + +\n"
                        "| |     | E\n"
                        "+-+-+-+-+-+\n";
  std::stringstream ss(mazeStr);
  std::string line;
  
  std::cout << mazeStr << std::endl;
  std::getline(ss, line);
  w.width = line.size();
  w.height = mazeStr.size() / (line.size() + 1);
  
  w.maze = new space*[w.width];
  for(unsigned int i = 0; i < w.width; ++i){
    w.maze[i] = new space[w.height];
  }

  //reset ss
  ss.str("");
  ss.clear();
  ss.str(mazeStr);
  
  unsigned int W = 0;
  unsigned int H = 0;
  while(std::getline(ss, line)){
    for(unsigned int i = 0; i < w.width; ++i){
      switch(line[i]){
      case ' ':
        w.maze[W][H] = EMPTY;
        break;
      case 'S':
        w.maze[W][H] = START;
        start = position(W, H);
        break;
      case 'E':
        w.maze[W][H] = END;
        end = position(W, H);
        break;
      default:
        w.maze[W][H] = WALL;
        break;
      };
      ++W;
    }
    ++H;
    W = 0;
  }
  
  frontier.push(Node{ start });
  while(!frontier.empty()){
    Node thisNode = frontier.front();
    frontier.pop();
    
    std::vector<Node> nodes = expand(thisNode, w);
    
    //if it found solution
    if(thisNode.back() == end){
      results.push_back(thisNode);
      continue;
    }
    
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
  
  for(auto itr = results.begin(); itr != results.end(); ++itr){
    for(auto it = (*itr).begin(); it != (*itr).end(); ++it){
      std::cout << "x " << (*it).first << " y " << (*it).second << std::endl;
    }
    std::cout << std::endl;
  }
  
  for(unsigned int i = 0; i < w.width; ++i){
    delete[] w.maze[i];
  }
  delete[] w.maze;
  return 0;
}