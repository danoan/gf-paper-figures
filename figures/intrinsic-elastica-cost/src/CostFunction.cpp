#include "CostFunction.h"

CostFunction::CostFunction(const KhalimskyEquivalent& ke,
                           const EstimationDataMap& edMap, double h,
                           double ringWidth, double alpha, double beta) {
  using namespace DGtal::Z2i;

  const Domain& KDomain = ke.innerContourK.domain();

  KSpace kspace;
  kspace.init(KDomain.lowerBound(), KDomain.upperBound(), true);

  // Distance transformations
  DTL2 dtInn(KDomain, ke.shapeK, DGtal::Z2i::l2Metric);
  DTL2 dtOut(KDomain, ke.shapeKCompl, DGtal::Z2i::l2Metric);

  DigitalSet ring =
      Utils::buildRing(KDomain, ke.shapeK, dtInn, dtOut, ringWidth);

  Display::displayRing(ke, ring, "ring.svg");

  for (Point p : ring) {
    if (p[0] % 2 == 0 && p[1] % 2 == 0) continue;            // Pointel
    if (abs(p[0]) % 2 == 1 && abs(p[1]) % 2 == 1) continue;  // Pixel

    Point closestPoint = getClosestPoint(p, ke.shapeK, dtInn, dtOut);
    double sDistance = getSDistance(p, h, ke.shapeK, dtInn, dtOut);

    if (edMap.find(closestPoint) == edMap.end()) {
      continue;
    }

    EstimationData ed = edMap.at(closestPoint);
    double curvatureCost = getCurvatureCost(ed.curvature, sDistance);

    double cost = alpha * ed.localLength + beta * pow(curvatureCost, 2);

    KSpace::SCell linel = kspace.sCell(p);
    auto pixels = kspace.sUpperIncident(linel);

    m_linelCostMap[p] =
        CostData{kspace.sCoords(pixels[0]), kspace.sCoords(pixels[1]), cost};
  }
}

CostFunction::LinelKCoords inline CostFunction::getClosestPoint(
    const LinelKCoords& p, const DigitalSet& shapeK, const DTL2& dtInn,
    const DTL2& dtOut) {
  if (shapeK(p))
    return dtInn.getVoronoiVector(p);
  else
    return dtOut.getVoronoiVector(p);
}

inline double CostFunction::getSDistance(const LinelKCoords& p, double h,
                                         const DigitalSet& shapeK,
                                         const DTL2& dtInn, const DTL2& dtOut) {
  if (shapeK(p))
    return -dtInn(p) / (2 * h);
  else
    return dtOut(p) / (2 * h);
}

inline double CostFunction::getCurvatureCost(double curvatureEstimation,
                                             double sDistance) {
  if (curvatureEstimation > 0)
    return 1.0 / (1.0 / curvatureEstimation + sDistance);
  else
    return 1.0 / (1.0 / curvatureEstimation - sDistance);
}