#include "RoundedRect.h"

RoundedRect::RoundedRect(double x, double y,double width, double height, double cornerRadius){
  CirclePredicate circleBottomLeft = circle( RealPoint(x,y),cornerRadius);
  CirclePredicate circleTopLeft = circle( RealPoint(x,y+height),cornerRadius);
  CirclePredicate circleTopRight = circle( RealPoint(x+width,y+height),cornerRadius);
  CirclePredicate circleBottomRight = circle( RealPoint(x+width,y),cornerRadius);

  cornerCircle = [circleBottomLeft,circleTopLeft,circleTopRight,circleBottomRight](const RealPoint& p){
    return circleBottomLeft(p) || circleTopLeft(p) || circleTopRight(p) || circleBottomRight(p); };

  RectPredicate leftRect = rect(RealPoint(x-cornerRadius,y),cornerRadius,height);
  RectPredicate topRect = rect(RealPoint(x,y+height),width,cornerRadius);
  RectPredicate rightRect = rect(RealPoint(x+width,y),cornerRadius,height);
  RectPredicate bottomRect = rect(RealPoint(x,y-cornerRadius),width,cornerRadius);
  RectPredicate mainRect = rect(RealPoint(x,y),width,height);

  innerRect = [mainRect,leftRect,topRect,rightRect,bottomRect](const RealPoint& p){
    return mainRect(p) || leftRect(p) || topRect(p) || rightRect(p) || bottomRect(p);
  };

  lb = RealPoint(x-cornerRadius,y-cornerRadius);
  ub = RealPoint(x+width+cornerRadius,y+height+cornerRadius);
}

RoundedRect::CirclePredicate RoundedRect::circle(const RealPoint& center, double radius)
{
  return [center,radius](const RealPoint& p){ return ( pow(center[0] - p[0],2) + pow(center[1]-p[1],2) - pow(radius,2) ) <= 0; };
}

RoundedRect::RectPredicate RoundedRect::rect(const RealPoint& bottomLeft, double width, double height){
  return [bottomLeft,width,height](const RealPoint& p){
    return ( (p[0] >= bottomLeft[0] && p[0] <= bottomLeft[0] + width) &&
        (p[1] >= bottomLeft[1] && p[1] <= bottomLeft[1] + height) );};
}

DGtal::Orientation RoundedRect::orientation(const RealPoint& aPoint) const
{
  if( innerRect(aPoint) || cornerCircle(aPoint) ) return DGtal::Orientation::INSIDE;
  else return DGtal::Orientation::OUTSIDE;
}