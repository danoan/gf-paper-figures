#include <string>
#include <DGtal/helpers/StdDefs.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "graph-flow/io/seed/GrabCutObject.h"
#include "graph-flow/contour-correction/model/image/DataDistribution.h"

using namespace GraphFlow::IO::Seed;
using namespace GraphFlow::ContourCorrection::Image;

int main(int argc, char* argv[]){
  std::string grabcutObjectFilePath = argv[1];
  std::string segMaskOutputPath = argv[2];
  int grabcutIterations = std::atoi(argv[3]);

  GrabCutObject gco = read(grabcutObjectFilePath);
  DataDistribution DD(gco, grabcutIterations);


  cv::imwrite(segMaskOutputPath,DD.gco.segMask);

  return 0;
}