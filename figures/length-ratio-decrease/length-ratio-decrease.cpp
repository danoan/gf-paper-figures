#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/base/Shapes.h>
#include <geoc/api/gridCurve/Length.hpp>
#include <geoc/api/gridCurve/Curvature.hpp>
#include <geoc/estimator/adaptable/Curvature.h>

using namespace DGtal::Z2i;

double totalSquaredCurvature(const DigitalSet& ds,double estimationRadius){
  using namespace GEOC::API::GridCurve;

  Curve curve;
  DIPaCUS::Misc::computeBoundaryCurve(curve,ds);


  KSpace kspace;
  kspace.init(ds.domain().lowerBound(),ds.domain().upperBound(),true);

  std::vector<double> evK;
  GEOC::Estimator::Standard::IICurvatureExtraData data(true,estimationRadius);
  GEOC::Estimator::Standard::IICurvature<Curve::ConstIterator>(ds,curve.begin(),curve.end(),evK,1.0,&data);

  std::vector<double> evL;
  Length::mdssClosed<Length::EstimationAlgorithms::ALG_PROJECTED >(kspace,curve.begin(),curve.end(),evL,1.0, nullptr);

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

double perimeter(const DigitalSet& ds){
  using namespace GEOC::API::GridCurve::Length;

  Curve curve;
  DIPaCUS::Misc::computeBoundaryCurve(curve,ds);

  std::vector<double> ev;
  KSpace kspace;
  kspace.init(ds.domain().lowerBound(),ds.domain().upperBound(),true);
  mdssClosed<EstimationAlgorithms::ALG_PROJECTED >(kspace,curve.begin(),curve.end(),ev,1.0, nullptr);

  double p=0;
  for(double d:ev) p+=d;

  return p;
}

int main(int argc, char* argv[]){
  std::string imageFilepath=argv[1];
  double estimationRadius = std::atof(argv[2]);

  cv::Mat cvImg = cv::imread(imageFilepath,cv::IMREAD_GRAYSCALE);

  Point lb(0,0);
  Point ub(cvImg.cols-1,cvImg.rows-1);
  Domain domain(lb,ub);

  DigitalSet ds(domain);
  DIPaCUS::Representation::CVMatToDigitalSet(ds,cvImg);

  double p = perimeter(ds);
  double tk2 = totalSquaredCurvature(ds,estimationRadius);

  std::cout << p << "\t" << tk2 << std::endl;

  return 0;
}