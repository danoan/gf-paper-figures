#ifndef OPT_REGIONS_H
#define OPT_REGIONS_H

#include <DGtal/helpers/StdDefs.h>

#include <set>
#include <vector>

#include "CostFunction.h"

class OptRegions {
 public:
  typedef DGtal::Z2i::DigitalSet DigitalSet;
  typedef DGtal::Z2i::Domain Domain;
  typedef DGtal::Z2i::Point Point;

 public:
  OptRegions(const Domain& domain, const DigitalSet& shape,
             const CostFunction& costFunction);

 private:
  void setOptBand(const CostFunction& costFunction);
  void setTerminalPoints(const DigitalSet& shape);

 public:
  Domain domain;
  DigitalSet optBand;
  DigitalSet sureFg;

  std::set<Point> sourcePoints;
  std::set<Point> targetPoints;
};

#endif