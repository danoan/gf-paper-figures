#include "utils.h"

namespace Utils {

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