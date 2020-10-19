#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include <DIPaCUS/base/Shapes.h>
#include <boost/filesystem.hpp>

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
  std::string outputFilepath = argv[8];

  int imageHeight = std::atoi(argv[9]);

  boost::filesystem::path p(outputFilepath);
  boost::filesystem::create_directories(p.remove_filename());


  RoundedRect roundedRectLeft(0,0,width,height,radius);
  RoundedRect roundedRectRight(width+gap,0,width,height,radius);
  RoundedRect roundedRect(0,0,2*width+gap,height,radius);

  DigitalSet shapeRectLeft = DIPaCUS::Shapes::digitizeShape(roundedRectLeft,gridStep);
  DigitalSet shapeRectRight = DIPaCUS::Shapes::digitizeShape(roundedRectRight,gridStep);
  DigitalSet shapeRect = DIPaCUS::Shapes::digitizeShape(roundedRect,gridStep);


  double beta=1.0;
  double estimationRadius = std::min(width,height)/2.0;

  double connected,disconnected;
  if(energy=="perimeter"){
    connected = perimeter(shapeRect,gridStep);
    disconnected = 2*perimeter(shapeRectLeft,gridStep);
  }else if(energy=="elastica"){
    connected = elastica(shapeRect,gridStep,estimationRadius,alpha,beta);
    disconnected = 2*elastica(shapeRectLeft,gridStep,estimationRadius,alpha,beta);
  }else{
    throw std::runtime_error("Unknown energy");
  }

  const Domain& domain = shapeRect.domain();
  Point dims = domain.upperBound() - domain.lowerBound() + Point(1,1);

  DigitalSet shapeRectLeftND(domain);
  shapeRectLeftND.insert(shapeRectLeft.begin(),shapeRectLeft.end());

  DigitalSet shapeRectRightND(domain);
  shapeRectRightND.insert(shapeRectRight.begin(),shapeRectRight.end());

  DigitalSet disconnectedShape(domain);
  disconnectedShape.insert(shapeRectLeftND.begin(),shapeRectLeftND.end());
  disconnectedShape.insert(shapeRectRightND.begin(),shapeRectRightND.end());

  DigitalSet connectedShape(domain);
  connectedShape.insert(shapeRect.begin(),shapeRect.end());


  cv::Mat cvImgBW = cv::Mat::zeros( dims[1],dims[0],CV_8UC1);
  DIPaCUS::Representation::digitalSetToCVMat(cvImgBW,disconnectedShape);

  DigitalSet contour(domain);
  cv::Mat cvContour = cv::Mat::zeros( dims[1],dims[0],CV_8UC1);
  double contourThickness = 0.125/gridStep;
  if(connected<disconnected){
    DIPaCUS::Misc::digitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(contour,connectedShape,( (unsigned int) contourThickness  ));
  }else{
    DIPaCUS::Misc::digitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(contour,shapeRectLeftND,( (unsigned int) contourThickness ));
    DIPaCUS::Misc::digitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(contour,shapeRectRightND,( (unsigned int) contourThickness ));
  }

  cv::Vec3b yellowGreen(153,208,85);
  cv::Vec3b lightGray(204,204,204);
  cv::Vec3b chocolateWeb(221,119,51);
  cv::Vec3b white(255,255,255);

  cv::Mat cvImg = cv::Mat::zeros( dims[1],dims[0],CV_8UC3);
  cvImg = white;
  cvImg.setTo(lightGray,cvImgBW);

  DIPaCUS::Representation::digitalSetToCVMat(cvContour,contour);
  cvImg.setTo( yellowGreen,cvContour);

  Point border((int) (2.0/gridStep), (int) (2.0/gridStep) );
  int imageWidth = (int) ( ( (2*(width+radius)+gap)/gridStep + 2*border[0]) );
  int xAnchor = border[0];
  int yAnchor = imageHeight - border[1];

  cv::Mat finalImage = cv::Mat::zeros( imageHeight,imageWidth, CV_8UC3);
  finalImage = white;


  int ir=0;
  for(int r=cvImg.rows-1;r>=0;--r){
    for(int c=0;c<cvImg.cols;++c){
      finalImage.at<cv::Vec3b>(yAnchor-ir,xAnchor+c) = cvImg.at<cv::Vec3b>(r,c);
    }
    ir++;
  }

  cv::imwrite(outputFilepath,finalImage);


  return 0;
}