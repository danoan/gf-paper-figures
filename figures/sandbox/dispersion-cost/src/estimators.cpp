#include "estimators.h"

namespace Estimators {
void curvatureII(std::vector<double>& ev, const Curve& curve,
                 const Domain& domain, const DigitalSet& ds,
                 double estimationRadius, double h) {
  using namespace GEOC::API::GridCurve;

  DGtal::Z2i::KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);

  GEOC::Estimator::Standard::IICurvatureExtraData data(true, estimationRadius);
  GEOC::Estimator::Standard::IICurvature<Curve::ConstIterator>(
      ds, curve.begin(), curve.end(), ev, h, &data);
}

void curvatureMDCA(std::vector<double>& ev, const Curve& curve,
                   const Domain& domain, double h) {
  using namespace GEOC::API::GridCurve;

  DGtal::Z2i::KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);

  Curvature::symmetricClosed<Curvature::EstimationAlgorithms::ALG_MDCA>(
      kspace, curve.begin(), curve.end(), ev, h, NULL);
}

void tangent(std::vector<RealPoint>& ev, const Curve& curve,
             const Domain& domain, double h) {
  using namespace GEOC::API::GridCurve::Tangent;

  DGtal::Z2i::KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);
  symmetricClosed<EstimationAlgorithms::ALG_MDSS>(kspace, curve.begin(),
                                                  curve.end(), ev, h, nullptr);
}

void localLength(std::vector<double>& ev, const Curve& curve,
                 const Domain& domain, double h) {
  using namespace GEOC::API::GridCurve::Length;

  DGtal::Z2i::KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);
  mdssClosed<EstimationAlgorithms::ALG_SINCOS>(kspace, curve.begin(),
                                                  curve.end(), ev, h, nullptr);
}
}  // namespace Estimators