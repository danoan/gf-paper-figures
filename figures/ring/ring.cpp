#include <functional>
#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/base/Shapes.h>
#include <DIPaCUS/base/Representation.h>

class RingShape
{
 public:
  typedef DGtal::Z2i::RealPoint RealPoint;

 private:
  typedef DGtal::Z2i::Point Point;

  typedef std::function<bool(const RealPoint& p)> CirclePredicate;

 public:
  RingShape(const double x, const double y, const double smallR, const double bigR);

  DGtal::Orientation orientation(const RealPoint& aPoint) const;

  RealPoint getLowerBound() const { return lb; }
  RealPoint getUpperBound() const { return ub; }

 private:
  CirclePredicate circle(const RealPoint& center, const double radius);

 public:
  RealPoint lb,ub;

  CirclePredicate smallCircle;
  CirclePredicate bigCircle;
};

RingShape::RingShape(const double x, const double y, const double smallR, const double bigR){
  smallCircle = circle( Point(x,y),smallR);
  bigCircle = circle( Point(x,y),bigR);

  lb = Point(x-bigR,y-bigR);
  ub = Point(x+bigR,y+bigR);
}

RingShape::CirclePredicate RingShape::circle(const RealPoint& center, const double radius)
{
  return [center,radius](const RealPoint& p){ return ( pow(center[0] - p[0],2) + pow(center[1]-p[1],2) - pow(radius,2) ) <= 0; };
}

DGtal::Orientation RingShape::orientation(const RealPoint& aPoint) const
{
  if( !smallCircle(aPoint) && bigCircle(aPoint) ) return DGtal::Orientation::INSIDE;
  else return DGtal::Orientation::OUTSIDE;
}


int main(int argc,char* argv[]){
  using namespace DGtal::Z2i;

  double r = std::atof(argv[1]);
  double R = std::atof(argv[2]);
  double h = std::atof(argv[3]);
  std::string outputFilepath = argv[4];

  RingShape ringShape(0,0,r,R);
  DigitalSet ring = DIPaCUS::Shapes::digitizeShape(ringShape,h);

  Point dims = ring.domain().upperBound() - ring.domain().lowerBound() + Point(1,1);
  cv::Mat cvImg = cv::Mat::zeros(dims[1],dims[0],CV_8UC1);
  DIPaCUS::Representation::digitalSetToCVMat(cvImg,ring);
  std::cout << ring.size() << std::endl;

  cv::imwrite(outputFilepath,cvImg);

  return 0;
}