#include <graph-flow/core/neighborhood/MorphologyNeighborhood.h>
#include <graph-flow/shape-evolution/shape-evolution.h>
#include <graph-flow/utils/digital/morphology.h>
#include <graph-flow/utils/digital/shapes.h>
#include <graph-flow/utils/digital/transform.h>
#include <graph-flow/utils/display.h>

#include <boost/filesystem.hpp>

using namespace GraphFlow;

struct OutputParams {
  std::string baseDataFolder =
      "/home/daniel/Projects/Git/gf-paper-figures/figures/"
      "didactic-one-iteration/data";

  std::string baseOutputFolder =
      "/home/daniel/Projects/Git/gf-paper-figures/figures/"
      "didactic-one-iteration/output/didactic-one-iteration";
};

ShapeEvolution::IterationCallback getCallback(const std::string& dataFolder) {
  return [dataFolder](const ShapeEvolution::GraphFlowIteration& gfi) {
    std::string outputFilepath =
        dataFolder + "/" + std::to_string(gfi.iteration) + ".png";
    Utils::Display::saveDigitalSetAsImage(gfi.ds, outputFilepath);

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

  boost::filesystem::copy(dataFolder + "/0.png", outputFolder + "/0.png",
                          boost::filesystem::copy_options::overwrite_existing);
  boost::filesystem::copy(
      dataFolder + "/" + std::to_string(gfi.iterations) + ".png",
      outputFolder + "/" + std::to_string(gfi.iterations) + ".png",
      boost::filesystem::copy_options::overwrite_existing);
}

int main(int argc, char* argv[]) {
  std::string baseDataFolder =
      "/home/daniel/Projects/Git/gf-paper-figures/figures/"
      "didactic-one-iteration/data";

  std::string baseOutputFolder =
      "/home/daniel/Projects/Git/gf-paper-figures/figures/"
      "didactic-one-iteration/output/didactic-one-iteration";

  double h = 0.125;

  auto _flower = Utils::Digital::Shapes::flower(h, 0, 0, 10, 5, 2);
  auto flower = Utils::Digital::Transform::bottomLeftBoundingBoxAtOrigin(
      _flower, DGtal::Z2i::Point(40, 40));

  ShapeEvolution::GraphFlowInput gfi(flower);
  gfi.border = 100;
  gfi.h = h;
  gfi.neighborhoodSize = 0;
  gfi.iterations = 5;

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