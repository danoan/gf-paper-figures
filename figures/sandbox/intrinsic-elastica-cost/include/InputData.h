#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#include <string>

struct InputData {
  enum CurvatureEstimator { II, MDCA };
  enum LengthEstimator { MDSS, LMDSS };

  InputData() {
    shapeName = "ball";

    curvatureEstimator = II;
    lengthEstimator = MDSS;

    h = 1.0;
    ringWidth = 2;
    alpha = 1;
    beta = 1;

    maxIt = 10;

    displayMaps = false;
    onlySquaredCurvature=false;
    outputFolder = "";
  }

  std::string shapeName;
  CurvatureEstimator curvatureEstimator;
  LengthEstimator lengthEstimator;

  double h;
  double ringWidth;
  double alpha;
  double beta;

  int maxIt;

  bool displayMaps;
  bool onlySquaredCurvature;
  std::string outputFolder;
};

InputData readInput(int argc, char* argv[]);

#endif