#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#include <string>

struct InputData{
  double h;
  std::string shapeName;
  
  double M;
  double ringWidth;
};

InputData readInput(int argc, char* argv[]);

#endif