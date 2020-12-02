#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/components/SetOperations.h>
#include <DIPaCUS/derivates/Misc.h>
#include <graph-flow/utils/digital.h>
#include <graph-flow/utils/display.h>
#include <graph-flow/utils/energy.h>
#include <graph-flow/utils/string.h>

#include <unordered_map>
#include <vector>

#include "CostData.h"
#include "EstimationData.h"
#include "FlowGraph.h"
#include "InputData.h"
#include "KhalimskyEquivalent.h"
#include "display.h"
#include "estimators.h"
#include "utils.h"

using namespace DGtal::Z2i;
typedef GraphFlow::Utils::Digital::DTL2 DTL2;
typedef DGtal::Z2i::Point LinelKCoords;

void setEstimationDataMap(std::unordered_map<Point, EstimationData>& edMap,
                          const Curve& curve, const Domain& domain,
                          const DigitalSet& shape, double h) {
  std::vector<double> kV;
  Estimators::curvatureMDCA(kV, curve, domain, h);
  // Estimators::curvatureII(kV, curve, domain, shape, 5,h);

  std::vector<double> lV;
  Estimators::localLength(lV, curve, domain, h);

  std::vector<RealPoint> tV;
  Estimators::tangent(tV, curve, domain, h);

  KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);

  auto scellRange = curve.getSCellsRange();
  auto it = scellRange.begin();
  int i = 0;
  do {
    Point p = it->preCell().coordinates;
    edMap[p] = EstimationData{tV[i], RealPoint(tV[1], -tV[0]), lV[i], kV[i]};

    SCell pointel = kspace.sDirectIncident(*it, *kspace.sDirs(*it));
    edMap[kspace.sKCoords(pointel)] = edMap[p];
    it++;
    i++;
  } while (it != scellRange.end());
}

Point inline getClosestPoint(const Point& p, const DigitalSet& shapeK,
                             const DTL2& dtInn, const DTL2& dtOut) {
  if (shapeK(p))
    return dtInn.getVoronoiVector(p);
  else
    return dtOut.getVoronoiVector(p);
}

inline double getSDistance(const Point& p, double h,const DigitalSet& shapeK,
                           const DTL2& dtInn, const DTL2& dtOut) {
  if (shapeK(p))
    return -dtInn(p) / (2 * h);
  else
    return dtOut(p) / (2 * h);
}

inline double getCurvatureCost(double curvatureEstimation, double sDistance) {
  if (curvatureEstimation > 0)
    return 1.0 / (1.0 / curvatureEstimation + sDistance);
  else
    return 1.0 / (1.0 / curvatureEstimation - sDistance);
}

void setKhalimskyEquivalent(KhalimskyEquivalent& ke, const Domain& domain,
                            const Curve& shapeContour, double h) {
  // Khalimsky shape and its complement
  ke.innerContourK = Utils::buildKhalimskyContour(shapeContour, domain, h);
  ke.shapeK.insert(ke.innerContourK.begin(), ke.innerContourK.end());

  Point kpInn = *shapeContour.getInnerPointsRange().begin();
  kpInn *= 2;
  kpInn += Point(1, 1);
  DIPaCUS::Misc::fillInterior(ke.shapeK, kpInn, ke.innerContourK);
}

void setCostFunction(std::unordered_map<LinelKCoords, CostData>& costFunction,
                     const Domain& KDomain, const DigitalSet& shapeK,
                     double ringWidth, double h, double alpha, double beta,
                     const DTL2& dtInn, const DTL2& dtOut,
                     const std::unordered_map<Point, EstimationData>& edMap) {
  KSpace kspace;
  kspace.init(KDomain.lowerBound(), KDomain.upperBound(), true);

  DigitalSet ring = Utils::buildRing(KDomain, shapeK, dtInn, dtOut, ringWidth);

  for (Point p : ring) {
    if (p[0] % 2 == 0 && p[1] % 2 == 0) continue;            // Pointel
    if (abs(p[0]) % 2 == 1 && abs(p[1]) % 2 == 1) continue;  // Pixel

    Point closestPoint = getClosestPoint(p, shapeK, dtInn, dtOut);
    double sDistance = getSDistance(p, h,shapeK, dtInn, dtOut);

    // if (edMap.find(closestPoint) == edMap.end()) {
    //   continue;
    // }

    EstimationData ed = edMap.at(closestPoint);
    double curvatureCost = getCurvatureCost(ed.curvature, sDistance);

    double cost = alpha * ed.localLength + beta * pow(curvatureCost, 2);

    KSpace::SCell linel = kspace.sCell(p);
    auto pixels = kspace.sUpperIncident(linel);

    costFunction[p] =
        CostData{kspace.sCoords(pixels[0]), kspace.sCoords(pixels[1]), cost};
  }
}

