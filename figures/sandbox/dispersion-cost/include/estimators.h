#ifndef ESTIMATORS_H
#define ESTIMATORS_H
#include <DGtal/helpers/StdDefs.h>
#include <geoc/estimator/adaptable/Curvature.h>

#include <geoc/api/gridCurve/Curvature.hpp>
#include <geoc/api/gridCurve/Length.hpp>
#include <geoc/api/gridCurve/Tangent.hpp>
#include <vector>

namespace Estimators {
typedef DGtal::Z2i::DigitalSet DigitalSet;
typedef DGtal::Z2i::Domain Domain;
typedef DGtal::Z2i::Curve curve;
typedef DGtal::Z2i::RealPoint RealPoint;

void curvatureII(std::vector<double>& ev, const Curve& curve,
                 const Domain& domain, const DigitalSet& ds,
                 double estimationRadius, double h);

void curvatureMDCA(std::vector<double>& ev, const Curve& curve,
                   const Domain& domain, double h);

void tangent(std::vector<RealPoint>& ev, const Curve& curve,
             const Domain& domain, double h);

void localLength(std::vector<double>& ev, const Curve& curve,
                 const Domain& domain, double h);
}  // namespace Estimators

#endif