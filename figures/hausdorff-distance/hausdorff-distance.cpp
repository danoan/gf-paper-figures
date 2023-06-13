#include "DIPaCUS/base/Shapes.h"
#include "DIPaCUS/base/Representation.h"
#include "DIPaCUS/derivates/Misc.h"
#include "DIPaCUS/components/Neighborhood.h"

#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/boards/Board2D.h"

#include <cmath>
#include <ostream>
#include <limits>
#include <vector>
#include <functional>

using namespace DGtal::Z2i;
using namespace std;

/* using VDigitalSet = DGtal::DigitalSetBySTLVector<Domain>; */
using VDigitalSet = DigitalSet;

struct Distance{
    int point_index_1;
    int point_index_2;
    double distance;
};

Distance hausdorff_distance(const VDigitalSet& s1, const VDigitalSet& s2){
    Distance hd{0,0,numeric_limits<double>::min()};

    size_t i=0;
    for(auto p1:s1){
        double x = std::numeric_limits<double>::max();
    
        size_t j=0;
        size_t best_j=0;
        for(auto p2:s2){
            auto d = sqrt((p1-p2).squaredNorm());

            if(d<x){
                x = d;
                best_j=j;
            }
            j++;
        }

        if( x > hd.distance ){
            hd.point_index_1 = i;
            hd.point_index_2 = best_j;
            hd.distance = x;
        }
    }

    return hd;
}

Point lowest_merge(const Point& p1, const Point& p2){
    Point p;

    for(size_t i=0;i<p1.size();i++){
        p[i] = p1[i] < p2[i] ? p1[i] : p2[i];
    }
   
    return p;
}

Point highest_merge(const Point& p1, const Point& p2){
    Point p;

    for(size_t i=0;i<p1.size();i++){
        p[i] = p1[i] > p2[i] ? p1[i] : p2[i];
    }
   
    return p;
}

template<class TDigitalSet>
Point get_center_point(const TDigitalSet& _d){
    Point d_center_point{0,0};
    for(auto p:_d) d_center_point+=p;
    
    for(size_t i=0;i<d_center_point.size();i++){
        d_center_point[i]= (int) d_center_point[i] / ( (int)_d.size() );
    }

    return d_center_point;
}

template<class TDigitalSet>
VDigitalSet center_digital_set(Point center_point, const TDigitalSet& _d){
    Point d_center_point=get_center_point(_d);
    Point t = center_point - d_center_point;

    VDigitalSet d(_d.domain());
    for(auto p:_d) d.insert(p+t);

    return d;
}

Distance dissimilarity_distance(const VDigitalSet& s1, const VDigitalSet& s2){
    DigitalSet b1(s1.domain());
    DIPaCUS::Misc::digitalBoundary<DIPaCUS::Neighborhood::EightNeighborhoodPredicate>(b1,s1);
    
    DigitalSet b2(s2.domain());
    DIPaCUS::Misc::digitalBoundary<DIPaCUS::Neighborhood::EightNeighborhoodPredicate>(b2,s2);

    Distance s1_s2_hd = hausdorff_distance(b1,b2);
    Distance s2_s1_hd = hausdorff_distance(b2,b1);

    if(s1_s2_hd.distance > s2_s1_hd.distance ) return s1_s2_hd;
    else return Distance{s2_s1_hd.point_index_2,s2_s1_hd.point_index_1,s2_s1_hd.distance};
}

Point get_point_from_index(const VDigitalSet& v, size_t point_index){
    size_t i = 0;
    for(auto p:v){
        if(i++==point_index) return p;
    }
    
    throw runtime_error("Index: " + to_string(point_index) + "not valid");
    return Point(0,0);
}

void display_dissimilarity(const DigitalSet& _s1, const DigitalSet& _s2, const double resolution){
    // First I need to find a common domain that fits both sets
    Point lb_s1,lb_s2,ub_s1,ub_s2;
    _s1.computeBoundingBox(lb_s1,ub_s1);
    _s2.computeBoundingBox(lb_s2,ub_s2);

    Point _lb = lowest_merge(lb_s1,lb_s2);
    Point _ub = highest_merge(ub_s1,ub_s2);
    Domain _domain(_lb,_ub);

    VDigitalSet s1(_domain);
    s1.insert(_s1.begin(),_s1.end());
   
    // Then I center both sets around a single point
    VDigitalSet s2 = center_digital_set( get_center_point(s1), _s2);
   
    // The operation above induces a translation. Likely a new minimum
    // common domain exist between the sets. I compute it.
    s1.computeBoundingBox(lb_s1,ub_s1);
    s2.computeBoundingBox(lb_s2,ub_s2);
    
    Point lb = lowest_merge(lb_s1,lb_s2);
    Point ub = highest_merge(ub_s1,ub_s2);
    Domain domain(lb,ub);

    // Then I create the final DigitalSet with the minimum common domain 
    // between them.
    VDigitalSet d1(domain);
    d1.insert(s1.begin(),s1.end());
    VDigitalSet d2(domain);
    d2.insert(s2.begin(),s2.end());

    
    DGtal::Color cool_green( 190, 216, 153, 128 ); 
    DGtal::Color cool_red( 216, 153, 190, 128 ); 
    DGtal::Color black( 0, 0, 0, 128 ); 
    
    /* DGtal::Board2D board; */
    /* board << domain */ 
    /*       << DGtal::CustomStyle( s1.className(), new DGtal::CustomColors( black,cool_green ) ) */
    /*       << d1 */
    /*       << DGtal::CustomStyle( s2.className(), new DGtal::CustomColors( black,cool_red ) ) */
    /*       << d2; */
    /* board.saveSVG("output.svg"); */

    auto d = dissimilarity_distance(d1,d2);
    cout << get_point_from_index(s1,d.point_index_1) << ";" << get_point_from_index(s2,d.point_index_2) << ";" << resolution*d.distance << "\n";
}

void test(){
    using namespace DIPaCUS;

    DigitalSet _s1 = Shapes::ball();
    DigitalSet _s2 = Shapes::triangle();

    display_dissimilarity(_s1,_s2, 0.25);
}

int main(int argc, char* argv[]){
    using namespace DIPaCUS;
    typedef DGtal::ImageContainerBySTLVector < DGtal::Z2i::Domain, unsigned char> Image2D;

    if(argc < 2){
        cout << "You need to enter the image path to compare with the disk of radius 8.\n";
        exit(0);
    }

    double resolution=0.125;
    if(argc==3){
        resolution = atof(argv[2]);
    }

    string imagePath = argv[1];

    typedef DGtal::DigitalSetInserter<DigitalSet> DigitalSetInserter;
    Image2D image = DGtal::GenericReader<Image2D>::import(imagePath);

    DigitalSet img_d(image.domain());
    DigitalSetInserter inserter(img_d);
    DGtal::setFromImage(image, inserter, 128, 255);

    DigitalSet _s1 = Shapes::ball(resolution,0,0,32);
    display_dissimilarity(_s1,img_d, resolution);
    

    return 0;
}
