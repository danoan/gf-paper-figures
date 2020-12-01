#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/derivates/Misc.h>
#include <graph-flow/utils/digital.h>

#include <unordered_map>
#include <vector>

#include "CostData.h"
#include "EstimationData.h"
#include "InputData.h"
#include "display.h"
#include "estimators.h"
#include "utils.h"

using namespace DGtal::Z2i;
typedef GraphFlow::Utils::Digital::DTL2 DTL2;

void setEstimationDataMap(std::unordered_map<Point, EstimationData>& edMap,
                          const Curve& curve, const Domain& domain,
                          const DigitalSet& shape, double h) {
  std::vector<double> kV;
  Estimators::curvatureMDCA(kV, curve, domain, h);

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

void setCostFunction(std::unordered_map<Point, CostData>& costFunction,
                     const Domain& KDomain, const DigitalSet& shapeK,
                     const DigitalSet& ring, double h, const DTL2& dtInn,
                     const DTL2& dtOut,
                     const std::unordered_map<Point, EstimationData>& edMap) {
  KSpace kspace;
  kspace.init(KDomain.lowerBound(), KDomain.upperBound(), true);

  for (Point p : ring) {
    if (p[0] % 2 == 0 && p[1] % 2 == 0) continue;  // Pointel
    if ( abs(p[0]) % 2 == 1 && abs(p[1]) % 2 == 1) continue;  // Pixel

    Point closestPoint;
    double sDistance;
    if (shapeK(p)) {
      closestPoint = dtInn.getVoronoiVector(p);
      sDistance = -dtInn(p) / (2 * h);
    } else {
      closestPoint = dtOut.getVoronoiVector(p);
      sDistance = dtOut(p) / (2 * h);
    }

    if (edMap.find(closestPoint) == edMap.end()) {
      // throw std::runtime_error("Linel or pointel missing");
      continue;
    }

    EstimationData ed = edMap.at(closestPoint);
    double curvatureCost;
    if (ed.curvature > 0)
      curvatureCost = 1.0 / (1.0 / ed.curvature + sDistance);
    else
      curvatureCost = 1.0 / (1.0 / ed.curvature - sDistance);

    // double cost = ed.localLength + ed.localLength * pow(curvatureCost,
    // 2);
    double cost = pow(curvatureCost, 2);

    KSpace::SCell linel = kspace.sCell(p);
    auto pixels = kspace.sUpperIncident(linel);

    costFunction[p] =
        CostData{kspace.sCoords(pixels[0]), kspace.sCoords(pixels[1]), cost};
  }
}

int main(int argc, char* argv[]) {
  InputData id = readInput(argc, argv);
  Point border(10, 10);

  DigitalSet _shape =
      GraphFlow::Utils::Digital::resolveShape(id.shapeName, id.h);
  Domain domain(_shape.domain().lowerBound() - border,
                _shape.domain().upperBound() + border);

  DigitalSet shape(domain);
  shape.insert(_shape.begin(), _shape.end());

  Curve shapeContour;
  DIPaCUS::Misc::computeBoundaryCurve(shapeContour, shape);

  std::unordered_map<Point, EstimationData> edMap;
  setEstimationDataMap(edMap, shapeContour, domain, shape, id.h);

  // Khalimsky shape and its complement
  DigitalSet innerContourK =
      Utils::buildKhalimskyContour(shapeContour, domain, id.h);
  const Domain& KDomain = innerContourK.domain();

  DigitalSet shapeK(KDomain);
  shapeK.insert(innerContourK.begin(), innerContourK.end());

  Point kpInn = *shapeContour.getInnerPointsRange().begin();
  kpInn *= 2;
  kpInn += Point(1, 1);
  DIPaCUS::Misc::fillInterior(shapeK, kpInn, innerContourK);

  DigitalSet shapeKCompl(shapeK.domain());
  shapeKCompl.assignFromComplement(shapeK);
  shapeKCompl += innerContourK;

  // Distance transformations
  auto dtInn = GraphFlow::Utils::Digital::exteriorDistanceTransform(
      KDomain, shapeKCompl);

  auto dtOut =
      GraphFlow::Utils::Digital::exteriorDistanceTransform(KDomain, shapeK);

  // Set cost function
  typedef DGtal::Z2i::Point LinelKCoords;
  std::unordered_map<LinelKCoords, CostData> costFunction;  
  
  DigitalSet ring =
      Utils::buildRing(KDomain, shapeK, dtInn, dtOut, id.ringWidth);
  setCostFunction(costFunction, KDomain, shapeK, ring,id.h, dtInn, dtOut, edMap);


  Display::displayCost(costFunction, innerContourK,
                       "costFunction.svg", std::cout, id.M);

  return 0;
}