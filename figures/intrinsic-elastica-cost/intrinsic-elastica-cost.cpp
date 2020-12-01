#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/derivates/Misc.h>
#include <graph-flow/utils/digital.h>

#include <unordered_map>
#include <vector>

#include "EstimationData.h"
#include "InputData.h"
#include "display.h"
#include "estimators.h"
#include "utils.h"

using namespace DGtal::Z2i;

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

  Display::displayCost(KDomain,shapeK,innerContourK,id.h,id.ringWidth,edMap,dtInn,dtOut,"costFunction.svg",std::cout,id.M);

  return 0;
}