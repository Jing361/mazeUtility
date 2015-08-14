#ifndef __AI_H__
#define __AI_H__

#include<vector>
#include<cmath>
#include<algorithm>
#include"stuff.hh"
#include"world.hh"

class AI{
private:
//use a pointer to prevent world destructor running
  world* w;
  
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
      return sqrt(((std::get<0>(x) - std::get<0>(y)) * (std::get<0>(x) - std::get<0>(y))) +
                  ((std::get<1>(x) - std::get<1>(y)) * (std::get<1>(x) - std::get<1>(y))) +
                  ((std::get<2>(x) - std::get<2>(y)) * (std::get<2>(x) - std::get<2>(y))));
    }
  };
  
  std::vector<Node> expandSearch(Node node);
  std::vector<position> expandGenerate(position pos);
  
public:
  AI(world* x):
    w(x){
  }

  std::vector<Node> operator()();
  void generate();
};

#endif
