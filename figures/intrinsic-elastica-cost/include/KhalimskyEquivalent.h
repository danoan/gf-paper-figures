#ifndef KHALIMSKY_EQUIVALENT_H
#define KHALIMSKY_EQUIVALENT_H

#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/derivates/Misc.h>

class KhalimskyEquivalent {
 public:
  typedef DGtal::Z2i::DigitalSet DigitalSet;
  typedef DGtal::Z2i::Domain Domain;
  typedef DGtal::Z2i::Curve Curve;

 private:
  static Domain initKDomain(const Domain& domain, double h) {
    int factor = (int)ceil(2.0 / h);
    return Domain(factor * domain.lowerBound(), factor * domain.upperBound());
  }

 public:
  KhalimskyEquivalent(const Domain& domain, const Curve& shapeContour,
                      double h);

 private:
  void buildKhalimskyContour(const Curve& shapeContour, const Domain& domain,
                             double h);

 public:
  Domain KDomain;
  DigitalSet shapeK;
  DigitalSet shapeKCompl;
  DigitalSet innerContourK;
};

#endif