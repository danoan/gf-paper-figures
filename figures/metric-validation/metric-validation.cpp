#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/io/colormaps/GradientColorMap.h>
#include <DIPaCUS/base/Shapes.h>
#include <DIPaCUS/derivates/Misc.h>
#include <graph-flow/utils/display.h>

#include <boost/filesystem.hpp>
#include <iostream>
#include <unordered_map>

#include "FlowGraph.h"
#include "LinelData.h"
#include "estimators.h"

using namespace DGtal::Z2i;

enum CurvatureEstimator { MDCA, II };
enum PointCollect { SOURCE, TARGET, NONE };

void updateLinelCost(std::unordered_map<Point, LinelData>& linelCost,
                     std::set<Point>& terminalPoints, PointCollect pointCollect,
                     const DigitalSet& shape,
                     CurvatureEstimator curvatureEstimator, double h,
                     double alpha, double beta) {
  const Domain& domain = shape.domain();
  KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);

  Curve contour;
  DIPaCUS::Misc::computeBoundaryCurve(contour, shape);

  std::vector<double> kV;
  if (curvatureEstimator == CurvatureEstimator::MDCA) {
    Estimators::curvatureMDCA(kV, contour, domain, h);
  } else if (curvatureEstimator == CurvatureEstimator::II) {
    Estimators::curvatureII(kV, contour, domain, shape, 5, h);
  }

  std::vector<double> lV;
  Estimators::localLength(lV, contour, domain, h);

  auto scellRange = contour.getSCellsRange();
  auto it = scellRange.begin();
  int i = 0;
  do {
    Point linelCoords = it->preCell().coordinates;

    double cost = alpha * lV[i] + beta * lV[i] * pow(kV[i], 2);

    auto innerPixel = kspace.sDirectIncident(*it, *kspace.sOrthDirs(*it));
    auto outerPixel = kspace.sIndirectIncident(*it, *kspace.sOrthDirs(*it));

    Point innerPixelC = kspace.sCoords(innerPixel);
    Point outerPixelC = kspace.sCoords(outerPixel);

    if (pointCollect == SOURCE)
      terminalPoints.insert(innerPixelC);
    else if (pointCollect == TARGET)
      terminalPoints.insert(outerPixelC);

    if (linelCost.find(linelCoords) == linelCost.end()) {
      linelCost[linelCoords] = LinelData(innerPixelC, outerPixelC);
    }

    linelCost[linelCoords].addCost(cost);

    it++;
    i++;
  } while (it != scellRange.end());
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

int main(int argc, char* argv[]) {
  double h = 1;
  double curvatureEstimator = CurvatureEstimator::MDCA;

  double radius = std::atof(argv[1]);
  double alpha = std::atof(argv[2]);
  double beta = std::atof(argv[3]);
  double w = std::atof(argv[4]);

  std::string outputFolder = argv[5];

  boost::filesystem::create_directories(outputFolder);

  std::unordered_map<Point, LinelData> linelCost;
  DigitalSet _disk = DIPaCUS::Shapes::ball(h, 0, 0, radius);

  int b = (int)(20.0 / h);
  Point border(b, b);

  Domain domain(_disk.domain().lowerBound() - border,
                _disk.domain().upperBound() + border);
  DigitalSet disk(domain);
  disk.insert(_disk.begin(), _disk.end());

  GraphFlow::Utils::Display::saveDigitalSetAsImage(disk,
                                                   outputFolder + "/begin.png");

  double factor = 0.05;
  std::set<Point> sourcePoints;
  std::set<Point> targetPoints;

  updateLinelCost(linelCost, targetPoints, PointCollect::NONE, disk,
                  CurvatureEstimator::MDCA, h, alpha, beta);

  for (int d = 1; d <= w; ++d) {
    PointCollect pcOuter = PointCollect::NONE;
    PointCollect pcInner = PointCollect::NONE;

    if (d == w) {
      pcOuter = PointCollect::TARGET;
      pcInner = PointCollect::SOURCE;
    }

    DigitalSet shapeOuter = DIPaCUS::Shapes::ball(h, 0, 0, radius + d * factor);
    DigitalSet shapeInner = DIPaCUS::Shapes::ball(h, 0, 0, radius - d * factor);

    updateLinelCost(linelCost, targetPoints, pcOuter, shapeOuter,
                    CurvatureEstimator::MDCA, h, alpha, beta);
    updateLinelCost(linelCost, sourcePoints, pcInner, shapeInner,
                    CurvatureEstimator::MDCA, h, alpha, beta);
  }

  std::string outputFilepath = outputFolder + "/metric-validation.svg";
  displayCost(linelCost, outputFilepath, std::cout);

  DigitalSet lastDisk = DIPaCUS::Shapes::ball(h, 0, 0, radius + w * factor);
  FlowGraph fg(lastDisk, sourcePoints, targetPoints, linelCost);

  DigitalSet sourceNodes(domain);
  for (Point p : lastDisk) {
    if (fg.isInSource(p)) sourceNodes.insert(p);
  }

  GraphFlow::Utils::Display::saveDigitalSetAsImage(
      sourceNodes, outputFolder + "/result.png");

  return 0;
}