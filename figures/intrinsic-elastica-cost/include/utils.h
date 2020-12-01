#ifndef UTILS_H
#define UTILS_H

#include <DGtal/helpers/StdDefs.h>

namespace Utils {
typedef DGtal::Z2i::DigitalSet DigitalSet;
typedef DGtal::Z2i::Domain Domain;
typedef DGtal::Z2i::Curve Curve;
DigitalSet buildKhalimskyContour(const Curve& shapeContour, const Domain& domain,
                               double h);
}  // namespace Utils

#endif