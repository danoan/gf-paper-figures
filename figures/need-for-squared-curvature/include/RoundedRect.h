#ifndef GRAPH_FLOW_PAPER_FIGURES_FIGURES_NEED_FOR_SQUARED_CURVATURE_INCLUDE_ROUNDEDRECT_H_
#define GRAPH_FLOW_PAPER_FIGURES_FIGURES_NEED_FOR_SQUARED_CURVATURE_INCLUDE_ROUNDEDRECT_H_

#include <DGtal/helpers/StdDefs.h>

class RoundedRect
{
 public:
  typedef DGtal::Z2i::RealPoint RealPoint;

 private:
  typedef std::function<bool(const RealPoint& p)> CirclePredicate;
  typedef std::function<bool(const RealPoint& p)> RectPredicate;

 public:
  RoundedRect(double x, double y, double width, double height, double cornerRadius);

  DGtal::Orientation orientation(const RealPoint& aPoint) const;

  RealPoint getLowerBound() const { return lb; }
  RealPoint getUpperBound() const { return ub; }

 private:
  CirclePredicate circle(const RealPoint& center, double radius);
  RectPredicate rect(const RealPoint& bottomLeft, double width, double height);

 public:
  RealPoint lb,ub;

  CirclePredicate cornerCircle;
  RectPredicate innerRect;
};

#endif //GRAPH_FLOW_PAPER_FIGURES_FIGURES_NEED_FOR_SQUARED_CURVATURE_INCLUDE_ROUNDEDRECT_H_
