#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<queue>
#include<tuple>
#include<algorithm>
#include<cmath>

enum space{ WALL, EMPTY, START, END };
enum action{ UP, DOWN, LEFT, RIGHT };

typedef std::pair<unsigned int, unsigned int> position;
typedef std::vector<position> Node;

class world{
public:
  world(std::string mazeStr){
    std::stringstream ss(mazeStr);
    std::string line;
    
    std::getline(ss, line);
    width = line.size();
    height = mazeStr.size() / (line.size() + 1);
    
    maze = new space*[width];
    for(unsigned int i = 0; i < width; ++i){
      maze[i] = new space[height];
    }

    //reset ss
    ss.str("");
    ss.clear();
    ss.str(mazeStr);
    
    unsigned int W = 0;
    unsigned int H = 0;
    while(std::getline(ss, line)){
      for(unsigned int i = 0; i < width; ++i){
        switch(line[i]){
        case ' ':
          maze[W][H] = EMPTY;
          break;
        case 'S':
          maze[W][H] = START;
          start = position(W, H);
          break;
        case 'E':
          maze[W][H] = END;
          end = position(W, H);
          break;
        case '|':
        case '+':
        default:
          maze[W][H] = WALL;
          break;
        };
        ++W;
      }
      ++H;
      W = 0;
    }
  }
  ~world(){
    for(unsigned int i = 0; i < width; ++i){
      delete[] maze[i];
    }
    delete[] maze;
  }
  
  space** maze;
  unsigned int width;
  unsigned int height;
  position start;
  position end;
};

class AI{
private:
//use a pointer to prevent world destructor running
  const world* w;
  
  class heuristic{
  public:
    const world* w;
    
    heuristic(const world* W):
      w(W){
    }
    
    int heur(const Node& node){
      return node.size() + distance(node.back(), w->end);
    }
    
    bool operator()(const Node& x, const Node& y){
      return heur(x) > heur(y);
    }
    
    double distance(position x, position y){
    return sqrt(((x.first - y.first) * (x.first - y.first)) + ((x.second- y.second) * (x.second - y.second)));
  }
  };
  
  std::vector<Node> expand(Node node){
    std::vector<Node> ret;
    unsigned int x = node.back().first;
    unsigned int y = node.back().second;
    if(x > 0 && w->maze[x - 1][y] != WALL){
      Node n = node;
      n.push_back(position(x - 1, y));
      ret.push_back(n);
    }
    if(y > 0 && w->maze[x][y - 1] != WALL){
      Node n = node;
      n.push_back(position(x, y - 1));
      ret.push_back(n);
    }
    if(x < w->width - 1 && w->maze[x + 1][y] != WALL){
      Node n = node;
      n.push_back(position(x + 1, y));
      ret.push_back(n);
    }
    if(y < w->height - 1 && w->maze[x][y + 1] != WALL){
      Node n = node;
      n.push_back(position(x, y + 1));
      ret.push_back(n);
    }
    
    return ret;
  }
  
public:
  AI(const world* x):
    w(x){
  }

  std::vector<Node> operator()(){
    heuristic h(w);
    std::priority_queue<Node, std::vector<Node>, heuristic> frontier(h);
    //std::queue<Node> frontier;
    std::vector<Node> explored;
    std::vector<Node> results;
    
    frontier.push(Node{ w->start });
    while(!frontier.empty()){
      Node thisNode = frontier.top();
      frontier.pop();
      
      //if it found solution
      if(thisNode.back() == w->end){
        results.push_back(thisNode);
        break;
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
};

int main(){
  std::string mazeStr = "+-+-+-+-+-+\n"
                        "S |       |\n"
                        "+ +-+-+ + +\n"
                        "| |     | |\n"
                        "+ + + + + +\n"
                        "|   | |   |\n"
                        //"+-+-+ +-+-+\n"
                        "+-+-+ + +-+\n"
                        "|     |   |\n"
                        "+ + +-+ + +\n"
                        "| |     | E\n"
                        "+-+-+-+-+-+\n";
  std::cout << mazeStr << std::endl;
  world w(mazeStr);
  AI ai(&w);
  
  std::vector<Node> results = ai();
  
  for(auto itr = results.begin(); itr != results.end(); ++itr){
    std::cout << (*itr).size() << std::endl;
    for(auto it = (*itr).begin(); it != (*itr).end(); ++it){
      std::cout << "x " << (*it).first << " y " << (*it).second << std::endl;
    }
    std::cout << std::endl;
  }
  return 0;
}