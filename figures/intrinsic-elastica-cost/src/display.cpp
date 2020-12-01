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

void displayCost(const std::unordered_map<Point,CostData>& costFunction,const DigitalSet& innerContourK,
                 const std::string& outputFilepath, std::ostream& os,
                 double maxCM) {
  typedef DGtal::GradientColorMap<double,
                                  DGtal::ColorGradientPreset::CMAP_GRAYSCALE>
      MyColorMap;

  MyColorMap hmap(0, maxCM);
  double M = 0;

  DGtal::Board2D board;
  for (auto pk : costFunction) {
    Point p = pk.first;
    double cost = pk.second.cost;

    if (cost > M) M = cost;

    board << DGtal::CustomStyle(p.className(),
                                new DGtal::CustomColors(hmap(cost), hmap(cost)))
          << p;
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