#include <graph-flow/core/neighborhood/MorphologyNeighborhood.h>
#include <graph-flow/shape-evolution/shape-evolution.h>
#include <graph-flow/utils/digital/morphology.h>
#include <graph-flow/utils/digital/shapes.h>
#include <graph-flow/utils/digital/transform.h>
#include <graph-flow/utils/display.h>

#include "DGtal/io/boards/Board2D.h"
#include "DGtal/io/readers/GenericReader.h"
#include "DGtal/images/ImageContainerBySTLVector.h"

#include <boost/filesystem.hpp>

using namespace GraphFlow;

struct OutputParams {
  std::string baseDataFolder =
      "/home/daniel/Projects/Git/phd-stuff/gf-paper-figures/figures/"
      "didactic-one-iteration/data";

  std::string baseOutputFolder =
      "/home/daniel/Projects/Git/phd-stuff/gf-paper-figures/figures/"
      "didactic-one-iteration/output/didactic-one-iteration";
};

struct InverseIdentity {
    unsigned char operator()(unsigned char aValue) const {
        return 255-aValue; 
    }
};

DGtal::Z2i::DigitalSet createSubSampledDigitalSetFromImage(const DGtal::Z2i::DigitalSet& inputDS){


    typedef DGtal::ImageContainerBySTLVector < DGtal::Z2i::Domain, unsigned char> Image2D;
    typedef DGtal::ConstImageAdapter<Image2D,  Image2D::Domain, 
                          DGtal::functors::BasicDomainSubSampler<Image2D::Domain>,  
                          unsigned char,
                          InverseIdentity > ConstImageAdapterForSubSampling;

    Image2D image2D = DGtal::ImageFromSet<Image2D>::create(inputDS,255);
    std::vector<DGtal::Z2i::Domain::Size> relGridRatio{ 16, 16};

    // Construction of the functor for the 2D domain subsampling from a given grid size and shift.
    DGtal::functors::BasicDomainSubSampler<Image2D::Domain> subSampler2D(image2D.domain(), relGridRatio, DGtal::Z2i::Point(0 ,0));

    InverseIdentity df;
    // Get the new domain produces from the subsampler and define the ConstImageAdapter:
    Image2D::Domain subSampledDomain2D  = subSampler2D.getSubSampledDomain();
    ConstImageAdapterForSubSampling subsampledImage2D (image2D, subSampledDomain2D, subSampler2D, df);

    DGtal::Z2i::DigitalSet ds(subsampledImage2D.domain());
    DGtal::DigitalSetInserter<DGtal::Z2i::DigitalSet> inserter(ds);
    DGtal::setFromImage(subsampledImage2D,inserter);


    return ds;
}

ShapeEvolution::IterationCallback getCallback(const std::string& dataFolder) {
  return [dataFolder](const ShapeEvolution::GraphFlowIteration& gfi) {

    std::string outputFilepathPNG =
        dataFolder + "/" + std::to_string(gfi.iteration) + ".png";
    auto subSampledDigitalSet = createSubSampledDigitalSetFromImage(gfi.ds);
    Utils::Display::saveDigitalSetAsImage(subSampledDigitalSet, outputFilepathPNG);
  
    std::string outputFilepathSVG =
        dataFolder + "/" + std::to_string(gfi.iteration) + ".svg";
    DGtal::Board2D board;

    DGtal::Color cool_green( 190, 216, 153 ); 
    DGtal::Color black( 0, 0, 0 ); 

    board << subSampledDigitalSet.domain() 
          << DGtal::CustomStyle( subSampledDigitalSet.className(), new DGtal::CustomColors( black,cool_green ) )
          << subSampledDigitalSet;
    board.saveSVG(outputFilepathSVG.c_str());

    std::cout << dataFolder << ":" << gfi.value << std::endl;
  };
}

void go(ShapeEvolution::GraphFlowInput& gfi, const OutputParams& op,
        const std::string& suffix) {
  auto neighborhood = Core::Neighborhood::Morphology(
      Core::Neighborhood::Morphology::MorphologyElement::RECT, 0);

  std::string dataFolder = op.baseDataFolder + "/" + suffix;
  std::string outputFolder = op.baseOutputFolder + "/" + suffix;

  boost::filesystem::create_directories(dataFolder);
  boost::filesystem::create_directories(outputFolder);

  ShapeEvolution::IterationCallback callback = getCallback(dataFolder);
  ShapeEvolution::graphFlow(gfi, neighborhood, std::cout, callback);

  boost::filesystem::copy(dataFolder + "/0.svg", outputFolder + "/0.svg",
                          boost::filesystem::copy_options::overwrite_existing);
  boost::filesystem::copy(
      dataFolder + "/" + std::to_string(gfi.iterations) + ".svg",
      outputFolder + "/" + std::to_string(gfi.iterations) + ".svg",
      boost::filesystem::copy_options::overwrite_existing);
}

int main(int argc, char* argv[]) {
  double h = 0.125;

  auto _flower = Utils::Digital::Shapes::flower(h, 0, 0, 16, 10, 2);
  auto flower = Utils::Digital::Transform::bottomLeftBoundingBoxAtOrigin(
      _flower, DGtal::Z2i::Point(40, 40));

  ShapeEvolution::GraphFlowInput gfi(flower);
  gfi.border = 100;
  gfi.h = h;
  gfi.neighborhoodSize = 0;
  gfi.iterations = 25;
  gfi.radius = 9;

  using SE = Utils::Digital::Morphology::StructuringElement;
  auto se = SE(SE::Type::RECT, 3);
  OutputParams op;

  // S0
  go(gfi, op, "S0");

  // S-1
  gfi.inputDS.clear();
  Utils::Digital::Morphology::erode(gfi.inputDS, flower, se, 1);
  go(gfi, op, "S-1");

  // S+1
  gfi.inputDS.clear();
  Utils::Digital::Morphology::dilate(gfi.inputDS, flower, se, 1);
  go(gfi, op, "S+1");

  return 0;
}
