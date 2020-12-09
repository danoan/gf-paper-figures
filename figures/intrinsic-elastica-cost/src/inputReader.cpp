#include <unistd.h>

#include <cstring>
#include <iostream>

#include "InputData.h"

void usage(char* argv[]) {
  std::cerr
      << "Usage: " << argv[0] << " OutputFolder \n"
      << " [-S shape name (default: ball) (triangle, square, "
         "pentagon, ball, ellipse, wave, bean)] \n"
      << " [-K curvature estimator (default: MDCA) (MDCA,II)] \n"
      << " [-L local length estimator (default: MDSS) (MDSS,LMDSS)] \n"
      << " [-h grid resolution (default: 1.0)] \n"
      << " [-w ring width (default: 2.0)] \n"
      << " [-a length penalization coefficient (default: 1.0)] \n"
      << " [-b squared curvature penalization coefficient (default: 1.0)] \n"
      << " [-i max iterations (default: 10)] \n"
      << " [-O only squared curvature \n"
      << " [-d display maps] \n";
}

InputData readInput(int argc, char* argv[]) {
  if (argc < 2) {
    usage(argv);
    exit(0);
  }

  InputData id;

  int opt;
  while ((opt = getopt(argc, argv, "S:K:L:h:w:a:b:i:Od")) != -1) {
    switch (opt) {
      case 'S': {
        id.shapeName = optarg;
        break;
      }
      case 'K': {
        if (strcmp(optarg, "II") == 0) {
          id.curvatureEstimator = InputData::II;
        } else if (strcmp(optarg, "MDCA") == 0) {
          id.curvatureEstimator = InputData::MDCA;
        } else {
          throw std::runtime_error("Curvature estimator not recognized!");
        }
        break;
      }
      case 'L': {
        if (strcmp(optarg, "MDSS") == 0) {
          id.lengthEstimator = InputData::MDSS;
        } else if (strcmp(optarg, "LMDSS") == 0) {
          id.lengthEstimator = InputData::LMDSS;
        } else {
          throw std::runtime_error("Length estimator not recognized!");
        }
        break;
      }
      case 'h': {
        id.h = std::atof(optarg);
        break;
      }
      case 'w': {
        id.ringWidth = std::atof(optarg);
        break;
      }
      case 'a': {
        id.alpha = std::atof(optarg);
        break;
      }
      case 'b': {
        id.beta = std::atof(optarg);
        break;
      }
      case 'i': {
        id.maxIt = std::atoi(optarg);
        break;
      }
      case 'O': {
        id.onlySquaredCurvature = true;
        break;
      }      
      case 'd': {
        id.displayMaps = true;
        break;
      }
      default: {
        usage(argv);
        break;
      }
    }
  }

  id.outputFolder = argv[optind++];

  return id;
}