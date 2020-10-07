#include <opencv2/core.hpp>
#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/base/Representation.h>
#include <DIPaCUS/derivates/Misc.h>

using namespace DGtal::Z2i;

void setHighlightMask(cv::Mat& outputImage, const cv::Mat& baseImage, const cv::Mat& mask, const cv::Vec3b color)
{
  outputImage = cv::Mat::zeros(baseImage.size(),baseImage.type());

  baseImage.copyTo(outputImage);

  DigitalSet dsMask( Domain(Point(0,0),Point(baseImage.cols-1,baseImage.rows-1)));
  DigitalSet dsContour( dsMask.domain() );
  cv::Mat contourMask = cv::Mat::zeros(baseImage.size(),CV_8UC1);

  DIPaCUS::Representation::CVMatToDigitalSet(dsMask,mask,1);
  DIPaCUS::Misc::digitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(dsContour,dsMask,2);
  DIPaCUS::Representation::digitalSetToCVMat(contourMask,dsContour);

  outputImage.setTo(color,contourMask);

}

int main(int argc,char* argv[]){
  std::string baseImagePath=argv[1];
  std::string maskPath=argv[2];
  std::string outputFilepath=argv[3];
  int c1=std::atoi(argv[4]);
  int c2=std::atoi(argv[5]);
  int c3=std::atoi(argv[6]);

  cv::Mat contour;

  cv::Mat baseImage = cv::imread(baseImagePath);
  cv::Mat maskImage = cv::imread(maskPath,cv::IMREAD_GRAYSCALE);

  setHighlightMask(contour,baseImage,maskImage,cv::Vec3b(c1,c2,c3));
  cv::imwrite( outputFilepath,contour);

  return 0;
}