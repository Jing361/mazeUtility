#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include"stuff.hh"
#include"world.hh"
#include"AI.hh"

int main(int argc, char** argv){
  std::string fname("maze");
  if(argc == 2){
    fname = argv[1];
  }
  std::ifstream maze(fname);
  std::string line;
  std::string mazeStr;
  while(std::getline(maze, line)){
    mazeStr += line + '\n';
  }
  //world w(mazeStr);
  world w(13, 13, 1);
  AI ai(&w);
  ai.generate();
  w.print();
  
  std::vector<Node> results = ai();
  //3 is subtracted from the size to account for the initial position, 
  //  and moves for each moving into the end positoin and out of the start position
  std::cout << "difficulty\t" << (double)((*results.begin()).size() - 3) / distance(w.start, w.end) << std::endl;
  std::cout << std::endl;
  
  for(auto itr = results.begin(); itr != results.end(); ++itr){
    std::cout << (*itr).size() << std::endl;
    
    coord lastX = std::get<0>(*(*itr).begin());
    coord lastY = std::get<1>(*(*itr).begin());
    coord lastZ = std::get<2>(*(*itr).begin());
    std::cout << "x " << lastX << " y " << lastY << " z " << lastZ << std::endl;
    for(auto it = ++(*itr).begin(); it != (*itr).end(); ++it){
      std::cout << "x " << std::get<0>(*it) << " y " << std::get<1>(*it) << " z " << std::get<2>(*it);
      unsigned int val = std::get<0>(*it) - lastX;
      switch(val){
      case -1:
        std::cout << " \tup";
      break;
      case  1:
        std::cout << " \tdown";
      break;
      };
      val = std::get<1>(*it) - lastY;
      switch(val){
      case -1:
        std::cout << " \tleft";
      break;
      case  1:
        std::cout << " \tright";
      break;
      };
      val = std::get<2>(*it) - lastZ;
      switch(val){
      case -1:
        std::cout << " \tdownstairs";
      break;
      case  1:
        std::cout << " \tupstairs";
      break;
      };
      std::cout << std::endl;
      
      lastX = std::get<0>(*it);
      lastY = std::get<1>(*it);
      lastZ = std::get<2>(*it);
    }
    std::cout << std::endl;
  }
  return 0;
}
