#ifndef __WORLD_HH__
#define __WORLD_HH__

#include<string>
#include"stuff.hh"

class world{
public:
  world(unsigned int x, unsigned int y, unsigned int z);
  world(std::string mazeStr);
  ~world();
  
  void print();
  void clear();
  
  space*** maze;
  unsigned int width;
  unsigned int height;
  unsigned int depth;
  position start;
  position end;
};

#endif
