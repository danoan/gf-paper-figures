#include "display.h"

namespace Display {
void displayVoronoi(const DigitalSet& shape, const Domain& domain, DTL2& dt,
                    const std::string& outputFilepath) {
  using namespace DGtal::Z2i;

  DigitalSet shapeD(domain);
  DIPaCUS::Morphology::dilate(
      shapeD, shape,
      DIPaCUS::Morphology::StructuringElement(
          DIPaCUS::Morphology::StructuringElement::CIRCLE, 3),
      1);

  Curve curve1;
  DIPaCUS::Misc::computeBoundaryCurve(curve1, shapeD);

  // Set up colorMap
  std::vector<DGtal::Color> colors = {DGtal::Color::Yellow, DGtal::Color::Blue,
                                      DGtal::Color::Black, DGtal::Color::Green,
                                      DGtal::Color::Red};
  std::map<Point, DGtal::Color> colorMap;
  {
    auto range = curve1.getInnerPointsRange();
    auto it = range.begin();
    int i = 0;
    do {
      Point p = *it;
      Point vp = dt.getVoronoiVector(p);
      if (colorMap.find(vp) == colorMap.end()) {
        colorMap[vp] = colors[i % 5];
        ++it;
        ++i;
      }
    } while (it != range.end());
  }

  // Display dilated contour and closest point to shape
  DGtal::Board2D board;
  {
    auto range = curve1.getInnerPointsRange();
    auto it = range.begin();
    int i = 0;
    do {
      Point p = *it;
      Point vp = dt.getVoronoiVector(p);

      auto color = colorMap[vp];

      board << DGtal::CustomStyle(p.className(),
                                  new DGtal::CustomColors(color, color))
            << p << vp;
      ++it;
      ++i;
    } while (it != range.end());
  }
  board.saveSVG(outputFilepath.c_str());
}

void displayCost(const Domain& KDomain, const DigitalSet& shapeK,
                 const DigitalSet& innerContourK, double h, double ringWidth,
                 const std::unordered_map<Point, EstimationData>& edMap,
                 const DTL2& dtInn, const DTL2& dtOut,
                 const std::string& outputFilepath, std::ostream& os,
                 double maxCM) {
  typedef DGtal::GradientColorMap<double,
                                  DGtal::ColorGradientPreset::CMAP_GRAYSCALE>
      MyColorMap;

  MyColorMap hmap(0, maxCM);
  double M = 0;

  DigitalSet ring(KDomain);
  for (Point p : KDomain) {
    double distance;
    if (shapeK(p))
      distance = dtInn(p);
    else
      distance = dtOut(p);

    if (distance <= ringWidth) ring.insert(p);
  }

  DGtal::Board2D board;
  for (Point p : ring) {
    Point closestPoint;
    double sDistance;
    if (shapeK(p)) {
      closestPoint = dtInn.getVoronoiVector(p);
      sDistance = -dtInn(p) / (2 * h);
    } else {
      closestPoint = dtOut.getVoronoiVector(p);
      sDistance = dtOut(p) / (2 * h);
    }

    if (edMap.find(closestPoint) == edMap.end()) {
      // throw std::runtime_error("Linel or pointel missing");
      continue;
    }

    EstimationData ed = edMap.at(closestPoint);
    double curvatureCost;
    if (ed.curvature > 0)
      curvatureCost = 1.0 / (1.0 / ed.curvature + sDistance);
    else
      curvatureCost = 1.0 / (1.0 / ed.curvature - sDistance);

    // double e = ed.localLength + ed.localLength * pow(curvatureCost, 2);
    double e = pow(curvatureCost, 2);
    if (e > M) M = e;

    board << DGtal::CustomStyle(p.className(),
                                new DGtal::CustomColors(hmap(e), hmap(e)))
          << p;

    os << "mapped curvature:" << ed.curvature
       << "; curvature cost:" << curvatureCost << "; cost:" << e
       << "; distance:" << sDistance << "\n";
  }
  os << M << "\n";

  for (Point p : innerContourK) {
    board << DGtal::CustomStyle(p.className(),
                                new DGtal::CustomColors(DGtal::Color::Blue,
                                                        DGtal::Color::Blue))
          << p;
  }

  board.saveSVG(outputFilepath.c_str());
}
}  // namespace Display