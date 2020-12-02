#ifndef KHALIMSKY_EQUIVALENT_H
#define KHALIMSKY_EQUIVALENT_H

#include <DGtal/helpers/StdDefs.h>

struct KhalimskyEquivalent {
  typedef DGtal::Z2i::DigitalSet DigitalSet;
  typedef DGtal::Z2i::Domain Domain;

  KhalimskyEquivalent(const Domain& domain)
      : shapeK(domain), innerContourK(domain) {}

  DigitalSet shapeK;
  DigitalSet innerContourK;
};

#endif