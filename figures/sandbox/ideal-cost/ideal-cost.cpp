#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/io/colormaps/GradientColorMap.h>
#include <DIPaCUS/base/Shapes.h>
#include <graph-flow/utils/display.h>
#include <graph-flow/utils/string.h>

#include <boost/filesystem.hpp>
#include <iostream>
#include <unordered_map>

#include "FlowGraph.h"
#include "estimators.h"

using namespace DGtal::Z2i;

typedef std::unordered_map<Point, double>& CostFunction;

void displayCost(const CostFunction& costFunction,
                 const std::string& outputFilepath, std::ostream& os) {
  typedef DGtal::GradientColorMap<double, DGtal::ColorGradientPreset::CMAP_JET>
      MyColorMap;

  double M = 0;
  double m = 100;

  for (auto pk : costFunction) {
    double cost = pk.second;
    if (cost > M) M = cost;
    if (cost < m) m = cost;
  }

  os << std::fixed << std::setprecision(3) << "Min unit cost: " << m << "\n"
     << "Max unit cost: " << M << "\n";

  MyColorMap hmap(m, M);

  DGtal::Board2D board;
  for (auto pk : costFunction) {
    Point p = pk.first;
    double cost = pk.second;

    if (cost > M) M = cost;
    if (cost < m) m = cost;

    board << DGtal::CustomStyle(p.className(),
                                new DGtal::CustomColors(hmap(cost), hmap(cost)))
          << p;
  }

  board.saveSVG(outputFilepath.c_str());
}

int main(int argc, char* argv[]) {
  double alpha = std::atof(argv[1]);
  double beta = std::atof(argv[2]);
  double h = 0.25;
  double estimationRadius = 5;

  int nit = 1;
  std::string outputFolder = argv[3];
  boost::filesystem::create_directories(outputFolder);

  DigitalSet _shape = DIPaCUS::Shapes::ball(h);
  DigitalSet shape = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_shape);
  const Domain& domain = shape.domain();

  GraphFlow::Utils::Display::saveDigitalSetAsImage(
      shape, outputFolder + "/result-" +
                       GraphFlow::Utils::String::nDigitsString(0, 4) +
                       ".png");  

  DigitalSet temp = shape;
  KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);
  std::unordered_map<Point, double> costMap;
  for (int k = 0; k < 20; ++k) {
    Curve curve;
    DIPaCUS::Misc::computeBoundaryCurve(curve, temp);

    std::vector<double> evK;
    Estimators::curvatureII(evK, curve, domain, temp, estimationRadius, h);

    std::vector<double> evL;
    Estimators::localLength(evL, curve, domain, h);

    auto it = curve.begin();
    int i = 0;
    do {
      double c = alpha * evL[i] + beta * evL[i] * pow(evK[i], 2);
      Point linelCoords = kspace.sKCoords(*it);
      costMap[linelCoords] = c;

      ++it;
      ++i;
    } while (it != curve.end());

    DigitalSet eightContour(domain);
    DIPaCUS::Misc::digitalBoundary<
        DIPaCUS::Neighborhood::EightNeighborhoodPredicate>(eightContour, temp);
    for (auto p : eightContour) temp.erase(p);
  }

  FlowGraph fg(kspace, shape, costMap);

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

  displayCost(costMap, outputFolder + "/cost-function.svg", std::cout);
  return 0;
}