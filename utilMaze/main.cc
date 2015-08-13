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
  std::cout << mazeStr << std::endl;
  world w(mazeStr);
  AI ai(&w);
  ai.generate();
  /*
  std::vector<Node> results = ai();
  
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
        std::cout << " \tleft";
      break;
      case  1:
        std::cout << " \tright";
      break;
      };
      val = std::get<1>(*it) - lastY;
      switch(val){
      case -1:
        std::cout << " \tup";
      break;
      case  1:
        std::cout << " \tdown";
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
  }*/
  return 0;
}
