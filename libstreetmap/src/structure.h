#ifndef MYSTRUCTURE_H
#define MYSTRUCTURE_H

#include "StreetsDatabaseAPI.h"
#include "m2_helper.h"
#include "graphics.h"
#include "OSMDatabaseAPI.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <string>
#include <iostream>
#include <unordered_set>
#include <cassert>
#include <utility>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/foreach.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

//define point type in r_trees
typedef bg::model::point<double, 2, bg::cs::cartesian> point;

//define POI_r_tree
//POI index -> POI position(lat, lon)
typedef std::pair<point, POIIndex> POI_position;

//define intersection_r_tree
//intersection index -> intersection position (lat, lon)
typedef std::pair<point, IntersectionIndex> intersection_position;

typedef bg::model::point<float, 2, bg::cs::cartesian> point_float;

typedef std::pair<point_float, IntersectionIndex> depot_position;

typedef std::pair<point_float, IntersectionIndex> node_position;


struct delivery_node {
    
    unsigned nodeID;

    delivery_node* pick_up;

    //if this node is a drop off point -> pick_up = its pick up point node
    //else if the node is a pick up point -> its pick up is NULL
};

struct valid_dropoff {
    
    unsigned nodeID;

    point_float node_position;

};

struct intersection_edge_info {
    unsigned streetOfSeg;

    unsigned connected_intersection;

    unsigned edge_index;

    float travlle_time_to_next_intersection;

    float segment_length_to_next_intersection;
};

struct swap_node{
    
    unsigned start;
    
    unsigned destinataion;
    
    float time_cost;
};

class streetGraph {
public:

    //2d vector: intersection index -> surrounding segment index vector
    std::vector<std::vector<StreetSegmentIndex> > intersection_street_segments;

    //2d vector: intersection index -> surrounding street names vector
    std::vector<std::vector<std::string> > intersection_street_segment_names;

    //map: intersection index -> adjacent intersection index vector
    std::unordered_map<IntersectionIndex, std::vector<IntersectionIndex> > adjacent_intersections;

    //2d vector: street index -> street segments index vector
    std::vector<std::unordered_set<StreetSegmentIndex> > street_street_segments;

    //2d:vector: street index -> all intersection index on this street vector
    std::vector<std::vector<IntersectionIndex> > all_street_intersections;

    //define constant data and load them from API
    //store them in container so don't need to get them form API every time use them
    unsigned number_of_streets;

    unsigned number_of_segments;

    unsigned number_of_intersections;

    unsigned number_of_interest_point;

    unsigned number_of_feature;

    unsigned number_of_nodes;

    unsigned numebr_of_ways;

    unsigned number_of_relations;

    //POI_r_tree
    bgi::rtree<POI_position, bgi::quadratic<16> > POI_rtree;

    //intersection_r_tree
    bgi::rtree<intersection_position, bgi::quadratic<16> > intersection_rtree;

    //build function: load data from API into containers
    void build();

    //member function to get length between two point without curve points
    double find_length(LatLon point1, LatLon point2);

    double avgLat;

    std::vector<t_point> intersection_position_vector;

    std::vector<t_point> POI_position_vector;

    t_bound_box boundary_box;

    std::vector<std::vector<t_point> > feature_point_vector_vector;

    std::vector<std::string> street_name_vector;

    std::vector<std::string> POI_names;

    std::vector<POI_info> POI_info_vector;

    std::vector<unsigned> POI_intersection;

    std::vector<std::string> segment_street_name;
    
    std::vector<IntersectionIndex> all_intersection;
   
private:


};


#endif /* MYSTRUCTURE_H */

