#ifndef UTILS_H
#define UTILS_H

#include <DGtal/helpers/StdDefs.h>
#include <graph-flow/utils/digital/misc.h>

namespace Utils {
typedef DGtal::Z2i::DigitalSet DigitalSet;
typedef DGtal::Z2i::Domain Domain;
typedef DGtal::Z2i::Curve Curve;
typedef GraphFlow::Utils::Digital::Misc::DTL2 DTL2;


DigitalSet buildRing(const Domain& KDomain, const DigitalSet& shapeK,
                     const DTL2& dtInn, const DTL2& dtOut, double ringWidth);
}  // namespace Utils

#endif