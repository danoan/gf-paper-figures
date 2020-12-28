#ifndef ESTIMATION_DATA_H
#define ESTIMATION_DATA_H

#include <DGtal/helpers/StdDefs.h>

struct EstimationData {
  typedef DGtal::Z2i::RealPoint RealPoint;

  RealPoint tangent;
  RealPoint normal;
  double localLength;
  double curvature;
};

#endif