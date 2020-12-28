#include <boost/filesystem.hpp>
#include <DGtal/helpers/StdDefs.h>
#include <graph-flow/core/neighborhood/MorphologyNeighborhood.h>
#include <graph-flow/shape-evolution/model/GraphFlowInput.h>
#include <graph-flow/shape-evolution/model/GraphFlowIteration.h>
#include <graph-flow/shape-evolution/shape-evolution.h>
#include <graph-flow/utils/digital.h>
#include <graph-flow/utils/string.h>

#include <iostream>

#include "energy.h"

using namespace DGtal::Z2i;
using namespace GraphFlow;

struct ContinuousFlow {
  ContinuousFlow(double diskRadius) : diskRadius(diskRadius) {}

  inline double perimeter() { return 2 * M_PI * diskRadius; }
  inline double tk2() { return 2 * M_PI / diskRadius; }
  bool next() {
    diskRadius -= 1.0 / diskRadius;
    return diskRadius > 0;
  }

  double diskRadius;
};

struct DiscreteFlow {
  typedef GraphFlow::ShapeEvolution::IterationCallback IterationCallback;
  typedef GraphFlow::ShapeEvolution::GraphFlowIteration GraphFlowIteration;

  DiscreteFlow(const DigitalSet& ds, double gridStep, double estimationRadius)
      : m_ds(ds.domain()),
        m_gridStep(gridStep),
        m_estimationRadius(estimationRadius) {
    m_ds.insert(ds.begin(), ds.end());
    update(ds);
  }

  void setRatio(const DigitalSet& candidate) {
    double candidatePerimeter = perimeter(candidate, m_gridStep);
    m_ratio = (m_perimeter - candidatePerimeter) / m_tk2;
  }

  void update(const DigitalSet& ds) {
    m_ds = ds;
    m_perimeter = perimeter(m_ds, m_gridStep);
    m_tk2 = totalSquaredCurvature(m_ds, m_estimationRadius, m_gridStep);
  }

  IterationCallback getCallback(bool updateDS) {
    return [this, updateDS](const GraphFlowIteration& gfIteration) -> void {
      switch (gfIteration.iterationState) {
        case GraphFlowIteration::Init: {
          break;
        }
        case GraphFlowIteration::Running: {
          this->setRatio(gfIteration.ds);
          if (updateDS) this->update(gfIteration.ds);
          break;
        }
        case GraphFlowIteration::End: {
          break;
        }
      }
    };
  }

  DigitalSet m_ds;
  double m_perimeter;
  double m_tk2;
  double m_ratio;

  double m_gridStep;
  double m_estimationRadius;
};

void print(std::ostream& os, const ContinuousFlow& CF, const DiscreteFlow& DF,
           bool header = false) {
  using namespace GraphFlow::Utils::String;

  int numCols = 16;

  if (header) {
    os << fixedStrLength(numCols, "C. Radius")
       << fixedStrLength(numCols, "D. Radius (~)")
       << fixedStrLength(numCols, "D. Bal. Radius")
       << fixedStrLength(numCols, "D. Perimeter")
       << fixedStrLength(numCols, "D. TK2")
       << fixedStrLength(numCols, "D. Ratio") << "\n";
  }

  os << fixedStrLength(numCols, CF.diskRadius)
     << fixedStrLength(numCols, DF.m_perimeter / (2 * M_PI))
     << fixedStrLength(numCols, DF.m_estimationRadius)
     << fixedStrLength(numCols, DF.m_perimeter)
     << fixedStrLength(numCols, DF.m_tk2) << fixedStrLength(numCols, DF.m_ratio)
     << "\n";
}

double ratioDiff(double ratio) { return std::fabs(ratio - 1); }

int main(int argc, char* argv[]) {
  std::ostream& os = std::cout;

  const double diskRadius = std::atof(argv[1]);
  const double gridStep = std::atof(argv[2]);
  const int maxIterations = std::atoi(argv[3]);
  std::string outputFilepath = argv[4];

  boost::filesystem::path p(outputFilepath);
  boost::filesystem::create_directories(p.remove_filename());

  std::ofstream ofs(outputFilepath);
  ContinuousFlow CF(diskRadius);

  DigitalSet diskShape = DIPaCUS::Shapes::ball(gridStep, 0, 0, diskRadius);
  DiscreteFlow DF(diskShape, gridStep, CF.diskRadius / gridStep);

  GraphFlow::Core::Neighborhood::Morphology M(
      GraphFlow::Core::Neighborhood::Morphology::MorphologyElement::CIRCLE, 0);

  
  ofs << "Find balance radius\n\n" 
  << "Disk Radius: " << diskRadius << "\n"
  << "Grid Step: " << gridStep << "\n"
  << "Max Iterations: " << maxIterations << "\n\n";

  int iteration = 0;
  print(ofs, CF, DF, true);
  while (iteration < maxIterations) {
    if( CF.next()==false) break;

    double bestRatio = 10;
    double bestBR = CF.diskRadius / gridStep;

    GraphFlow::ShapeEvolution::GraphFlowInput gfi(DF.m_ds);
    gfi.radius = bestBR;
    gfi.iterations = 1;
    while (ratioDiff(bestRatio) > 0.1 && gfi.radius > 1) {      
      //false: do not update DF
      auto callback = DF.getCallback(false);
      graphFlow(gfi, M, std::cout, callback);

      if (ratioDiff(DF.m_ratio) < ratioDiff(bestRatio)) {
        bestRatio = DF.m_ratio;
        bestBR = gfi.radius;
      }
      gfi.radius -= 1;
    }

    gfi.radius = bestBR;
    //true: update DF for evolution using bestBR
    auto callback = DF.getCallback(true);
    graphFlow(gfi, M, std::cout, callback);
    DF.m_estimationRadius = bestBR;

    print(ofs, CF, DF);
    if( DF.m_ds.size() < 10 ) break;

    iteration++;
  }

  ofs.flush();
  ofs.close();

  return 0;
}