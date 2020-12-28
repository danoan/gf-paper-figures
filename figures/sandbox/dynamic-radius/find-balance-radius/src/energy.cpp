#include "energy.h"

double totalSquaredCurvature(const DGtal::Z2i::DigitalSet& ds,double estimationRadius,double h){
  using namespace DGtal::Z2i;
  using namespace GEOC::API::GridCurve;

  Curve curve;
  DIPaCUS::Misc::computeBoundaryCurve(curve,ds);


  KSpace kspace;
  kspace.init(ds.domain().lowerBound(),ds.domain().upperBound(),true);

  std::vector<double> evK;
  GEOC::Estimator::Standard::IICurvatureExtraData data(true,estimationRadius);
  GEOC::Estimator::Standard::IICurvature<Curve::ConstIterator>(ds,curve.begin(),curve.end(),evK,h,&data);

  std::vector<double> evL;
  Length::mdssClosed<Length::EstimationAlgorithms::ALG_PROJECTED >(kspace,curve.begin(),curve.end(),evL,h, nullptr);

  double totalK2=0;
  auto itK=evK.begin();
  auto itL=evL.begin();
  do{
    totalK2 += pow((*itK),2)*(*itL);
    itK++;
    itL++;
  }while(itK!=evK.end());

  return totalK2;
}

double perimeter(const DGtal::Z2i::DigitalSet& ds,double h){
  using namespace DGtal::Z2i;
  using namespace GEOC::API::GridCurve::Length;

  Curve curve;
  DIPaCUS::Misc::computeBoundaryCurve(curve,ds);

  std::vector<double> ev;
  KSpace kspace;
  kspace.init(ds.domain().lowerBound(),ds.domain().upperBound(),true);
  mdssClosed<EstimationAlgorithms::ALG_PROJECTED >(kspace,curve.begin(),curve.end(),ev,h, nullptr);

  double p=0;
  for(double d:ev) p+=d;

  return p;
}

double elastica(const DGtal::Z2i::DigitalSet& ds,double h,double estimationRadius,double alpha, double beta){
  return alpha*perimeter(ds,h) + beta*totalSquaredCurvature(ds,estimationRadius,h);

}