#ifndef __WORLD_HH__
#define __WORLD_HH__

#include<string>
#include"stuff.hh"

class world{
private:
  space*** maze;
  
  void place(space sp, position& pos);

public:
  world(unsigned int x, unsigned int y, unsigned int z);
  world(std::string mazeStr);
  ~world();
  
  void print();
  void clear();
  void placeEnds();
  space& getSpace(coord x, coord y, coord z);
  space& getSpace(position pos);
  
  unsigned int width;
  unsigned int height;
  unsigned int depth;
  position start;
  position end;
};

#endif
