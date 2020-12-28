#ifndef LINEL_DATA_H
#define LINEL_DATA_H

#include <DGtal/helpers/StdDefs.h>

struct LinelData {
  typedef DGtal::Z2i::Point Point;

  LinelData()
      : costSum(0),
        nContribs(0) {}

  void addCost(double cost) {
    nContribs++;
    this->costSum += cost;
  }

  inline double cost() const { return costSum / nContribs; }

  Point innerPixel;
  Point outerPixel;

  double costSum;
  double nContribs;
};

#endif