std::unordered_map<LinelKCoords, CostData> getCostFunction(
    const KhalimskyEquivalent& ke,
    const std::unordered_map<Point, EstimationData>& edMap, double h,
    double alpha, double beta, double ringWidth) {
  const Domain& KDomain = ke.innerContourK.domain();

  DigitalSet shapeKCompl(ke.shapeK.domain());
  shapeKCompl.assignFromComplement(ke.shapeK);
  shapeKCompl += ke.innerContourK;

  // Distance transformations
  auto dtInn = GraphFlow::Utils::Digital::exteriorDistanceTransform(
      KDomain, shapeKCompl);

  auto dtOut =
      GraphFlow::Utils::Digital::exteriorDistanceTransform(KDomain, ke.shapeK);

  // Set cost function
  std::unordered_map<LinelKCoords, CostData> costFunction;
  setCostFunction(costFunction, KDomain, ke.shapeK, ringWidth, h, alpha, beta,
                  dtInn, dtOut, edMap);

  return costFunction;
}

void setOptBand(DigitalSet& optBand,
                const std::unordered_map<Point, CostData>& costFunction) {
  for (auto pk : costFunction) {
    Point linelKCoords = pk.first;
    CostData cd = pk.second;

    optBand.insert(cd.pixel1);
    optBand.insert(cd.pixel2);
  }
}

void setTerminalPoints(std::set<Point>& sourcePoints,
                       std::set<Point>& targetPoints, const Domain& domain,
                       const DigitalSet& shape, const DigitalSet& optBand) {
  DGtal::SurfelAdjacency<2> sadj(true);
  std::vector<std::vector<DGtal::Z2i::SCell> > vscells;

  KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);

  DGtal::Surfaces<KSpace>::extractAll2DSCellContours(vscells, kspace, sadj,
                                                     optBand);

  for (auto seqSCells : vscells) {
    Curve c;
    c.initFromSCellsVector(seqSCells);
    auto range = c.getInnerPointsRange();
    auto it = range.begin();
    bool insideFlag = shape(*it);
    do {
      if (insideFlag)
        sourcePoints.insert(*it);
      else
        targetPoints.insert(*it);
      ++it;
    } while (it != range.end());
  }
}

DigitalSet computeCut(const Domain& domain, const DigitalSet& shape,
                      const std::unordered_map<Point, CostData>& costFunction) {
  DigitalSet optBand(domain);
  setOptBand(optBand, costFunction);

  DigitalSet sureFg(domain);
  DIPaCUS::SetOperations::setDifference(sureFg, shape, optBand);

  std::set<Point> sourcePoints;
  std::set<Point> targetPoints;
  setTerminalPoints(sourcePoints, targetPoints, domain, shape, optBand);

  Display::displayOptRegions(sureFg, optBand, sourcePoints, targetPoints,
                             "optRegions.svg");

  FlowGraph fg(optBand, sourcePoints, targetPoints, costFunction);
  std::cout << "Cut Value:" << fg.cutValue() << std::endl;

  DigitalSet sourceNodes(domain);
  for (Point p : optBand) {
    if (fg.isInSource(p)) sourceNodes.insert(p);
  }
  sourceNodes.insert(sureFg.begin(), sureFg.end());

  return sourceNodes;
}

int main(int argc, char* argv[]) {
  InputData id = readInput(argc, argv);
  Point border((int)ceil(40 / id.h), (int)ceil(40 / id.h));

  DigitalSet _shape =
      GraphFlow::Utils::Digital::resolveShape(id.shapeName, id.h);
  Domain domain(_shape.domain().lowerBound() - border,
                _shape.domain().upperBound() + border);

  DigitalSet shape(domain);
  shape.insert(_shape.begin(), _shape.end());

  GraphFlow::Utils::Display::saveDigitalSetAsImage(shape, "it-0.png");

  double currElastica =
      GraphFlow::Utils::Energy::elastica(shape, 5, id.h, id.alpha, id.beta);
  std::cout << "Elastica: " << currElastica << std::endl;

  int it = 0;
  while (it < id.maxIt) {
    Curve shapeContour;
    DIPaCUS::Misc::computeBoundaryCurve(shapeContour, shape);

    std::unordered_map<Point, EstimationData> edMap;
    setEstimationDataMap(edMap, shapeContour, domain, shape, id.h);

    KhalimskyEquivalent ke(domain);
    setKhalimskyEquivalent(ke, domain, shapeContour, id.h);

    auto costFunction =
        getCostFunction(ke, edMap, id.h, id.alpha, id.beta, id.ringWidth);

    DigitalSet sourceNodes = computeCut(domain, shape, costFunction);

    shape.clear();
    shape.insert(sourceNodes.begin(), sourceNodes.end());
    ++it;

    std::string outputFilepath =
        "it-" + GraphFlow::Utils::String::nDigitsString(it, 4) + ".png";
    GraphFlow::Utils::Display::saveDigitalSetAsImage(shape, outputFilepath);

    currElastica =
        GraphFlow::Utils::Energy::elastica(shape, 5, id.h, id.alpha, id.beta);
    std::cout << "Elastica: " << currElastica << std::endl;
  }

  return 0;
}