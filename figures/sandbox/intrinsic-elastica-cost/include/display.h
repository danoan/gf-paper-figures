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

#include "CostData.h"
#include "EstimationData.h"
#include "KhalimskyEquivalent.h"
#include "CostFunction.h"
#include "OptRegions.h"

namespace Display {
typedef DGtal::Z2i::DigitalSet DigitalSet;
typedef DGtal::Z2i::Domain Domain;
typedef GraphFlow::Utils::Digital::DTL2 DTL2;
typedef DGtal::Z2i::Point Point;

void displayVoronoi(const DigitalSet& shape, const Domain& domain, DTL2& dt,
                    const std::string& outputFilepath);

void displayCost(const CostFunction& costFunction,
                 const DigitalSet& innerContourK,
                 const std::string& outputFilepath, std::ostream& os);

void displayOptRegions(const OptRegions& optRegions,
                       const std::string& outputFilepath);

void simpleDisplay(const DigitalSet& ds, const std::string& outputFilepath);

void displayRing(const KhalimskyEquivalent& ke, const DigitalSet& ring,
                 const std::string& outputFilepath);
}  // namespace Display

#endif