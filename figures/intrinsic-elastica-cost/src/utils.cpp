#include "utils.h"

namespace Utils {
DigitalSet buildKhalimskyContour(const Curve& shapeContour,
                                 const Domain& domain, double h) {
  using namespace DGtal::Z2i;

  int factor = (int)ceil(2.0 / h);
  Domain KDomain(factor * domain.lowerBound(), factor * domain.upperBound());

  KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);

  DigitalSet innerContourK(KDomain);
  {
    auto linelRange = shapeContour.getSCellsRange();
    auto it = linelRange.begin();
    do {
      SCell pointel1 = kspace.sIndirectIncident(*it, *kspace.sDirs(*it));
      SCell pointel2 = kspace.sDirectIncident(*it, *kspace.sDirs(*it));

      Point linelKCoord = it->preCell().coordinates;
      innerContourK.insert(linelKCoord);
      innerContourK.insert(kspace.sKCoords(pointel1));
      innerContourK.insert(kspace.sKCoords(pointel2));

      it++;
    } while (it != linelRange.end());
  }

  return innerContourK;
}

DigitalSet buildRing(const Domain& KDomain, const DigitalSet& shapeK,
                     const DTL2& dtInn, const DTL2& dtOut, double ringWidth) {
  using namespace DGtal::Z2i;

  DigitalSet ring(KDomain);
  for (Point p : KDomain) {
    double distance;
    if (shapeK(p))
      distance = dtInn(p);
    else
      distance = dtOut(p);

    if (distance <= ringWidth) ring.insert(p);
  }

  return ring;
}
}  // namespace Utils