#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include <DIPaCUS/derivates/Misc.h>

#include <graph-flow/utils/digital.h>
#include <graph-flow/utils/string.h>
#include <graph-flow/utils/display.h>

#include <boost/filesystem.hpp>
#include <iostream>

using namespace DGtal::Z2i;

enum ShapeType {
  Triangle,
  Square,
  Pentagon,
  Heptagon,
  Ball,
  Ellipse,
  Flower,
  Wave,
  Bean,
  UserDefined,
  NoType
};

struct Shape {
  Shape() : type(NoType), imagePath(""), name("") {}

  Shape(ShapeType type, const std::string& imagePath = "")
      : type(type), imagePath(imagePath) {
    if (type == ShapeType::Triangle)
      name = "triangle";
    else if (type == ShapeType::Square)
      name = "square";
    else if (type == ShapeType::Pentagon)
      name = "pentagon";
    else if (type == ShapeType::Heptagon)
      name = "heptagon";
    else if (type == ShapeType::Ball)
      name = "ball";
    else if (type == ShapeType::Ellipse)
      name = "ellipse";
    else if (type == ShapeType::Flower)
      name = "flower";
    else if (type == ShapeType::Wave)
      name = "wave";
    else if (type == ShapeType::Bean)
      name = "bean";
    else
      name = "user-defined";
  }

  ShapeType type;
  std::string imagePath;
  std::string name;
};

DigitalSet resolveShape(Shape shape, double gridStep) {
  int radius = 20;
  if (shape.type == ShapeType::Triangle)
    return DIPaCUS::Shapes::triangle(gridStep, 0, 0, radius);
  else if (shape.type == ShapeType::Square)
    return DIPaCUS::Shapes::square(gridStep, 0, 0, radius);
  else if (shape.type == ShapeType::Pentagon)
    return DIPaCUS::Shapes::NGon(gridStep, 0, 0, radius, 5);
  else if (shape.type == ShapeType::Heptagon)
    return DIPaCUS::Shapes::NGon(gridStep, 0, 0, radius, 7);
  else if (shape.type == ShapeType::Ball)
    return DIPaCUS::Shapes::ball(gridStep, 0, 0, radius);
  else if (shape.type == ShapeType::Flower)
    return DIPaCUS::Shapes::flower(gridStep, 0, 0, radius, radius / 2.0, 2);
  else if (shape.type == ShapeType::Ellipse)
    return DIPaCUS::Shapes::ellipse(gridStep, 0, 0, radius, radius / 2);
  else if (shape.type == ShapeType::Wave)
    return DIPaCUS::Shapes::wave(gridStep, 1200, radius * 3, radius * 6, 0.01);
  else if (shape.type == ShapeType::Bean)
    return DIPaCUS::Shapes::bean(gridStep, 0, 0, 0.1);
  else {
    cv::Mat img = cv::imread(shape.imagePath, CV_8UC1);
    Domain domain(DGtal::Z2i::Point(0, 0),
                  DGtal::Z2i::Point(img.cols - 1, img.rows - 1));
    DigitalSet ds(domain);
    DIPaCUS::Representation::CVMatToDigitalSet(ds, img, 1);
    return ds;
  }
}
DigitalSet resolveShape(const std::string& shape, double gridStep) {
  if (shape == "triangle")
    return resolveShape(Shape(ShapeType::Triangle), gridStep);
  else if (shape == "square")
    return resolveShape(Shape(ShapeType::Square), gridStep);
  else if (shape == "pentagon")
    return resolveShape(Shape(ShapeType::Pentagon), gridStep);
  else if (shape == "heptagon")
    return resolveShape(Shape(ShapeType::Heptagon), gridStep);
  else if (shape == "ball")
    return resolveShape(Shape(ShapeType::Ball), gridStep);
  else if (shape == "flower")
    return resolveShape(Shape(ShapeType::Flower), gridStep);
  else if (shape == "ellipse")
    return resolveShape(Shape(ShapeType::Ellipse), gridStep);
  else if (shape == "wave")
    return resolveShape(Shape(ShapeType::Wave), gridStep);
  else if (shape == "bean")
    return resolveShape(Shape(ShapeType::Bean), gridStep);
  else
    return resolveShape(Shape(ShapeType::UserDefined, shape), gridStep);
}

void getBand(const DigitalSet& shape, const Domain& domain, int optBand,
             DigitalSet& intPoints, DigitalSet& extPoints,
             const std::string& outputFolder) {
  auto dtl2Int =
      GraphFlow::Utils::Digital::interiorDistanceTransform(domain, shape);
  auto dtl2Ext =
      GraphFlow::Utils::Digital::exteriorDistanceTransform(domain, shape);

  for (auto p : domain) {
    if (dtl2Int(p) > 0 && dtl2Int(p) <= optBand) intPoints.insert(p);
    if (dtl2Ext(p) > 0 && dtl2Ext(p) <= optBand) extPoints.insert(p);
  }

  // DGtal::Board2D board;
  // board << domain
  //       << DGtal::CustomStyle(intPoints.className(),
  //                             new DGtal::CustomColors(DGtal::Color::Yellow,
  //                                                     DGtal::Color::Yellow))
  //       << intPoints
  //       << DGtal::CustomStyle(
  //              extPoints.className(),
  //              new DGtal::CustomColors(DGtal::Color::Red, DGtal::Color::Red))
  //       << extPoints;

  // board.saveSVG((outputFolder + "/intExtPoints.svg").c_str());
}

int main(int argc, char* argv[]) {
  std::string shapeName = argv[1];
  double gridStep = std::atof(argv[2]);
  double optBand = std::atof(argv[3]);
  double bRadius = std::atof(argv[4]);
  int maxIterations = std::atoi(argv[5]);
  std::string outputFolder = argv[6];

  boost::filesystem::path p(outputFolder);
  boost::filesystem::create_directories(p);

  DigitalSet shape = resolveShape(shapeName, gridStep);
  const Domain& domain = shape.domain();

  DigitalSet outSet(domain);
  outSet.insert(shape.begin(), shape.end());

  int iteration = 0;
  while (iteration < maxIterations) {
    GraphFlow::Utils::Display::saveDigitalSetAsImage(
        outSet, outputFolder + "/" +
                    GraphFlow::Utils::String::nDigitsString(iteration, 4) +
                    ".png");

    DigitalSet intPoints(domain);
    DigitalSet extPoints(domain);

    getBand(outSet, domain, optBand, intPoints, extPoints, outputFolder);
    DigitalSet optSet(domain);
    optSet += intPoints;
    optSet += extPoints;

    std::map<Point, double> pointRatioMap;

    DIPaCUS::Misc::DigitalBallIntersection DBI(bRadius, outSet);
    DigitalSet intersectionSet(domain);
    double ballPoints = DBI.digitalBall().size();
    for (auto p : optSet) {
      intersectionSet.clear();
      DBI(intersectionSet, p);
      double ratio = intersectionSet.size() / ballPoints;
      pointRatioMap[p] = ratio;
    }

    DigitalSet tempDiff(domain);
    DIPaCUS::SetOperations::setDifference(tempDiff, outSet, optSet);
    outSet.clear();
    outSet.insert(tempDiff.begin(),tempDiff.end());    

    for (auto pair : pointRatioMap) {
      Point p = pair.first;
      double ratio = pair.second;
      if (ratio >= 0.5) outSet.insert(p);
    }

    iteration++;
  }

  return 0;
}