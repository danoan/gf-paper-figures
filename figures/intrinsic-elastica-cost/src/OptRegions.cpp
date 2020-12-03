#include "OptRegions.h"

OptRegions::OptRegions(const Domain& domain, const DigitalSet& shape,
                       const CostFunction& costFunction)
    : domain(domain), optBand(domain), sureFg(domain) {
  setOptBand(costFunction);
  setTerminalPoints(shape);
  DIPaCUS::SetOperations::setDifference(sureFg, shape, optBand);
}

void OptRegions::setOptBand(const CostFunction& costFunction) {
  for (auto pk : costFunction) {
    Point linelKCoords = pk.first;
    CostData cd = pk.second;

    optBand.insert(cd.pixel1);
    optBand.insert(cd.pixel2);
  }
}

void OptRegions::setTerminalPoints(const DigitalSet& shape) {
  using namespace DGtal::Z2i;

  const Domain& domain = optBand.domain();
  DGtal::SurfelAdjacency<2> sadj(true);
  std::vector<std::vector<SCell> > vscells;

  KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);

  DGtal::Surfaces<KSpace>::extractAll2DSCellContours(vscells, kspace, sadj,
                                                     optBand);

  for (auto seqSCells : vscells) {
    Curve c;
    c.initFromSCellsVector(seqSCells);
    auto range = c.getInnerPointsRange();
    auto it = range.begin();
    bool insideFlag = shape(*it);
    do {
      if (insideFlag)
        sourcePoints.insert(*it);
      else
        targetPoints.insert(*it);
      ++it;
    } while (it != range.end());
  }
}