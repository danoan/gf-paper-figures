#include "InputData.h"

InputData readInput(int argc, char* argv[]) {
  InputData id;

  id.shapeName = argv[1];
  id.h = std::atof(argv[2]);
  id.M = std::atof(argv[3]);
  id.m = std::atof(argv[4]);
  id.ringWidth = std::atof(argv[5]);
  id.alpha = std::atof(argv[6]);
  id.beta = std::atof(argv[7]);
  id.maxIt = std::atoi(argv[8]);

  return id;
}