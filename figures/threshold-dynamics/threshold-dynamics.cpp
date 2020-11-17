#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include <DIPaCUS/derivates/Misc.h>

#include <graph-flow/utils/digital.h>
#include <graph-flow/utils/string.h>
#include <graph-flow/utils/display.h>

#include <boost/filesystem.hpp>
#include <iostream>

using namespace DGtal::Z2i;

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

  DigitalSet shape = GraphFlow::Utils::Digital::resolveShape(shapeName, gridStep);
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