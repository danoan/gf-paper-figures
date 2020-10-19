#include <DGtal/helpers/StdDefs.h>
#include <DGtal/shapes/parametric/Ball2D.h>
#include <opencv2/core.hpp>

#include <DIPaCUS/base/Shapes.h>
#include <DIPaCUS/base/Representation.h>

using namespace DGtal::Z2i;

int main(int argc, char* argv[]){
  double radius=std::atof(argv[1]);
  std::string outputFilepath=argv[2];
  double h = std::atof(argv[3]);

  DGtal::Ball2D<Space> ballShape(0,0,radius);
  DigitalSet ballDS = DIPaCUS::Shapes::digitizeShape(ballShape,h);

  Point dims = ballDS.domain().upperBound() - ballDS.domain().lowerBound() + Point(1,1);
  cv::Mat cvImg = cv::Mat::zeros(dims[1],dims[0],CV_8UC1);
  DIPaCUS::Representation::digitalSetToCVMat(cvImg,ballDS);

  cv::imwrite(outputFilepath,cvImg);
  return 0;
}