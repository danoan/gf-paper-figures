#include <DGtal/geometry/curves/ArithmeticalDSSComputer.h>
#include <DGtal/geometry/curves/estimation/MostCenteredMaximalSegmentEstimator.h>
#include <DGtal/geometry/curves/estimation/SegmentComputerEstimators.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>
#include <graph-flow/core/neighborhood/MorphologyNeighborhood.h>
#include <graph-flow/shape-evolution/shape-evolution.h>
#include <graph-flow/utils/digital/contour.h>
#include <graph-flow/utils/digital/shapes.h>
#include <graph-flow/utils/string.h>

#include <vector>

#include "DGtal/base/Common.h"
#include "DGtal/geometry/curves/SaturatedSegmentation.h"
#include "DGtal/shapes/Shapes.h"

using namespace DGtal::Z2i;
using namespace GraphFlow::Utils;

namespace ConcaveConvex {
using namespace std;
using namespace DGtal;
using namespace Z2i;

template <typename Iterator, typename Board>
int drawCCP(const Iterator& itb, const Iterator& ite, Board& aBoard) {
  // choose the drawing mode
  aBoard << SetMode("ArithmeticalDSS", "BoundingBox");
  // prepare the drawing style and the pen color
  string aStyleName = "ArithmeticalDSS/BoundingBox";
  CustomPenColor* aPenColor;

  int inflectionPoints = 0;
  // for each maximal segment
  for (Iterator i(itb); i != ite; ++i) {
    // get the current maximal segment
    typedef typename Iterator::SegmentComputer::Primitive DSS;
    DSS maximalDSS = i->primitive();

    // if located at the end of a connected part
    if (!(i.intersectNext() && i.intersectPrevious())) {
      aPenColor = new CustomPenColor(Color::Black);

      // otherwise
    } else {
      // get the points located before and after the maximal segment
      typedef typename DSS::Point Point;
      Point beforeFirst = *(--(i->begin()));
      Point afterLast = *(i->end());

      // remainders and bounds
      typedef typename DSS::Integer Integer;
      Integer r1 = maximalDSS.remainder(beforeFirst);
      Integer r2 = maximalDSS.remainder(afterLast);
      Integer mu = maximalDSS.mu();
      Integer omega = maximalDSS.omega();

      // configurations
      if ((r1 <= mu - 1) && (r2 <= mu - 1)) {  // concave
        aPenColor = new CustomPenColor(Color::Green);
      } else if ((r1 >= mu + omega) && (r2 >= mu + omega)) {  // convex
        aPenColor = new CustomPenColor(Color::Blue);
      } else if ((r1 >= mu + omega) && (r2 <= mu - 1)) {  // convex to concave
        aPenColor = new CustomPenColor(Color::Yellow);
        inflectionPoints++;
      } else if ((r1 <= mu - 1) && (r2 >= mu + omega)) {  // concave to convex
        aPenColor = new CustomPenColor(Color::Yellow);
        inflectionPoints++;
      } else {  // pb
        aPenColor = new CustomPenColor(Color::Red);
      }
    }

    // draw the maximal segment on the board
    aBoard << CustomStyle(aStyleName, aPenColor) << maximalDSS;
  }

  return inflectionPoints;
}

template <typename Iterator, typename Board>
int segmentationIntoMaximalDSSs(const Iterator& itb, const Iterator& ite,
                                Board& aBoard) {
  typedef
      typename IteratorCirculatorTraits<Iterator>::Value::Coordinate Coordinate;

  // choose the primitive computer and the segmentation
  typedef ArithmeticalDSSComputer<Iterator, Coordinate, 4> RecognitionAlgorithm;
  typedef SaturatedSegmentation<RecognitionAlgorithm> Segmentation;

  // create the segmentation
  RecognitionAlgorithm algo;
  Segmentation s(itb, ite, algo);

  // draw the result
  return drawCCP(s.begin(), s.end(), aBoard);
}

}  // namespace ConcaveConvex

namespace Estimation {
typedef Curve::ConstIterator CurveIterator;

typedef DGtal::functors::SCellToPoint<DGtal::Z2i::KSpace> AdapterFunctor;
typedef DGtal::ConstRangeAdapter<CurveIterator, AdapterFunctor, KSpace::Point>
    RangeAdapter;

typedef DGtal::ArithmeticalDSSComputer<RangeAdapter::ConstIterator, int, 4>
    SegmentComputer;
typedef DGtal::TangentFromDSSEstimator<SegmentComputer> SCEstimator;
typedef DGtal::PointVector<2, double> TangentVector;

void tangentEstimation(std::vector<TangentVector>& ev, const KSpace& kspace,
                       CurveIterator itb, CurveIterator ite, double h) {
  // Creating a range for points from the GridCurve
  AdapterFunctor myFunctor(kspace);
  RangeAdapter range(itb, ite, myFunctor);

  // Initializing the tangent estimator
  SegmentComputer sc;
  SCEstimator f;

  DGtal::MostCenteredMaximalSegmentEstimator<SegmentComputer, SCEstimator>
      MCMDSSTangentEstimator(sc, f);

  MCMDSSTangentEstimator.init(range.begin(), range.end());
  MCMDSSTangentEstimator.eval(range.begin(), range.end(),
                              std::back_inserter(ev), h);
}
}  // namespace Estimation

