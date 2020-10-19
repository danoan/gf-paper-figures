#ifndef GRAPH_FLOW_PAPER_FIGURES_FIGURES_NEED_FOR_SQUARED_CURVATURE_INCLUDE_ENERGY_H_
#define GRAPH_FLOW_PAPER_FIGURES_FIGURES_NEED_FOR_SQUARED_CURVATURE_INCLUDE_ENERGY_H_

#include <DGtal/helpers/StdDefs.h>

#include <geoc/api/gridCurve/Length.hpp>
#include <geoc/api/gridCurve/Curvature.hpp>
#include <geoc/estimator/adaptable/Curvature.h>

double totalSquaredCurvature(const DGtal::Z2i::DigitalSet& ds,double estimationRadius,double h);
double perimeter(const DGtal::Z2i::DigitalSet& ds,double h);
double elastica(const DGtal::Z2i::DigitalSet& ds,double h,double estimationRadius,double alpha, double beta);

#endif //GRAPH_FLOW_PAPER_FIGURES_FIGURES_NEED_FOR_SQUARED_CURVATURE_INCLUDE_ENERGY_H_
