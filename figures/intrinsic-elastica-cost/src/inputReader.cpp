#include "InputData.h"

InputData readInput(int argc, char* argv[]) {
  InputData id;

  id.shapeName = argv[1];
  id.h = std::atof(argv[2]);
  id.M = std::atof(argv[3]);
  id.ringWidth = std::atof(argv[4]);

  return id;
}