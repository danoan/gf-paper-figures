#ifndef COST_DATA_H
#define COST_DATA_H

#include <DGtal/helpers/StdDefs.h>

struct CostData {
  typedef DGtal::Z2i::Point Point;
  
  Point pixel1;
  Point pixel2;
  double cost;
};

#endif