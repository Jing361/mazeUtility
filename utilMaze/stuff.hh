#ifndef __STUFF_HH__
#define __STUFF_HH__

#include<tuple>
#include<vector>

enum space{ WALL, EMPTY, START, END, STAIR };
enum action{ UP, DOWN, LEFT, RIGHT, VERTU, VERTD };

typedef unsigned int coord;
typedef std::tuple<coord, coord, coord> position;
typedef std::vector<position> Node;

#endif
