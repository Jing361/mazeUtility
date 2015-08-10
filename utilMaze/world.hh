#ifndef __WORLD_HH__
#define __WORLD_HH__

#include<string>
#include"stuff.hh"

class world{
public:
  world(std::string mazeStr);
  ~world();
  
  space*** maze;
  unsigned int width;
  unsigned int height;
  unsigned int depth;
  position start;
  position end;
};

#endif
