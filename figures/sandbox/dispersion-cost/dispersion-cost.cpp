#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/io/colormaps/GradientColorMap.h>
#include <DIPaCUS/base/Shapes.h>
#include <DIPaCUS/derivates/Misc.h>
#include <graph-flow/utils/display.h>
#include <graph-flow/utils/string.h>

#include <boost/filesystem.hpp>
#include <iostream>
#include <unordered_map>

#include "FlowGraph.h"
#include "LinelData.h"
#include "estimators.h"

using namespace DGtal::Z2i;

void testCurveInnerContour(const Domain& domain, const Curve& contour) {
  KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);

  auto srange = contour.getSCellsRange();
  auto iprange = contour.getInnerPointsRange();

  auto itS = srange.begin();
  auto itIP = iprange.begin();
  do {
    auto innerPixel = kspace.sDirectIncident(*itS, *kspace.sOrthDirs(*itS));
    if (kspace.sCoords(innerPixel) != *itIP)
      throw std::runtime_error("Inequivalence of ranges!");

    ++itS;
    ++itIP;
  } while (itS != srange.end());
}

/*
  I assume a Khalimsky space
  I assume the disk is center at linel (0,1).
*/
void setLinelsDisk(std::unordered_map<Point, double>& linelsDisk, double radius,double h) {
  Point origin(0, 1);
  DigitalSet disk = DIPaCUS::Shapes::ball(h, origin[0], origin[1], 2 * radius);

  for (auto p : disk) {
    if (abs(p[0]) % 2 == 0 && abs(p[1]) % 2 == 0) continue;
    if (abs(p[0]) % 2 == 1 && abs(p[1]) % 2 == 1) continue;

    Point d = p - origin;
    linelsDisk[d] = h*sqrt(pow(d[0], 2) + pow(d[1], 2));
  }
}

void updateDispersionMap(std::unordered_map<Point, LinelData>& dispersionMap,
                         const Point& centerLinel, double kernelValue, double h,
                         double alpha, double beta,
                         const std::unordered_map<Point, double>& linelsDisk,
                         const DigitalSet& intersectWith, bool inner) {
  for (auto p : linelsDisk) {
    Point linel = p.first + centerLinel;
    double weight = p.second;

    if (!intersectWith(linel)) continue;

    if (dispersionMap.find(linel) == dispersionMap.end()) {
      dispersionMap[linel] = LinelData();
    }

    if (inner)
      dispersionMap[linel].addCost(
          alpha * h + beta * h * pow(1.0 / (1.0 / kernelValue - weight), 2));
    else
      dispersionMap[linel].addCost(
          alpha * h + beta * h * pow(1.0 / (1.0 / kernelValue + weight), 2));
  }
}

DigitalSet getFilledKhalimsky(const DigitalSet& shape) {
  Curve contour;
  DIPaCUS::Misc::computeBoundaryCurve(contour, shape);

  KSpace kspace;
  kspace.init(shape.domain().lowerBound(), shape.domain().upperBound(), true);

  // LB*2+(1,1)-(1,1)
  // UB*2+(1,1)+(1,1)
  Domain KDomain(2 * shape.domain().lowerBound(),
                 2 * shape.domain().upperBound() + Point(2, 2));

  DigitalSet KShapeContour(KDomain);

  auto srange = contour.getSCellsRange();
  auto prange = contour.getPointsRange();

  auto itS = srange.begin();
  do {
    KShapeContour.insert(kspace.sKCoords(*itS));

    auto pointels = kspace.sLowerIncident(*itS);
    KShapeContour.insert(kspace.sKCoords(pointels[0]));

    ++itS;
  } while (itS != srange.end());

  Point innerPoint = 2 * (*shape.begin()) + Point(1, 1);
  DigitalSet KShape(KDomain);
  DIPaCUS::Misc::fillInterior(KShape, innerPoint, KShapeContour);

  return KShape;
}

DigitalSet prepareShape(double h, double radius, double borderSize = 20) {
  DigitalSet _disk = DIPaCUS::Shapes::ball(h, 0, 0, radius);
  // DigitalSet _disk = DIPaCUS::Shapes::bean(h);

  int b = (int)(borderSize / h);
  Point border(b, b);

  Domain domain(_disk.domain().lowerBound() - border,
                _disk.domain().upperBound() + border);
  DigitalSet disk(domain);
  disk.insert(_disk.begin(), _disk.end());

  return disk;
}

typedef std::unordered_map<Point, LinelData>& CostFunction;

void displayCost(const CostFunction& costFunction,
                 const std::string& outputFilepath, std::ostream& os) {
  typedef DGtal::GradientColorMap<double, DGtal::ColorGradientPreset::CMAP_JET>
      MyColorMap;

  double M = 0;
  double m = 100;

  for (auto pk : costFunction) {
    double cost = pk.second.cost();
    if (cost > M) M = cost;
    if (cost < m) m = cost;
  }

  os << std::fixed << std::setprecision(3) << "Min unit cost: " << m << "\n"
     << "Max unit cost: " << M << "\n";

  MyColorMap hmap(m, M);

  DGtal::Board2D board;
  for (auto pk : costFunction) {
    Point p = pk.first;
    double cost = pk.second.cost();

    if (cost > M) M = cost;
    if (cost < m) m = cost;

    board << DGtal::CustomStyle(p.className(),
                                new DGtal::CustomColors(hmap(cost), hmap(cost)))
          << p;
  }

  board.saveSVG(outputFilepath.c_str());
}

