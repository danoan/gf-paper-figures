#ifndef DISPLAY_H
#define DISPLAY_H

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/io/colormaps/GradientColorMap.h>
#include <DIPaCUS/components/Morphology.h>
#include <DIPaCUS/derivates/Misc.h>
#include <graph-flow/utils/digital.h>

#include <map>
#include <string>
#include <unordered_map>

#include "EstimationData.h"

namespace Display {
typedef DGtal::Z2i::DigitalSet DigitalSet;
typedef DGtal::Z2i::Domain Domain;
typedef GraphFlow::Utils::Digital::DTL2 DTL2;
typedef DGtal::Z2i::Point Point;

void displayVoronoi(const DigitalSet& shape, const Domain& domain, DTL2& dt,
                    const std::string& outputFilepath);

void displayCost(const Domain& KDomain, const DigitalSet& shapeK,
                 const DigitalSet& innerContourK, double h, double ringWidth,
                 const std::unordered_map<Point, EstimationData>& edMap,
                 const DTL2& dtInn, const DTL2& dtOut,
                 const std::string& outputFilepath, std::ostream& os,
                 double maxCM = 1);
}  // namespace Display

#endif