int countInflectionPoints(const DigitalSet& ds,
                          const std::string& outputFilepath = "") {
  const Domain& domain = ds.domain();
  KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);

  std::vector<std::vector<KSpace::SCell>> contours;
  DGtal::SurfelAdjacency<KSpace::dimension> SAdj(true);
  DGtal::Surfaces<KSpace>::extractAll2DSCellContours(contours, kspace, SAdj,
                                                     ds);

  int inflectionPoints=0;
  for (auto contour : contours) {
    Curve curve;
    curve.initFromSCellsVector(contour);

    auto codesRange = curve.getPointsRange();

    DGtal::Board2D board;
    inflectionPoints += ConcaveConvex::segmentationIntoMaximalDSSs(
        codesRange.begin(), codesRange.end(), board);

    if (outputFilepath != "") {
      board.saveSVG(outputFilepath.c_str());
    }
  }

  return inflectionPoints;
}

void tangentProfile(const DigitalSet& ds, double h, std::ostream& os) {
  double pi = 3.14159265359;
  const Domain& domain = ds.domain();
  KSpace kspace;
  kspace.init(domain.lowerBound(), domain.upperBound(), true);

  std::vector<std::vector<KSpace::SCell>> contours;
  DGtal::SurfelAdjacency<KSpace::dimension> SAdj(true);
  DGtal::Surfaces<KSpace>::extractAll2DSCellContours(contours, kspace, SAdj,
                                                     ds);

  int longestLength = 0;
  int longestId = 0;
  for (int i = 0; i < contours.size(); ++i) {
    if (contours[i].size() > longestLength) {
      longestLength = contours[i].size();
      longestId = i;
    }
  }

  Curve curve;
  curve.initFromSCellsVector(contours[longestId]);
  std::vector<Estimation::TangentVector> ev;
  Estimation::tangentEstimation(ev, kspace, curve.begin(), curve.end(), h);

  os << "n,tangent,t2\n";
  double curr;
  double f;
  for (int i = 0; i < ev.size(); ++i) {
    curr = atan2(ev[i][1], ev[i][0]);
    if (curr < 0)
      f = 2 * pi;
    else
      f = 0;
    os << i << "," << curr + f << "," << curr << "\n";
  }
}

double computeAreaSimilarity(const DigitalSet& dsBase,
                             const DigitalSet& dsCompareWith) {
  double totalPoints = dsBase.size();
  double diffCount = 0;
  for (auto p : dsCompareWith) {
    if (!dsBase(p)) {
      diffCount += 1;
    }
  }

  for (auto p : dsBase) {
    if (!dsCompareWith(p)) {
      diffCount += 1;
    }
  }

  return 1 - (diffCount / totalPoints);
}

enum Operation { Tangent, Inflection, Elastica, AreaDiff };

struct InputData {
  std::string inputFilepathBase;
  std::string inputFilepathCompareWith;
  std::string outputFilepath;

  double h;
  double estimationRadius;
  Operation op;
};

InputData readInput(int argc, char* argv[]) {
  InputData id;
  int opt;
  while ((opt = getopt(argc, argv, "i:I:o:h:e:p:")) != -1) {
    switch (opt) {
      case 'i': {
        id.inputFilepathBase = optarg;
        break;
      }
      case 'I': {
        id.inputFilepathCompareWith = optarg;
        break;
      }
      case 'o': {
        id.outputFilepath = optarg;
        break;
      }
      case 'h': {
        id.h = std::atof(optarg);
        break;
      }
      case 'e': {
        id.estimationRadius = std::atof(optarg);
        break;
      }
      case 'p': {
        if (strcmp(optarg, "tangent") == 0) {
          id.op = Operation::Tangent;
        } else if (strcmp(optarg, "inflection") == 0) {
          id.op = Operation::Inflection;
        } else if (strcmp(optarg, "elastica") == 0) {
          id.op = Operation::Elastica;
        } else if (strcmp(optarg, "area-diff") == 0) {
          id.op = Operation::AreaDiff;
        } else {
          throw std::runtime_error("Operation not recognized.");
          exit(1);
        }
        break;
      }
      default: {
        throw std::runtime_error("Argument not recognized.");
        exit(1);
      }
    }
  }

  return id;
}

int main(int argc, char* argv[]) {
  typedef Digital::Representation::Image2D Image2D;

  InputData id = readInput(argc, argv);

  std::ostream* os;
  if (id.outputFilepath.empty()) {
    os = &std::cout;
  } else {
    os = new std::ofstream(id.outputFilepath);
  }

  Image2D imageBase =
      DGtal::GenericReader<Image2D>::import(id.inputFilepathBase);
  DigitalSet dsBase(imageBase.domain());
  Digital::Representation::imageAsDigitalSet(dsBase, imageBase);

  switch (id.op) {
    case Operation::Tangent: {
      tangentProfile(dsBase, id.h, *os);
      break;
    }
    case Operation::Elastica: {
      *os << Energy::elastica(dsBase, id.estimationRadius, id.h, 0, 1);
      break;
    }
    case Operation::Inflection: {
      *os << countInflectionPoints(dsBase);
      break;
    }
    case Operation::AreaDiff: {
      Image2D imageCompareWith =
          DGtal::GenericReader<Image2D>::import(id.inputFilepathCompareWith);
      DigitalSet dsCompareWith(imageBase.domain());
      Digital::Representation::imageAsDigitalSet(dsCompareWith,
                                                 imageCompareWith);

      *os << std::setprecision(2)
          << computeAreaSimilarity(dsBase, dsCompareWith);
      break;
    }
  }

  if (!id.outputFilepath.empty()) {
    delete os;
  }

  return 0;
}
