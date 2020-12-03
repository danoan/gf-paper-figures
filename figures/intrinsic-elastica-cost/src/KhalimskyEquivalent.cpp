#include "KhalimskyEquivalent.h"

KhalimskyEquivalent::KhalimskyEquivalent(const Domain& domain,
                                         const Curve& shapeContour, double h)
    : KDomain(initKDomain(domain, h)),
      shapeK(KDomain),
      shapeKCompl(KDomain),
      innerContourK(KDomain) {
  using namespace DGtal::Z2i;

  buildKhalimskyContour(shapeContour, domain, h);
  shapeK.insert(innerContourK.begin(), innerContourK.end());

  Point kpInn = *shapeContour.getInnerPointsRange().begin();
  kpInn *= 2;
  kpInn += Point(1, 1);
  DIPaCUS::Misc::fillInterior(shapeK, kpInn, innerContourK);

  shapeKCompl.assignFromComplement(shapeK);

  for (auto p : innerContourK) shapeK.erase(p);
}

void KhalimskyEquivalent::buildKhalimskyContour(const Curve& shapeContour,
                                                const Domain& domain,
                                                double h) {
  using namespace DGtal::Z2i;

  KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);

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
}