#include<cmath>
#include"stuff.hh"

double distance(position x, position y){
  return sqrt(((std::get<0>(x) - std::get<0>(y)) * (std::get<0>(x) - std::get<0>(y))) +
              ((std::get<1>(x) - std::get<1>(y)) * (std::get<1>(x) - std::get<1>(y))) +
              ((std::get<2>(x) - std::get<2>(y)) * (std::get<2>(x) - std::get<2>(y))));
}
