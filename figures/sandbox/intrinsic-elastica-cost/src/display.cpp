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

void displayCost(const CostFunction& costFunction,
                 const DigitalSet& innerContourK,
                 const std::string& outputFilepath, std::ostream& os) {
  typedef DGtal::GradientColorMap<double, DGtal::ColorGradientPreset::CMAP_JET>
      MyColorMap;

  double M = 0;
  double m = 100;

  for (auto pk : costFunction) {
    double cost = pk.second.cost;
    if (cost > M) M = cost;
    if (cost < m) m = cost;
  }

  os << std::fixed << std::setprecision(3) << "Min unit cost: " << m << "\n"
     << "Max unit cost: " << M << "\n";

  MyColorMap hmap(m, M);

  DGtal::Board2D board;
  for (auto pk : costFunction) {
    Point p = pk.first;
    double cost = pk.second.cost;

    if (cost > M) M = cost;
    if (cost < m) m = cost;

    board << DGtal::CustomStyle(p.className(),
                                new DGtal::CustomColors(hmap(cost), hmap(cost)))
          << p;
  }

  for (Point p : innerContourK) {
    board << DGtal::CustomStyle(p.className(),
                                new DGtal::CustomColors(DGtal::Color::White,
                                                        DGtal::Color::White))
          << p;
  }

  board.saveSVG(outputFilepath.c_str());
}

void displayOptRegions(const OptRegions& optRegions,
                       const std::string& outputFilepath) {
  DGtal::Board2D board;
  std::set<Point> optSourceIntersection;
  for (auto p : optRegions.sourcePoints)
    if (optRegions.optBand(p)) optSourceIntersection.insert(p);

  std::set<Point> optTargetIntersection;
  for (auto p : optRegions.targetPoints)
    if (optRegions.optBand(p)) optTargetIntersection.insert(p);

  for (auto p : optRegions.sureFg) {
    board << DGtal::CustomStyle(p.className(),
                                new DGtal::CustomColors(DGtal::Color::Green,
                                                        DGtal::Color::Green))
          << p;
  }
  for (auto p : optRegions.sourcePoints) {
    board << DGtal::CustomStyle(p.className(),
                                new DGtal::CustomColors(DGtal::Color::Blue,
                                                        DGtal::Color::Blue))
          << p;
  }
  for (auto p : optRegions.targetPoints) {
    board << DGtal::CustomStyle(
                 p.className(),
                 new DGtal::CustomColors(DGtal::Color::Red, DGtal::Color::Red))
          << p;
  }
  for (auto p : optRegions.optBand) {
    board << DGtal::CustomStyle(p.className(),
                                new DGtal::CustomColors(DGtal::Color::Yellow,
                                                        DGtal::Color::Yellow))
          << p;
  }
  for (auto p : optSourceIntersection) {
    board << DGtal::CustomStyle(p.className(),
                                new DGtal::CustomColors(DGtal::Color::Lime,
                                                        DGtal::Color::Lime))
          << p;
  }
  for (auto p : optTargetIntersection) {
    board << DGtal::CustomStyle(p.className(),
                                new DGtal::CustomColors(DGtal::Color::Magenta,
                                                        DGtal::Color::Magenta))
          << p;
  }
  board.saveSVG(outputFilepath.c_str());
}

void simpleDisplay(const DigitalSet& ds, const std::string& outputFilepath) {
  DGtal::Board2D board;
  board << ds.domain() << ds;
  board.saveSVG(outputFilepath.c_str());
}

void displayRing(const KhalimskyEquivalent& ke, const DigitalSet& ring,
                 const std::string& outputFilepath) {
  DGtal::Board2D board;
  board << DGtal::CustomStyle(
               ring.className(),
               new DGtal::CustomColors(DGtal::Color::Red, DGtal::Color::Red))
        << ring
        << DGtal::CustomStyle(ring.className(),
                              new DGtal::CustomColors(DGtal::Color::Yellow,
                                                      DGtal::Color::Yellow))
        << ke.innerContourK;
  board.saveSVG(outputFilepath.c_str());
}
}  // namespace Display