void displayLinelDisk() {
  // Domain KDomain(2*domain.lowerBound(),2*domain.upperBound()+Point(2,2));
  // DigitalSet linelsDiskDS(KDomain);
  // for(auto p:linelsDisk){
  //   linelsDiskDS.insert(p.first);
  // }
  // DGtal::Board2D board;
  // board << linelsDiskDS;
  // board.saveSVG( (outputFolder + "/linels-disk.svg").c_str());
  // exit(1);
}

int main(int argc, char* argv[]) {
  double h = std::atof(argv[1]);

  double radiusShape = std::atof(argv[2]);
  double radiusEstimation = std::atof(argv[3]);
  double radiusCost = std::atof(argv[4]);

  double alpha = std::atof(argv[5]);
  double beta = std::atof(argv[6]);

  int maxIt = std::atoi(argv[7]);
  std::string outputFolder = argv[8];
  boost::filesystem::create_directories(outputFolder);

  DigitalSet shape = prepareShape(h, radiusShape);
  const Domain& domain = shape.domain();

  GraphFlow::Utils::Display::saveDigitalSetAsImage(
      shape, outputFolder + "/result-0000.png");

  std::unordered_map<Point, double> linelsDisk;
  setLinelsDisk(linelsDisk, radiusCost,h);

  int nit = 1;
  while (nit <= maxIt) {
    // Curvature estimation
    Curve contour;
    DIPaCUS::Misc::computeBoundaryCurve(contour, shape);
    // testCurveInnerContour(domain,contour);

    std::vector<double> kV;
    // Estimators::curvatureII(kV, contour, domain, shape, radiusEstimation, h);
    Estimators::curvatureMDCA(kV, contour, domain, h);

    // Khalimsky inner and outer shapes
    DigitalSet eightContour(domain);
    DIPaCUS::Misc::digitalBoundary<
        DIPaCUS::Neighborhood::EightNeighborhoodPredicate>(eightContour, shape);
    DigitalSet shapeI = shape;
    for (auto p : eightContour) shapeI.erase(p);
    DigitalSet shapeO(domain);
    shapeO.assignFromComplement(shapeI);

    DigitalSet shapeKI = getFilledKhalimsky(shapeI);
    DigitalSet shapeKO(shapeKI.domain());
    shapeKO.assignFromComplement(shapeKI);

    // Creation of dispersionMap
    KSpace kspace;
    kspace.init(domain.lowerBound(), domain.upperBound(), true);

    auto srange = contour.getSCellsRange();
    auto itS = srange.begin();
    int i = 0;
    std::unordered_map<Point, LinelData> dispersionMap;
    do {
      double kernelValue = kV[i];
      Point linelKCoords = kspace.sKCoords(*itS);      

      updateDispersionMap(dispersionMap, linelKCoords, kernelValue, h, alpha,
                          beta, linelsDisk, shapeKI, true);
      updateDispersionMap(dispersionMap, linelKCoords, kernelValue, h, alpha,
                          beta, linelsDisk, shapeKO, false);

      ++itS;
      ++i;
      // break;
    } while (itS != srange.end());

    displayCost(dispersionMap,
                outputFolder + "/dispersion-map-" +
                    GraphFlow::Utils::String::nDigitsString(nit, 4) + ".svg",
                std::cout);

    FlowGraph fg(kspace, shape, dispersionMap);

    DGtal::Board2D board;
    DigitalSet sourcePoints(domain);
    DigitalSet targetPoints(domain);
    sourcePoints.insert(fg.sourcePoints.begin(), fg.sourcePoints.end());
    targetPoints.insert(fg.targetPoints.begin(), fg.targetPoints.end());

    board << domain
          << DGtal::CustomStyle(
                 targetPoints.className(),
                 new DGtal::CustomColors(DGtal::Color::Red, DGtal::Color::Red))
          << targetPoints
          << DGtal::CustomStyle(sourcePoints.className(),
                                new DGtal::CustomColors(DGtal::Color::Green,
                                                        DGtal::Color::Green))
          << sourcePoints;
    board.saveSVG( (outputFolder + "/terminal-points.svg").c_str());

    DigitalSet sourceNodes(domain);
    DigitalSet fgSet = shape;
    for (auto pk : fg.vertexSet) {
      Point p = pk.first;
      fgSet.erase(p);
      if (fg.isInSource(p)) sourceNodes.insert(p);
    }

    sourceNodes += fgSet;

    GraphFlow::Utils::Display::saveDigitalSetAsImage(
        sourceNodes, outputFolder + "/result-" +
                         GraphFlow::Utils::String::nDigitsString(nit, 4) +
                         ".png");

    shape = sourceNodes;
    nit++;
  }

  return 0;
}