#ifndef COST_FUNCTION_H
#define COST_FUNCTION_H

#include <DGtal/geometry/volumes/distance/DistanceTransformation.h>
#include <DGtal/helpers/StdDefs.h>

#include <unordered_map>

#include "CostData.h"
#include "EstimationData.h"
#include "KhalimskyEquivalent.h"
#include "utils.h"

class CostFunction {
 public:
  typedef DGtal::Z2i::Point LinelKCoords;
  typedef std::unordered_map<LinelKCoords, CostData> LinelCostMap;
  typedef std::unordered_map<LinelKCoords, EstimationData> EstimationDataMap;

 private:
  typedef DGtal::Z2i::DigitalSet DigitalSet;
  typedef DGtal::DistanceTransformation<DGtal::Z2i::Space, DigitalSet,
                                        DGtal::Z2i::L2Metric>
      DTL2;

 public:
  CostFunction(const KhalimskyEquivalent& ke, const EstimationDataMap& edMap,
               double h, double ringWidth, double alpha, double beta,bool onlySquaredCurvature=false);

  inline const CostData& at(const LinelKCoords& lc) const {
    return m_linelCostMap.at(lc);
  }

  inline LinelCostMap::const_iterator begin() const {
    return m_linelCostMap.begin();
  }
  inline LinelCostMap::const_iterator end() const {
    return m_linelCostMap.end();
  }

 private:
  LinelKCoords inline getClosestPoint(const LinelKCoords& p,
                                      const DigitalSet& shapeK,
                                      const DTL2& dtInn, const DTL2& dtOut);

  inline double getSDistance(const LinelKCoords& p, double h,
                             const DigitalSet& shapeK, const DTL2& dtInn,
                             const DTL2& dtOut);

  inline double getCurvatureCost(double curvatureEstimation, double sDistance);

 private:
  LinelCostMap m_linelCostMap;
};

#endif