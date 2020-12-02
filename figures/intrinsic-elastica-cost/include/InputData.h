#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#include <string>

struct InputData{
  double h;
  std::string shapeName;
  
  double M;
  double m;
  double ringWidth;
  double alpha;
  double beta;
  int maxIt;
};

InputData readInput(int argc, char* argv[]);

#endif