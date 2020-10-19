#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include <DIPaCUS/base/Shapes.h>

#include "RoundedRect.h"
#include "energy.h"

int main(int argc, char* argv[]){
  using namespace DGtal::Z2i;

  double width = std::atof(argv[1]);
  double height = std::atof(argv[2]);
  double radius = std::atof(argv[3]);
  double gap = std::atof(argv[4]);
  double alpha = std::atof(argv[5]);
  double gridStep = std::atof(argv[6]);

  std::string energy = argv[7];


  RoundedRect roundedRectLeft(0,0,width,height,radius);
  RoundedRect roundedRectRight(width+gap,0,width,height,radius);
  RoundedRect roundedRect(0,0,2*width+gap,height,radius);

  DigitalSet shapeRectLeft = DIPaCUS::Shapes::digitizeShape(roundedRectLeft,gridStep);
  DigitalSet shapeRectRight = DIPaCUS::Shapes::digitizeShape(roundedRectRight,gridStep);
  DigitalSet shapeRect = DIPaCUS::Shapes::digitizeShape(roundedRect,gridStep);


  double beta=1.0;
  double estimationRadius = std::min(width,height)/2.0;

  if(energy=="disconnected-perimeter"){
    std::cout << 2*perimeter(shapeRectLeft,gridStep);
  }else if(energy=="connected-perimeter"){
    std::cout << perimeter(shapeRect,gridStep);
  }else if(energy=="disconnected-elastica"){
    std::cout << 2*elastica(shapeRectLeft,gridStep,estimationRadius,alpha,beta);
  }else if(energy=="connected-elastica"){
    std::cout << elastica(shapeRect,gridStep,estimationRadius,alpha,beta);
  }else{
    throw std::runtime_error("Unknown energy");
  }


  return 0;
}