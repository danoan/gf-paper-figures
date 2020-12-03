#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/derivates/Misc.h>
#include <graph-flow/utils/digital.h>
#include <graph-flow/utils/display.h>
#include <graph-flow/utils/energy.h>
#include <graph-flow/utils/string.h>

#include <boost/filesystem.hpp>
#include <unordered_map>
#include <vector>

#include "CostData.h"
#include "EstimationData.h"
#include "FlowGraph.h"
#include "InputData.h"
#include "KhalimskyEquivalent.h"
#include "display.h"
#include "estimators.h"
#include "utils.h"

using namespace DGtal::Z2i;
typedef GraphFlow::Utils::Digital::DTL2 DTL2;
typedef DGtal::Z2i::Point LinelKCoords;

void setEstimationDataMap(std::unordered_map<Point, EstimationData>& edMap,
                          const Curve& curve, const Domain& domain,
                          const DigitalSet& shape,
                          InputData::CurvatureEstimator curvatureEstimator,
                          InputData::LengthEstimator lengthEstimator,
                          double h) {
  std::vector<double> kV;

  if (curvatureEstimator == InputData::MDCA) {
    Estimators::curvatureMDCA(kV, curve, domain, h);
  }

  else if (curvatureEstimator == InputData::II) {
    Estimators::curvatureII(kV, curve, domain, shape, 5, h);
  }

  // To implement both length estimators
  std::vector<double> lV;
  Estimators::localLength(lV, curve, domain, h);

  std::vector<RealPoint> tV;
  Estimators::tangent(tV, curve, domain, h);

  KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);

  auto scellRange = curve.getSCellsRange();
  auto it = scellRange.begin();
  int i = 0;
  do {
    Point p = it->preCell().coordinates;
    edMap[p] = EstimationData{tV[i], RealPoint(tV[1], -tV[0]), lV[i], kV[i]};

    SCell pointel = kspace.sDirectIncident(*it, *kspace.sDirs(*it));
    edMap[kspace.sKCoords(pointel)] = edMap[p];
    it++;
    i++;
  } while (it != scellRange.end());
}

DigitalSet computeCut(const OptRegions& optRegions,
                      const CostFunction& costFunction) {
  FlowGraph fg(optRegions.optBand, optRegions.sourcePoints,
               optRegions.targetPoints, costFunction);
  std::cout << "Cut Value:" << fg.cutValue() << std::endl;

  DigitalSet sourceNodes(optRegions.domain);
  for (Point p : optRegions.optBand) {
    if (fg.isInSource(p)) sourceNodes.insert(p);
  }
  sourceNodes.insert(optRegions.sureFg.begin(), optRegions.sureFg.end());

  return sourceNodes;
}

int main(int argc, char* argv[]) {
  InputData id = readInput(argc, argv);
  boost::filesystem::create_directories(id.outputFolder);

  Point border((int)ceil(40 / id.h), (int)ceil(40 / id.h));

  DigitalSet _shape =
      GraphFlow::Utils::Digital::resolveShape(id.shapeName, id.h);
  Domain domain(_shape.domain().lowerBound() - border,
                _shape.domain().upperBound() + border);

  DigitalSet shape(domain);
  shape.insert(_shape.begin(), _shape.end());

  GraphFlow::Utils::Display::saveDigitalSetAsImage(
      shape, id.outputFolder + "/it-0.png");

  double currElastica =
      GraphFlow::Utils::Energy::elastica(shape, 5, id.h, id.alpha, id.beta);
  std::cout << "Elastica: " << currElastica << std::endl;

  int it = 0;
  while (it < id.maxIt) {
    Curve shapeContour;
    DIPaCUS::Misc::computeBoundaryCurve(shapeContour, shape);

    std::unordered_map<Point, EstimationData> edMap;
    setEstimationDataMap(edMap, shapeContour, domain, shape,
                         id.curvatureEstimator, id.lengthEstimator, id.h);

    KhalimskyEquivalent ke(domain, shapeContour, id.h);
    CostFunction cf(ke, edMap, id.h, id.ringWidth, id.alpha, id.beta);
    OptRegions optRegions(domain, shape, cf);

    if (id.displayMaps) {
      Display::displayCost(cf, ke.innerContourK,
                           id.outputFolder + "/costFunction.svg", std::cout);
      Display::displayOptRegions(optRegions,
                                 id.outputFolder + "/optRegions.svg");
    }

    DigitalSet sourceNodes = computeCut(optRegions, cf);

    shape.clear();
    shape.insert(sourceNodes.begin(), sourceNodes.end());
    ++it;

    std::string outputFilepath =
        id.outputFolder + "/it-" +
        GraphFlow::Utils::String::nDigitsString(it, 4) + ".png";
    GraphFlow::Utils::Display::saveDigitalSetAsImage(shape, outputFilepath);

    currElastica =
        GraphFlow::Utils::Energy::elastica(shape, 5, id.h, id.alpha, id.beta);
    std::cout << "Elastica: " << currElastica << "\n\n";
  }

  return 0;
}