#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <string>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <math.h>
#include "Surface.h"
#include "easygl/easygl_constants.h"
#include "fontcache.h"
#include "graphics.h"
#include "graphics_automark.h"
#include "graphics_state.h"
#include "graphics_types.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m2_helper.h"
#include "structure.h"
#include <X11/keysym.h>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/foreach.hpp>

////////////////////////////////////////////////
//          global variables                  //
////////////////////////////////////////////////

std::string this_city_name = "Toronto";

std::string this_map_path;

std::string osm_path;

std::string test;

std::string test2;

float box_height;

std::vector<std::string> possibile_input;

std::vector<std::string> possibile_input_temp;

bool search_bar_flag = false;

bool search_bar_flag2 = false;

bool print_path_termainal = false;

unsigned path_start, path_end;

extern std::vector<double> segment_length_vector;

//stores input string when search intersectioon
std::vector<string> string_input;

//stores input string when search street
std::string find_this_street;

//map key(street name) -> element(street ids with the key name)
extern std::unordered_map<std::string, std::vector<StreetIndex> > streetID_from_name;

extern streetGraph street_graph;

//segment index -> from & to & curve points position in xy
extern std::vector< std::vector<t_point> > segment_segment_position_vector;

//stores segments with speed limit > 80
extern std::vector<StreetSegmentIndex> highway_segment;

//stores segments with speed limit > 50 & < 80
extern std::vector<StreetSegmentIndex> main_street_segment;

//stores segments with speed limit < 50
extern std::vector<StreetSegmentIndex> loacal_street_segment;

//bounday of current city map
extern std::vector<double> boundary;

//stores all matched intersection position (find button)
std::vector<t_point> matched_intersection_position;

//stores all matched intersection position (street button)
std::vector<unsigned> matched_street_number;

std::vector<std::vector<unsigned> > path_segment_intersection;

std::vector<std::vector<unsigned> > path_segment_POI;

std::vector<unsigned> path_segment_click;

//stores all closed feature area
std::set<float> feature_area_set;

//map: key(feature id) -> element(this feature area)
std::map<unsigned, float> feature_area_map;

//stores all closed feature area
std::vector<float> feature_area_vector;

//OSMID -> OSMNode pointer
std::unordered_map<OSMID, const OSMNode*> OSMID_OSMNode;

//OSMID -> OSMWay pointer
std::unordered_map<OSMID, const OSMWay*> OSMID_OSMWay;

//OSMID -> OSMNRelation pointer
std::unordered_map<OSMID, const OSMRelation*> OSMID_OSMRelation;

//OSMID -> OSMEntity pointer
std::unordered_map<OSMID, const OSMEntity*> OSMID_OSMEntity;

//stores all OSMNode pointer
std::vector<const OSMNode*> OSMNode_vector;

//stores all OSMWay pointer
std::vector<const OSMWay*> OSMWay_vector;

//stores all OSMRelation pointer
std::vector<const OSMRelation*> OSMRelation_vector;

//map key(OSMEntity pointer) -> element (OSMEntity point tags)
std::unordered_map<const OSMEntity*, std::unordered_map<std::string, std::string> > OSMEntity_Point_Tags;

//store all bus station in order
std::vector<t_point> bus_station_location;

//store all bus station names in order
std::vector<std::string> bus_station_name;

//store all subway station in order
std::vector<t_point> subway_station_location;

//store all subway station names in order
std::vector<std::string> subway_station_name;

//store all ferry terminal in order
std::vector<t_point> ferry_station_location;

//store all ferry terminal names in order
std::vector<std::string> ferry_station_name;

//store all transport line
std::vector< std::vector<OSMID> > transport_line;

//store all subway line
std::vector< std::vector<OSMID> > subway_line;

//store all bus line
std::vector< std::vector<OSMID> > bus_line;

//store all ferry line
std::vector< std::vector<OSMID> > ferry_line;

//current zoom level
unsigned zoom_level;

double initial_height;
double initial_width;

double current_height;
double current_width;

//flag: first draw a map, avoid load same map information
unsigned first_draw_map = 1;

//store feature info in different vectors (based on feature type)
std::vector<feature_info> park_feature;
std::vector<feature_info> beach_feature;
std::vector<feature_info> lake_feature;
std::vector<feature_info> island_feature;
std::vector<feature_info> shoreline_feature;
std::vector<feature_info> river_feature;
std::vector<feature_info> buliding_feature;
std::vector<feature_info> golfcourse_feature;
std::vector<feature_info> greenspace_feature;
std::vector<feature_info> stream_feature;
std::vector<feature_info> unknown_feature;
std::vector<feature_info> all_feature_info;

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 2, bg::cs::cartesian> t_point_point;

//define bus_station_r_tree element
typedef std::pair<t_point_point, unsigned> bus_station_position;

//define subway_station_r_tree element
typedef std::pair<t_point_point, unsigned> subway_station_position;

//define bus_station_r_tree
bgi::rtree<bus_station_position, bgi::quadratic<16> > bus_station_rtree;

//define subway_station_r_tree
bgi::rtree<subway_station_position, bgi::quadratic<16> > subway_station_rtree;

////////////////////////////////////////
//    Define some constant numbers    //
////////////////////////////////////////

const int zoom_level_1 = 1;
const int zoom_level_2 = 3;
const int zoom_level_3 = 6;
const int zoom_level_4 = 10;
const int zoom_level_5 = 50;
const int zoom_level_6 = 80;
const int zoom_level_7 = 140;
const int zoom_level_8 = 220;
const int zoom_level_stop = 830;

const t_color highway_color = t_color(254, 216, 157);
const t_color street_color = t_color(255, 255, 255);
const t_color building_color = t_color(241, 241, 241);
const t_color water_color = t_color(163, 204, 255);
const t_color shoreline_color = t_color(255, 255, 255);
const t_color park_color = t_color(203, 230, 163);
const t_color beach_color = t_color(YELLOW);

const float check_feature_area = 1 / 200;

const int highway_speed = 80;

const float left_offset2_const = 0.045;
const float right_offset2_const = 0.01;
const float top_offset2_const = 0.035;
const float bottom_offset2_const = 0.89;

const float left_offset1_const = 0.05;
const float right_offset1_const = 0.6;
const float top_offset1_const = 0.04;
const float bottom_offset1_const = 0.91;

const float left_offset2_const2 = 0.045;
const float right_offset2_const2 = 0.59;
const float top_offset2_const2 = 0.035;
const float bottom_offset2_const2 = 0.9;

void drawscreen(void) {

    clearscreen();

    t_bound_box initial_coords = t_bound_box(boundary[3], boundary[2], boundary[1], boundary[0]);

    initial_height = initial_coords.get_height();
    initial_width = initial_coords.get_width();

    current_height = get_visible_world().get_height();
    current_width = get_visible_world().get_width();

    if (initial_height > initial_width)
        zoom_level = find_zoom_level_by_height();
    else
        zoom_level = find_zoom_level_by_width();

    if (first_draw_map == 1) {
        initialize_feature_id_vector();
        initialize_feature_area_map();
        initialize_OSMNode_Map();
        initialize_OSMWay_Map();
        initialize_OSMRelation_Map();
        initialize_station_info();
        initialize_station_tree();
        initialize_transport_lines();
        first_draw_map = 0;
    }

    //std::cout << zoom_level << std::endl;

    if (zoom_level < zoom_level_1) {

        zoom_level_1_draw();


    } else if (zoom_level <= zoom_level_2) {
        zoom_level_2_draw();

    } else if (zoom_level <= zoom_level_3) {
        zoom_level_3_draw();

    } else if (zoom_level <= zoom_level_4) {
        zoom_level_4_draw();

    } else if (zoom_level < zoom_level_5) {
        zoom_level_5_draw();

    } else if (zoom_level < zoom_level_6) {
        zoom_level_6_draw();


    } else if (zoom_level < zoom_level_7) {
        zoom_level_7_draw();

    } else if (zoom_level >= zoom_level_7) {
        zoom_level_8_draw();
    }

    highlight_matched_intersection();

    highlight_matched_street();

    highlight_path_intersection();

    highlight_path_POI();

    highlight_path_click();

    hightlight_path_terminal();

    draw_ferry_staions();

    draw_search_bar(search_bar_flag);

    draw_search_bar2(search_bar_flag2);

    copy_off_screen_buffer_to_screen();
}

////////////////////////////////////////////////////////////////
//     Draw different layers based on current zoom level      //
////////////////////////////////////////////////////////////////

void zoom_level_1_draw(void) {

    draw_natural_feature();
    draw_main_street();
    draw_highway();

    draw_feature_name(park_feature);
    draw_feature_name(lake_feature);
    draw_feature_name(greenspace_feature);
    draw_feature_name(golfcourse_feature);

    draw_transport_lines("subway");
    draw_transport_lines("ferry");

    draw_city_name();
}

void zoom_level_2_draw(void) {

    draw_natural_feature();
    draw_main_street();
    draw_highway();

    draw_feature_name(park_feature);
    draw_feature_name(island_feature);
    draw_feature_name(greenspace_feature);
    draw_feature_name(golfcourse_feature);

    draw_transport_lines("subway");
    draw_transport_lines("ferry");

    draw_city_name();

}

void zoom_level_3_draw(void) {

    draw_natural_feature();
    draw_main_street();
    draw_all_street_segment();
    draw_highway();

    draw_transport_lines("subway");
    draw_transport_lines("ferry");

    draw_feature_name(park_feature);
    draw_feature_name(island_feature);
    draw_feature_name(greenspace_feature);
    draw_feature_name(golfcourse_feature);
}

void zoom_level_4_draw(void) {

    draw_natural_feature();
    draw_main_street();
    draw_all_street_segment();
    draw_highway();

    draw_transport_lines("subway");
    draw_transport_lines("ferry");

    draw_feature_name(park_feature);
    draw_feature_name(island_feature);
    draw_feature_name(greenspace_feature);
    draw_feature_name(golfcourse_feature);
}

void zoom_level_5_draw(void) {

    draw_feature();
    draw_main_street();
    draw_all_street_segment();

    draw_transport_lines("ferry");

    draw_feature_name(park_feature);
    draw_feature_name(island_feature);
    draw_feature_name(greenspace_feature);
    draw_feature_name(golfcourse_feature);
    draw_feature_name(buliding_feature);

    draw_street_names();
}

void zoom_level_6_draw(void) {

    draw_feature();
    draw_main_street();
    draw_all_street_segment();

    draw_transport_lines("ferry");

    draw_feature_name(park_feature);
    draw_feature_name(island_feature);
    draw_feature_name(greenspace_feature);
    draw_feature_name(golfcourse_feature);
    draw_feature_name(buliding_feature);

    draw_street_names();
}

void zoom_level_7_draw(void) {

    draw_feature();
    draw_main_street();
    draw_all_street_segment();

    draw_transport_lines("ferry");

    draw_feature_name(park_feature);
    draw_feature_name(island_feature);
    draw_feature_name(greenspace_feature);
    draw_feature_name(golfcourse_feature);
    draw_feature_name(buliding_feature);

    draw_street_names();
}

void zoom_level_8_draw(void) {

    draw_feature();
    draw_main_street();
    draw_all_street_segment();

    draw_transport_lines("ferry");

    draw_feature_name(park_feature);
    draw_feature_name(island_feature);
    draw_feature_name(greenspace_feature);
    draw_feature_name(golfcourse_feature);
    draw_feature_name(buliding_feature);

    draw_street_names();
}

unsigned find_zoom_level_by_height() {

    unsigned get_zoom_level = initial_height / current_height;

    return get_zoom_level;

}

unsigned find_zoom_level_by_width() {

    unsigned get_zoom_level = initial_width / current_width;

    return get_zoom_level;

}

//draw all street(segment)

void draw_all_street_segment(void) {

    set_draw_mode(DRAW_NORMAL);
    setlinestyle(SOLID, ROUND);
    setlinewidth(1);
    setcolor(street_color);

    if (zoom_level > zoom_level_4)
        setlinewidth(5);

    if (zoom_level >= zoom_level_7)
        setlinewidth(15);

    if (zoom_level >= zoom_level_8)
        setlinewidth(40);

    for (unsigned i = 0; i < street_graph.number_of_segments; i++) {

        draw_segment(i);
    }
}

//draw all features

void draw_feature(void) {

    set_draw_mode(DRAW_NORMAL);
    setlinestyle(SOLID, ROUND);
    setlinewidth(1);

    for (unsigned i = 0; i < street_graph.number_of_feature; i++) {

        FeatureType feature_type = getFeatureType(i);

        unsigned count = getFeaturePointCount(i);

        //stores all points of a feature
        std::vector<t_point> feature_point_vector;

        //pointer -> all points of a feature
        t_point* this_feature_ptr;

        for (unsigned j = 0; j < count; j++) {

            LatLon feature_point = getFeaturePoint(i, j);

            t_point feature_t_point = LatLon_XY_converter(feature_point);


            feature_point_vector.push_back(feature_t_point);
        }

        this_feature_ptr = new t_point[feature_point_vector.size()];

        for (unsigned k = 0; k < feature_point_vector.size(); k++) {

            this_feature_ptr[k] = feature_point_vector[k];
        }

        unsigned size = feature_point_vector.size();

        t_point check_start = feature_point_vector[0];

        t_point check_end = feature_point_vector[size - 1];


        //check if this feature is enclosed (start point = end point)
        //enclosed -> fill poly
        //open -> draw line
        if (check_start.x == check_end.x && check_start.y == check_end.y) {

            switch (getFeatureType(i)) {
                case Park:
                    setcolor(park_color);
                    break;

                case Beach:
                    setcolor(beach_color);
                    break;

                case Lake:
                    setcolor(water_color);
                    break;

                case River:
                    setcolor(water_color);
                    break;

                case Island:
                    setcolor(park_color);
                    break;

                case Shoreline:
                    setcolor(shoreline_color);
                    break;

                case Building:
                    setcolor(building_color);
                    break;

                case Greenspace:
                    setcolor(park_color);
                    break;

                case Golfcourse:
                    setcolor(park_color);
                    break;

                case Stream:
                    setcolor(water_color);
                    break;

                case Unknown:
                    setcolor(RED);
            }


            fillpoly(this_feature_ptr, count);

        } else {

            switch (getFeatureType(i)) {
                case Park:
                    setcolor(park_color);
                    break;

                case Beach:
                    setcolor(beach_color);
                    break;

                case Lake:
                    setcolor(water_color);
                    break;

                case River:
                    setcolor(water_color);
                    break;

                case Island:
                    setcolor(park_color);
                    break;

                case Shoreline:
                    setcolor(shoreline_color);
                    break;

                case Building:
                    setcolor(building_color);
                    break;

                case Greenspace:
                    setcolor(park_color);
                    break;

                case Golfcourse:
                    setcolor(park_color);
                    break;

                case Stream:
                    setcolor(water_color);
                    break;

                case Unknown:
                    setcolor(RED);
            }

            for (unsigned b = 1; b < feature_point_vector.size(); b++) {
                drawline(feature_point_vector[b], feature_point_vector[b - 1]);
            }
        }

        //free allocated memory
        delete this_feature_ptr;
    }
}

//Converter: LatLon -> XY(t_point)

t_point LatLon_XY_converter(LatLon input) {

    float input_lat = input.lat();
    float input_lon = input.lon();

    float input_x = (input_lon * cos(street_graph.avgLat * DEG_TO_RAD));
    float input_y = input_lat;

    t_point X_Y = t_point(input_x, input_y);

    return X_Y;
}

//Converter: XY(t_point) -> LatLon

LatLon XY_LatLon_Converter(t_point input) {

    float input_x = input.x;
    float input_y = input.y;

    float input_lon = input_x / (cos(street_graph.avgLat * DEG_TO_RAD));
    float input_lat = input_y;

    LatLon input_latlon = LatLon(input_lat, input_lon);

    return input_latlon;
}

//draw highways (segment)

void draw_highway(void) {

    setcolor(highway_color);
    set_draw_mode(DRAW_NORMAL);
    setlinestyle(SOLID, ROUND);
    setlinewidth(2);

    for (unsigned i = 0; i < highway_segment.size(); i++) {

        unsigned segment_id = highway_segment[i];

        draw_segment(segment_id);
    }


}

//draw main streets (segment)

void draw_main_street(void) {

    setcolor(street_color);
    set_draw_mode(DRAW_NORMAL);
    setlinestyle(SOLID, ROUND);
    setlinewidth(1);

    if (zoom_level >= zoom_level_6)
        setlinewidth(2);
    else if (zoom_level >= (zoom_level_5 - zoom_level_4))
        setlinewidth(3);


    for (unsigned i = 0; i < main_street_segment.size(); i++) {

        unsigned segment_id = main_street_segment[i];

        draw_segment(segment_id);

    }
}

//take one segment id as input, draw this segment

void draw_segment(unsigned segment_id) {

    std::string arrow = "-->";

    StreetSegmentInfo segment_info = getStreetSegmentInfo(segment_id);

    LatLon start_point = getIntersectionPosition(segment_info.from);
    LatLon end_point = getIntersectionPosition(segment_info.to);

    float start_lon = start_point.lon();
    float start_lat = start_point.lat();

    float start_x = (start_lon * cos(street_graph.avgLat * DEG_TO_RAD));
    float start_y = start_lat;

    float end_lon = end_point.lon();
    float end_lat = end_point.lat();

    float end_x = (end_lon * cos(street_graph.avgLat * DEG_TO_RAD));
    float end_y = end_lat;

    t_point start_t_point = t_point(start_x, start_y);

    t_point end_t_point = t_point(end_x, end_y);

    unsigned curve_point = segment_info.curvePointCount;

    for (unsigned j = 0; j <= curve_point; j++) {

        if (curve_point == 0) {

            if (segment_info.speedLimit >= highway_speed)
                setcolor(highway_color);
            else
                setcolor(street_color);

            drawline(start_x, start_y, end_x, end_y);

            if (segment_info.oneWay && zoom_level >= zoom_level_6) {

                float angle = atan2((start_y - end_y), (start_x - end_x)) / DEG_TO_RAD;

                settextrotation(angle);

                settextrotation(angle);

                setcolor(25, 25, 112);

                setfontsize(15);

                drawtext(0.5 * (start_x + end_x), 0.5 * ((start_y + end_y)), arrow, fabs(start_x - end_x), fabs(start_y - end_y));
            }


        } else if (curve_point == 1) {

            LatLon curve_point_position = getStreetSegmentCurvePoint(segment_id, 0);

            float curve_point_x = (curve_point_position.lon() * cos(street_graph.avgLat * DEG_TO_RAD));

            float curve_point_y = curve_point_position.lat();

            t_point curve_t_point = t_point(curve_point_x, curve_point_y);

            if (segment_info.speedLimit >= highway_speed)
                setcolor(highway_color);
            else
                setcolor(street_color);

            drawline(start_t_point, curve_t_point);

            if (segment_info.oneWay && zoom_level >= zoom_level_6) {

                float angle = atan2((start_t_point.y - curve_t_point.y), (start_t_point.x - curve_t_point.x)) / DEG_TO_RAD;

                settextrotation(angle);

                setcolor(25, 25, 112);

                setfontsize(15);

                drawtext(0.5 * (start_t_point.x + curve_t_point.x), 0.5 * ((start_t_point.y + curve_t_point.y)), arrow, fabs(start_t_point.y - curve_t_point.x), fabs(start_t_point.y - curve_t_point.y));

            }

            if (segment_info.speedLimit >= highway_speed)
                setcolor(highway_color);
            else
                setcolor(street_color);

            drawline(end_t_point, curve_t_point);

            if (segment_info.oneWay && zoom_level >= zoom_level_6) {

                float angle = atan2((end_t_point.y - curve_t_point.y), (end_t_point.x - curve_t_point.x)) / DEG_TO_RAD;

                settextrotation(angle);

                settextrotation(angle);

                setcolor(25, 25, 112);

                setfontsize(15);

                drawtext(0.5 * (end_t_point.x + curve_t_point.x), 0.5 * ((end_t_point.y + curve_t_point.y)), arrow, fabs(end_t_point.y - curve_t_point.x), fabs(end_t_point.y - curve_t_point.y));

            }

        } else {

            for (unsigned k = 1; k < (curve_point); k++) {

                LatLon curve_point_position1 = getStreetSegmentCurvePoint(segment_id, k - 1);

                LatLon curve_point_position2 = getStreetSegmentCurvePoint(segment_id, k);

                float curve_point_x1 = (curve_point_position1.lon() * cos(street_graph.avgLat * DEG_TO_RAD));

                float curve_point_y1 = curve_point_position1.lat();

                t_point curve_t_point1 = t_point(curve_point_x1, curve_point_y1);

                float curve_point_x2 = (curve_point_position2.lon() * cos(street_graph.avgLat * DEG_TO_RAD));

                float curve_point_y2 = curve_point_position2.lat();

                t_point curve_t_point2 = t_point(curve_point_x2, curve_point_y2);

                if (segment_info.speedLimit >= highway_speed)
                    setcolor(highway_color);
                else
                    setcolor(street_color);

                drawline(curve_t_point1, curve_t_point2);

                if (segment_info.oneWay && zoom_level >= zoom_level_6) {

                    float angle = atan2((curve_t_point1.y - curve_t_point2.y), (curve_t_point1.x - curve_t_point2.x)) / DEG_TO_RAD;

                    settextrotation(angle);

                    settextrotation(angle);

                    setcolor(25, 25, 112);

                    setfontsize(15);

                    drawtext(0.5 * (curve_t_point1.x + curve_t_point2.x), 0.5 * ((curve_t_point1.y + curve_t_point2.y)), arrow, fabs(curve_t_point1.y - curve_t_point2.x), fabs(curve_t_point1.y - curve_t_point2.y));

                }


                if (k == 1) {

                    if (segment_info.speedLimit >= highway_speed)
                        setcolor(highway_color);
                    else
                        setcolor(street_color);

                    drawline(start_t_point, curve_t_point1);

                    if (segment_info.oneWay && zoom_level >= zoom_level_6) {

                        float angle = atan2((start_t_point.y - curve_t_point1.y), (start_t_point.x - curve_t_point1.x)) / DEG_TO_RAD;

                        settextrotation(angle);

                        setcolor(25, 25, 112);

                        setfontsize(15);

                        drawtext(0.5 * (start_t_point.x + curve_t_point1.x), 0.5 * ((start_t_point.y + curve_t_point1.y)), arrow, fabs(start_t_point.y - curve_t_point1.x), fabs(start_t_point.y - curve_t_point1.y));

                    }

                }
                if (k == (curve_point - 1)) {

                    if (segment_info.speedLimit >= highway_speed)
                        setcolor(highway_color);
                    else
                        setcolor(street_color);

                    drawline(end_t_point, curve_t_point2);


                    if (segment_info.oneWay && zoom_level >= zoom_level_6) {

                        float angle = atan2((curve_t_point2.y - end_t_point.y), (curve_t_point2.x - end_t_point.x)) / DEG_TO_RAD;

                        settextrotation(angle);

                        settextrotation(angle);

                        setcolor(25, 25, 112);

                        setfontsize(15);

                        drawtext(0.5 * (curve_t_point2.x + end_t_point.x), 0.5 * ((curve_t_point2.y + end_t_point.y)), arrow, fabs(curve_t_point2.y - end_t_point.x), fabs(curve_t_point2.y - end_t_point.y));

                    }

                }
            }
        }
    }
}

//draw all nature feature

void draw_natural_feature(void) {

    set_draw_mode(DRAW_NORMAL);
    setlinestyle(SOLID, ROUND);
    setlinewidth(1);

    for (unsigned i = 0; i < street_graph.number_of_feature; i++) {

        FeatureType feature_type = getFeatureType(i);

        unsigned count = getFeaturePointCount(i);

        std::vector<t_point> feature_point_vector;

        t_point* this_feature_ptr;

        for (unsigned j = 0; j < count; j++) {

            LatLon feature_point = getFeaturePoint(i, j);

            t_point feature_t_point = LatLon_XY_converter(feature_point);

            feature_point_vector.push_back(feature_t_point);

        }

        this_feature_ptr = new t_point[feature_point_vector.size()];

        for (unsigned k = 0; k < feature_point_vector.size(); k++) {

            this_feature_ptr[k] = feature_point_vector[k];

        }

        unsigned size = feature_point_vector.size();

        t_point check_start = feature_point_vector[0];

        t_point check_end = feature_point_vector[size - 1];


        if (check_start.x == check_end.x && check_start.y == check_end.y) {

            switch (getFeatureType(i)) {
                case Park:
                    setcolor(park_color);
                    fillpoly(this_feature_ptr, count);
                    break;

                case Beach:
                    setcolor(beach_color);
                    fillpoly(this_feature_ptr, count);
                    break;

                case Lake:
                    setcolor(water_color);
                    fillpoly(this_feature_ptr, count);
                    break;

                case River:
                    setcolor(water_color);
                    fillpoly(this_feature_ptr, count);
                    break;

                case Island:
                    setcolor(park_color);
                    fillpoly(this_feature_ptr, count);
                    break;

                case Shoreline:
                    setcolor(shoreline_color);
                    fillpoly(this_feature_ptr, count);
                    break;

                case Building:
                    break;

                case Greenspace:
                    setcolor(park_color);
                    fillpoly(this_feature_ptr, count);
                    break;

                case Golfcourse:
                    setcolor(park_color);
                    fillpoly(this_feature_ptr, count);
                    break;

                case Stream:
                    setcolor(water_color);
                    fillpoly(this_feature_ptr, count);
                    break;

                case Unknown:
                    setcolor(RED);
            }


        } else {

            switch (getFeatureType(i)) {
                case Park:
                    setcolor(park_color);
                    for (unsigned b = 1; b < feature_point_vector.size(); b++) {
                        drawline(feature_point_vector[b], feature_point_vector[b - 1]);
                    }
                    break;

                case Beach:
                    setcolor(beach_color);
                    for (unsigned b = 1; b < feature_point_vector.size(); b++) {
                        drawline(feature_point_vector[b], feature_point_vector[b - 1]);
                    }
                    break;

                case Lake:
                    setcolor(water_color);
                    for (unsigned b = 1; b < feature_point_vector.size(); b++) {
                        drawline(feature_point_vector[b], feature_point_vector[b - 1]);
                    }
                    break;

                case River:
                    setcolor(water_color);
                    for (unsigned b = 1; b < feature_point_vector.size(); b++) {
                        drawline(feature_point_vector[b], feature_point_vector[b - 1]);
                    }
                    break;

                case Island:
                    setcolor(park_color);
                    for (unsigned b = 1; b < feature_point_vector.size(); b++) {
                        drawline(feature_point_vector[b], feature_point_vector[b - 1]);
                    }
                    break;

                case Shoreline:
                    setcolor(shoreline_color);
                    for (unsigned b = 1; b < feature_point_vector.size(); b++) {
                        drawline(feature_point_vector[b], feature_point_vector[b - 1]);
                    }
                    break;

                case Building:
                    break;

                case Greenspace:
                    setcolor(park_color);
                    for (unsigned b = 1; b < feature_point_vector.size(); b++) {
                        drawline(feature_point_vector[b], feature_point_vector[b - 1]);
                    }
                    break;

                case Golfcourse:
                    setcolor(park_color);
                    for (unsigned b = 1; b < feature_point_vector.size(); b++) {
                        drawline(feature_point_vector[b], feature_point_vector[b - 1]);
                    }
                    break;

                case Stream:
                    setcolor(water_color);
                    for (unsigned b = 1; b < feature_point_vector.size(); b++) {

                        drawline(feature_point_vector[b], feature_point_vector[b - 1]);
                    }
                    break;

                case Unknown:
                    setcolor(RED);
            }
        }

        delete this_feature_ptr;
    }
}

//initialize the map stores feature area

void initialize_feature_area_map() {

    for (unsigned i = 0; i < street_graph.number_of_feature; i++) {

        unsigned count = getFeaturePointCount(i);

        std::vector<t_point> feature_point_vector;

        for (unsigned j = 0; j < count; j++) {

            LatLon feature_point = getFeaturePoint(i, j);

            t_point feature_t_point = LatLon_XY_converter(feature_point);

            feature_point_vector.push_back(feature_t_point);
        }

        float area = get_poly_area(count, feature_point_vector);
        feature_area_map.insert(std::make_pair(area, i));
        feature_area_set.insert(area);
        feature_area_vector.push_back(area);
    }
}

//calculate a polygon area

float get_poly_area(unsigned numOfPoint, std::vector<t_point> point_vector) {

    float area = 0;
    int j = numOfPoint - 1;
    for (unsigned i = 0; i < numOfPoint; i++) {
        area = area + ((point_vector[j].x + point_vector[i].x) * (point_vector[j].y - point_vector[i].y));
        j = i;
    }
    area = area * 0.5;

    return (abs(area));

}

//calculate a line length

float get_line_lenght(unsigned numOfPoint, std::vector<t_point> point_vector) {

    float length = 0;

    for (unsigned i = 1; i < numOfPoint; i++) {

        t_point point1 = point_vector[i];
        t_point point2 = point_vector[i - 1];

        length += sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
    }
    return length;
}

//stores all feature information

void initialize_feature_id_vector() {

    for (unsigned i = 0; i < street_graph.number_of_feature; i++) {

        FeatureType feature_type = getFeatureType(i);

        unsigned count = getFeaturePointCount(i);

        std::vector<t_point> feature_point_vector;

        t_point* this_feature_ptr;

        for (unsigned j = 0; j < count; j++) {

            LatLon feature_point = getFeaturePoint(i, j);

            t_point feature_t_point = LatLon_XY_converter(feature_point);

            feature_point_vector.push_back(feature_t_point);

        }

        this_feature_ptr = new t_point[feature_point_vector.size()];

        for (unsigned k = 0; k < feature_point_vector.size(); k++) {

            this_feature_ptr[k] = feature_point_vector[k];

        }

        unsigned size = feature_point_vector.size();

        t_point check_start = feature_point_vector[0];

        t_point check_end = feature_point_vector[size - 1];

        switch (getFeatureType(i)) {
            case Park:
            {
                feature_info this_feature;
                this_feature.feature_index = i;
                this_feature.color = park_color;
                this_feature.feature_point = count;
                this_feature.feature_point_vector = feature_point_vector;
                this_feature.feature_type = Park;
                if (check_start.x == check_end.x && check_start.y == check_end.y) {
                    this_feature.area_length = get_poly_area(count, feature_point_vector);
                    this_feature.is_closed = true;

                } else {
                    this_feature.is_closed = false;
                    this_feature.area_length = get_line_lenght(count, feature_point_vector);
                }
                park_feature.push_back(this_feature);
                all_feature_info.push_back(this_feature);
                break;
            }
            case Beach:
            {
                feature_info this_feature;
                this_feature.feature_index = i;
                this_feature.color = beach_color;
                this_feature.feature_point = count;
                this_feature.feature_point_vector = feature_point_vector;
                this_feature.feature_type = Beach;
                if (check_start.x == check_end.x && check_start.y == check_end.y) {
                    this_feature.area_length = get_poly_area(count, feature_point_vector);
                    this_feature.is_closed = true;

                } else {
                    this_feature.is_closed = false;
                    this_feature.area_length = get_line_lenght(count, feature_point_vector);
                }
                beach_feature.push_back(this_feature);
                all_feature_info.push_back(this_feature);
                break;

            }
            case Lake:
            {
                feature_info this_feature;
                this_feature.feature_index = i;
                this_feature.color = water_color;
                this_feature.feature_point = count;
                this_feature.feature_point_vector = feature_point_vector;
                this_feature.feature_type = Lake;
                if (check_start.x == check_end.x && check_start.y == check_end.y) {
                    this_feature.area_length = get_poly_area(count, feature_point_vector);
                    this_feature.is_closed = true;

                } else {
                    this_feature.is_closed = false;
                    this_feature.area_length = get_line_lenght(count, feature_point_vector);
                }
                lake_feature.push_back(this_feature);
                all_feature_info.push_back(this_feature);
                break;

            }
            case River:
            {
                feature_info this_feature;
                this_feature.feature_index = i;
                this_feature.color = water_color;
                this_feature.feature_point = count;
                this_feature.feature_point_vector = feature_point_vector;
                this_feature.feature_type = River;
                if (check_start.x == check_end.x && check_start.y == check_end.y) {
                    this_feature.area_length = get_poly_area(count, feature_point_vector);
                    this_feature.is_closed = true;

                } else {
                    this_feature.is_closed = false;
                    this_feature.area_length = get_line_lenght(count, feature_point_vector);
                }
                park_feature.push_back(this_feature);
                all_feature_info.push_back(this_feature);
                break;
            }
            case Island:
            {
                feature_info this_feature;
                this_feature.feature_index = i;
                this_feature.color = park_color;
                this_feature.feature_point = count;
                this_feature.feature_point_vector = feature_point_vector;
                this_feature.feature_type = Island;
                if (check_start.x == check_end.x && check_start.y == check_end.y) {
                    this_feature.area_length = get_poly_area(count, feature_point_vector);
                    this_feature.is_closed = true;

                } else {
                    this_feature.is_closed = false;
                    this_feature.area_length = get_line_lenght(count, feature_point_vector);
                }
                island_feature.push_back(this_feature);
                all_feature_info.push_back(this_feature);
                break;
            }

            case Shoreline:
            {
                feature_info this_feature;
                this_feature.feature_index = i;
                this_feature.color = shoreline_color;
                this_feature.feature_point = count;
                this_feature.feature_point_vector = feature_point_vector;
                this_feature.feature_type = Shoreline;
                if (check_start.x == check_end.x && check_start.y == check_end.y) {
                    this_feature.area_length = get_poly_area(count, feature_point_vector);
                    this_feature.is_closed = true;

                } else {
                    this_feature.is_closed = false;
                    this_feature.area_length = get_line_lenght(count, feature_point_vector);
                }
                shoreline_feature.push_back(this_feature);
                all_feature_info.push_back(this_feature);
                break;

            }
            case Building:
            {
                feature_info this_feature;
                this_feature.feature_index = i;
                this_feature.color = building_color;
                this_feature.feature_point = count;
                this_feature.feature_point_vector = feature_point_vector;
                this_feature.feature_type = Building;
                if (check_start.x == check_end.x && check_start.y == check_end.y) {
                    this_feature.area_length = get_poly_area(count, feature_point_vector);
                    this_feature.is_closed = true;

                } else {
                    this_feature.is_closed = false;
                    this_feature.area_length = get_line_lenght(count, feature_point_vector);
                }
                buliding_feature.push_back(this_feature);
                all_feature_info.push_back(this_feature);
                break;
            }
            case Greenspace:
            {
                feature_info this_feature;
                this_feature.feature_index = i;
                this_feature.color = park_color;
                this_feature.feature_point = count;
                this_feature.feature_point_vector = feature_point_vector;
                this_feature.feature_type = Greenspace;
                if (check_start.x == check_end.x && check_start.y == check_end.y) {
                    this_feature.area_length = get_poly_area(count, feature_point_vector);
                    this_feature.is_closed = true;

                } else {
                    this_feature.is_closed = false;
                    this_feature.area_length = get_line_lenght(count, feature_point_vector);
                }
                greenspace_feature.push_back(this_feature);
                all_feature_info.push_back(this_feature);
                break;

            }
            case Golfcourse:
            {
                feature_info this_feature;
                this_feature.feature_index = i;
                this_feature.color = park_color;
                this_feature.feature_point = count;
                this_feature.feature_point_vector = feature_point_vector;
                this_feature.feature_type = Golfcourse;
                if (check_start.x == check_end.x && check_start.y == check_end.y) {
                    this_feature.area_length = get_poly_area(count, feature_point_vector);
                    this_feature.is_closed = true;

                } else {
                    this_feature.is_closed = false;
                    this_feature.area_length = get_line_lenght(count, feature_point_vector);
                }
                golfcourse_feature.push_back(this_feature);
                all_feature_info.push_back(this_feature);
                break;
            }
            case Stream:
            {
                feature_info this_feature;
                this_feature.feature_index = i;
                this_feature.color = water_color;
                this_feature.feature_point = count;
                this_feature.feature_point_vector = feature_point_vector;
                this_feature.feature_type = Stream;
                if (check_start.x == check_end.x && check_start.y == check_end.y) {
                    this_feature.area_length = get_poly_area(count, feature_point_vector);
                    this_feature.is_closed = true;

                } else {
                    this_feature.is_closed = false;
                    this_feature.area_length = get_line_lenght(count, feature_point_vector);
                }
                stream_feature.push_back(this_feature);
                all_feature_info.push_back(this_feature);
                break;
            }
            case Unknown:
            {
                feature_info this_feature;
                this_feature.feature_index = i;
                this_feature.color = building_color;
                this_feature.feature_point = count;
                this_feature.feature_point_vector = feature_point_vector;
                this_feature.feature_type = Unknown;
                if (check_start.x == check_end.x && check_start.y == check_end.y) {
                    this_feature.area_length = get_poly_area(count, feature_point_vector);
                    this_feature.is_closed = true;

                } else {
                    this_feature.is_closed = false;
                    this_feature.area_length = get_line_lenght(count, feature_point_vector);
                }
                unknown_feature.push_back(this_feature);
                all_feature_info.push_back(this_feature);
                break;
            }
        }
    }
}

//draw all features (of one type) stores in a vector

void draw_feature_vector(std::vector<feature_info> feature_vector) {

    for (unsigned i = 0; i < feature_vector.size(); i++) {

        if (feature_vector[i].is_closed) {

            setcolor(feature_vector[i].color);

            t_point* this_feature_ptr;

            this_feature_ptr = new t_point[feature_vector[i].feature_point_vector.size()];

            for (unsigned k = 0; k < feature_vector[i].feature_point_vector.size(); k++) {

                this_feature_ptr[k] = feature_vector[i].feature_point_vector[k];

            }

            fillpoly(this_feature_ptr, feature_vector[i].feature_point);

            delete this_feature_ptr;

        } else {
            setcolor(feature_vector[i].color);

            setlinewidth(2);

            for (unsigned b = 1; b < feature_vector[i].feature_point_vector.size(); b++) {

                drawline(feature_vector[i].feature_point_vector[b], feature_vector[i].feature_point_vector[b - 1]);
            }
        }
    }
}

//draw the feature's name
//if feature area > (1/200)*bounded city area -> print name

void draw_feature_name(std::vector<feature_info> feature_vector) {

    float feature_boundary_top;
    float feature_boundary_bottom;
    float feature_boundary_left;
    float feature_boundary_right;

    for (unsigned i = 0; i < feature_vector.size(); i++) {

        unsigned feature_index = feature_vector[i].feature_index;

        float feature_area = feature_area_vector[feature_index];

        if ((feature_area / get_visible_world().area()) > (check_feature_area)) {

            for (unsigned j = 0; j < feature_vector[i].feature_point_vector.size(); j++) {

                t_point check_xy = feature_vector[i].feature_point_vector[j];

                if (j == 0) {
                    feature_boundary_top = check_xy.y;
                    feature_boundary_bottom = check_xy.y;
                    feature_boundary_left = check_xy.x;
                    feature_boundary_right = check_xy.x;
                } else {

                    if (check_xy.y > feature_boundary_top)
                        feature_boundary_top = check_xy.y;

                    if (check_xy.y < feature_boundary_bottom)
                        feature_boundary_bottom = check_xy.y;

                    if (check_xy.x > feature_boundary_right)
                        feature_boundary_right = check_xy.x;

                    if (check_xy.x < feature_boundary_left)
                        feature_boundary_left = check_xy.x;
                }
            }

            float this_feature_center_x = feature_boundary_left + 0.5 * (feature_boundary_right - feature_boundary_left);
            float this_feature_center_y = feature_boundary_bottom + 0.5 * (feature_boundary_top - feature_boundary_bottom);

            t_point this_feature_center = t_point(this_feature_center_x, this_feature_center_y);

            t_bound_box this_feature_bound = t_bound_box(feature_boundary_left, feature_boundary_bottom, feature_boundary_right, feature_boundary_top);

            std::string this_feature_name = getFeatureName(feature_index);

            if (this_feature_name != "<noname>") {

                set_draw_mode(DRAW_NORMAL);
                setlinestyle(SOLID, ROUND);
                setlinewidth(1);
                setcolor(BLACK);
                setfontsize(10);
                settextrotation(0);
                drawtext(this_feature_center, this_feature_name, this_feature_bound);

            }
        }
    }
}

//print current city name

void draw_city_name() {

    t_bound_box this_city_boundary = t_bound_box(boundary[3], boundary[2], boundary[1], boundary[0]);

    float city_boundary_top = boundary[0];
    float city_boundary_bottom = boundary[2];
    float city_boundary_left = boundary[3];
    float city_boundary_right = boundary[1];

    float this_city_center_x = city_boundary_left + 0.5 * (city_boundary_right - city_boundary_left);
    float this_city_center_y = city_boundary_bottom + 0.5 * (city_boundary_top - city_boundary_bottom);

    t_point this_city_center = t_point(this_city_center_x, this_city_center_y);

    set_draw_mode(DRAW_NORMAL);
    setlinestyle(SOLID, ROUND);
    setlinewidth(1);
    setcolor(BLACK);
    setfontsize(40);
    settextrotation(0);
    drawtext(this_city_center, this_city_name, this_city_boundary);

}


//draw different POI (icon) based on its type

void draw_POI_icon_by_type() {

    for (unsigned i = 0; i < street_graph.number_of_interest_point; i++) {

        if (street_graph.POI_info_vector[i].POI_type == "hospital") {

            Surface hospital = load_png_from_file("hospital.png");
            draw_surface(hospital, street_graph.POI_info_vector[i].POI_position.x, street_graph.POI_info_vector[i].POI_position.y);
        } else if (street_graph.POI_info_vector[i].POI_type == "restaurant") {

            Surface restaurant = load_png_from_file("restaurant.png");
            draw_surface(restaurant, street_graph.POI_info_vector[i].POI_position.x, street_graph.POI_info_vector[i].POI_position.y);

        } else if (street_graph.POI_info_vector[i].POI_type == "university" || street_graph.POI_info_vector[i].POI_type == "college") {

            Surface university = load_png_from_file("university.png");
            draw_surface(university, street_graph.POI_info_vector[i].POI_position.x, street_graph.POI_info_vector[i].POI_position.y);
        } else if (street_graph.POI_info_vector[i].POI_type == "bank" || street_graph.POI_info_vector[i].POI_type == "atm" || street_graph.POI_info_vector[i].POI_type == "financial") {

            Surface bank = load_png_from_file("bank.png");
            draw_surface(bank, street_graph.POI_info_vector[i].POI_position.x, street_graph.POI_info_vector[i].POI_position.y);
        } else if (street_graph.POI_info_vector[i].POI_type == "cafe") {

            Surface cafe = load_png_from_file("cafe.png");
            draw_surface(cafe, street_graph.POI_info_vector[i].POI_position.x, street_graph.POI_info_vector[i].POI_position.y);
        } else if (street_graph.POI_info_vector[i].POI_type == "karaoke" || street_graph.POI_info_vector[i].POI_type == "pub" || street_graph.POI_info_vector[i].POI_type == "spa") {

            Surface entertainment = load_png_from_file("entertainment.png");
            draw_surface(entertainment, street_graph.POI_info_vector[i].POI_position.x, street_graph.POI_info_vector[i].POI_position.y);

        } else if (street_graph.POI_info_vector[i].POI_type == "cinema" || street_graph.POI_info_vector[i].POI_type == "theater") {

            Surface cinema = load_png_from_file("cinema.png");
            draw_surface(cinema, street_graph.POI_info_vector[i].POI_position.x, street_graph.POI_info_vector[i].POI_position.y);

        } else if (street_graph.POI_info_vector[i].POI_type == "pharmacy") {

            Surface pharmacy = load_png_from_file("drug.png");
            draw_surface(pharmacy, street_graph.POI_info_vector[i].POI_position.x, street_graph.POI_info_vector[i].POI_position.y);

        } else if (street_graph.POI_info_vector[i].POI_type == "post_office") {

            Surface post_office = load_png_from_file("post.png");
            draw_surface(post_office, street_graph.POI_info_vector[i].POI_position.x, street_graph.POI_info_vector[i].POI_position.y);
        }

    }

}

//store OSM information

void initialize_OSMNode_Map() {

    for (unsigned i = 0; i < street_graph.number_of_nodes; i++) {

        const OSMNode* this_node = getNodeByIndex(i);

        OSMNode_vector.push_back(this_node);

        unsigned tag_count = getTagCount(this_node);

        std::unordered_map<std::string, std::string> Tag_Pair;

        for (unsigned j = 0; j < tag_count; j++) {

            Tag_Pair.insert(getTagPair(this_node, j));
        }

        OSMEntity_Point_Tags.insert(std::make_pair(this_node, Tag_Pair));

        OSMID this_id = this_node->id();

        OSMID_OSMNode.insert(std::make_pair(this_id, this_node));

        OSMID_OSMEntity.insert(std::make_pair(this_id, this_node));
    }


}

//store OSMWay information

void initialize_OSMWay_Map() {

    for (unsigned i = 0; i < street_graph.numebr_of_ways; i++) {

        const OSMWay* this_node = getWayByIndex(i);

        OSMWay_vector.push_back(this_node);

        unsigned tag_count = getTagCount(this_node);

        std::unordered_map<std::string, std::string> Tag_Pair;

        for (unsigned j = 0; j < tag_count; j++) {

            Tag_Pair.insert(getTagPair(this_node, j));
        }

        OSMEntity_Point_Tags.insert(std::make_pair(this_node, Tag_Pair));

        OSMID this_id = this_node->id();
        draw_subway_staions();

        OSMID_OSMWay.insert(std::make_pair(this_id, this_node));

        OSMID_OSMEntity.insert(std::make_pair(this_id, this_node));
    }

}

//store OSMRelation information

void initialize_OSMRelation_Map() {


    for (unsigned i = 0; i < street_graph.number_of_relations; i++) {

        const OSMRelation* this_node = getRelationByIndex(i);

        OSMRelation_vector.push_back(this_node);

        unsigned tag_count = getTagCount(this_node);

        std::unordered_map<std::string, std::string> Tag_Pair;

        for (unsigned j = 0; j < tag_count; j++) {

            Tag_Pair.insert(getTagPair(this_node, j));
        }

        OSMEntity_Point_Tags.insert(std::make_pair(this_node, Tag_Pair));

        OSMID this_id = this_node->id();

        OSMID_OSMRelation.insert(std::make_pair(this_id, this_node));

        OSMID_OSMEntity.insert(std::make_pair(this_id, this_node));
    }
}

//stores transportation stations information (location & station name)

void initialize_station_info() {

    for (unsigned i = 0; i < street_graph.number_of_nodes; i++) {

        bool is_station = false;

        const OSMNode* this_node = OSMNode_vector[i];

        std::unordered_map<std::string, std::string> check_tags = OSMEntity_Point_Tags.find(this_node) -> second;

        auto itr = check_tags.find("public_transport");

        itr = check_tags.find("highway");

        if (itr != check_tags.end()) {

            if (itr->second == "bus_stop") {
                initialize_station_info_heler("bus", this_node, check_tags);
            }

        } else {

            itr = check_tags.find("railway");

            if (itr != check_tags.end()) {

                if (itr->second == "station" || itr->second == "subway_entrance") {
                    initialize_station_info_heler("subway", this_node, check_tags);
                }
            } else {

                itr = check_tags.find("aeroway");

                if (itr != check_tags.end()) {

                    itr = check_tags.find("amenity");

                    if (itr != check_tags.end()) {

                        if (itr->second == "ferry_terminal") {

                            initialize_station_info_heler("ferry", this_node, check_tags);
                        }
                    }

                }

            }
        }
    }
}

//store stations into different vectors based on its type

void initialize_station_info_heler(std::string transport, const OSMNode* this_node, std::unordered_map<std::string, std::string> check_tags) {

    LatLon position_latlon = this_node->coords();
    t_point position = LatLon_XY_converter(position_latlon);

    std::string station_name;

    auto name = check_tags.find("name");
    if (name != check_tags.end() && name ->second != "<unknown>")
        station_name = name ->second;
    else
        station_name = "\0";


    if (transport == "bus") {
        bus_station_location.push_back(position);
        bus_station_name.push_back(station_name);
    } else if (transport == "subway") {
        subway_station_location.push_back(position);
        subway_station_name.push_back(station_name);
    } else if (transport == "ferry") {

        ferry_station_location.push_back(position);
        ferry_station_name.push_back(station_name);
    }
}

//insert station locations into r_tree

void initialize_station_tree() {

    for (unsigned i = 0; i < bus_station_location.size(); i++) {

        t_point position1 = bus_station_location[i];
        t_point_point point_posistion1 = t_point_point(position1.x, position1.y);
        bus_station_rtree.insert(std::make_pair(point_posistion1, i));
    }

    for (unsigned j = 0; j < subway_station_location.size(); j++) {

        t_point position2 = subway_station_location[j];
        t_point_point point_posistion2 = t_point_point(position2.x, position2.y);
        subway_station_rtree.insert(std::make_pair(point_posistion2, j));
    }
}

//find closest bus station from bus station location r_tree

t_point find_closest_bus_station(t_point my_position) {

    //transfer input my_position to point type
    float x_my_position = my_position.x;
    float y_my_position = my_position.y;

    t_point_point target = t_point_point(x_my_position, y_my_position);

    std::vector<bus_station_position> possible_station_id;

    bus_station_rtree.query(bgi::nearest(target, 3), std::back_inserter(possible_station_id));

    t_point closest_bus_station = bus_station_location[possible_station_id[0].second];

    return closest_bus_station;

}


// accessing OSMID to retrieve transportation information

void initialize_transport_lines() {

    for (unsigned i = 0; i < street_graph.numebr_of_ways; i++) {

        const OSMWay* this_way = OSMWay_vector[i];

        unordered_map<string, string> check_tags = OSMEntity_Point_Tags.find(this_way) -> second;

        auto itr = check_tags.find("railway");

        if (itr != check_tags.end()) {

            if (itr ->second == "subway") {

                vector<OSMID> noderef = this_way -> ndrefs();

                initialize_transport_lines_helper("subway", this_way);

                transport_line.push_back(noderef);
            }
        } else {

            auto itr = check_tags.find("route");

            if (itr != check_tags.end()) {

                if (itr ->second == "ferry") {

                    vector<OSMID> noderef = this_way -> ndrefs();

                    initialize_transport_lines_helper("ferry", this_way);

                    transport_line.push_back(noderef);
                }
            }
        }
    }
}

void initialize_transport_lines_helper(std::string transport, const OSMWay * this_way) {


    if (transport == "subway") {

        vector<OSMID> noderef = this_way -> ndrefs();

        subway_line.push_back(noderef);

    } else if (transport == "ferry") {

        vector<OSMID> noderef = this_way ->ndrefs();

        ferry_line.push_back(noderef);

    }

}

// Distinguish different transportation methods

void draw_transport_lines(std::string transport) {

    if (transport == "subway") {

        for (unsigned i = 0; i < subway_line.size(); i++) {

            std::vector<OSMID> this_subway_line = subway_line[i];

            for (unsigned j = 1; j < this_subway_line.size(); j++) {

                t_point node_position1 = LatLon_XY_converter((OSMID_OSMNode.find(this_subway_line[j - 1])->second->coords()));

                t_point node_position2 = LatLon_XY_converter((OSMID_OSMNode.find(this_subway_line[j])->second->coords()));

                setlinewidth(3);

                if (zoom_level <= zoom_level_3)
                    setlinewidth(1);

                setcolor(128, 0, 0);
                drawline(node_position1, node_position2);
            }
        }

    } else if (transport == "ferry") {

        for (unsigned i = 0; i < ferry_line.size(); i++) {

            std::vector<OSMID> this_ferry_line = ferry_line[i];

            for (unsigned j = 1; j < this_ferry_line.size(); j++) {

                t_point node_position1 = LatLon_XY_converter((OSMID_OSMNode.find(this_ferry_line[j - 1])->second->coords()));

                t_point node_position2 = LatLon_XY_converter((OSMID_OSMNode.find(this_ferry_line[j])->second->coords()));

                setlinewidth(3);

                if (zoom_level <= zoom_level_4)
                    setlinewidth(1);

                setcolor(220, 220, 220);

                drawline(node_position1, node_position2);
            }
        }
    }
}

void draw_bus_staions() {

    t_bound_box current_screen_bound = get_visible_world();

    for (unsigned i = 0; i < bus_station_location.size(); i++) {

        t_point station_position = bus_station_location[i];

        if (station_position.x > current_screen_bound.left() && station_position.x < current_screen_bound.right() && station_position.y > current_screen_bound.bottom() && station_position.y < current_screen_bound.top()) {

            Surface bus = load_png_from_file("bus.png");

            draw_surface(bus, station_position.x, station_position.y);
        }
    }
}

void draw_ferry_staions() {

    t_bound_box initial_coords = t_bound_box(boundary[3], boundary[2], boundary[1], boundary[0]);

    t_bound_box current_screen_bound = get_visible_world();

    for (unsigned i = 0; i < ferry_station_location.size(); i++) {

        t_point station_position = ferry_station_location[i];

        if (station_position.x > current_screen_bound.left() && station_position.x < current_screen_bound.right() && station_position.y > current_screen_bound.bottom() && station_position.y < current_screen_bound.top()) {

            std::string station_name = ferry_station_name[i];

            Surface ferry = load_png_from_file("ferry.png");

            draw_surface(ferry, station_position.x, station_position.y);

            if (zoom_level > zoom_level_6) {
                set_draw_mode(DRAW_NORMAL);
                setlinestyle(SOLID, ROUND);
                setlinewidth(1);
                setcolor(BLACK);
                setfontsize(10);
                settextrotation(0);
                drawtext(station_position, station_name, initial_coords);
            }
        }
    }
}

void draw_subway_staions() {

    std::unordered_map <std::string, unsigned> check_drew;

    t_bound_box initial_coords = t_bound_box(boundary[3], boundary[2], boundary[1], boundary[0]);

    for (unsigned i = 0; i < subway_station_location.size(); i++) {

        t_point station_position = subway_station_location[i];

        std::string station_name = get_subway_station_name(station_position);

        check_drew.insert(std::make_pair(station_name, i));

        Surface subway = load_png_from_file("subway.png");

        draw_surface(subway, station_position.x, station_position.y);

        if (zoom_level >= zoom_level_7) {
            set_draw_mode(DRAW_NORMAL);
            setlinestyle(SOLID, ROUND);
            setlinewidth(1);
            setcolor(BLACK);
            setfontsize(10);
            settextrotation(0);
            drawtext(station_position, station_name, initial_coords);
        }
    }
}

// Highlight intersections in the visible world

void highlight_intersection() {

    /*t_bound_box message_display_box = get_visible_world();

    float left_offset2 = message_display_box.get_width() * left_offset2_const;
    float left_coordinate2 = message_display_box.left() + left_offset2;
    float right_offset2 = message_display_box.get_width() * right_offset2_const;
    float right_coordinate2 = message_display_box.right() - right_offset2;
    float top_offset2 = message_display_box.get_height() * top_offset2_const;
    float top_coordinate2 = message_display_box.top() - top_offset2;
    float bottom_offset2 = message_display_box.get_height() * bottom_offset2_const;
    float bottom_coordinate2 = message_display_box.bottom() + bottom_offset2;

    setcolor(255, 140, 0);
    fillrect(left_coordinate2, bottom_coordinate2, right_coordinate2, top_coordinate2);

    t_bound_box input_center(left_coordinate2, bottom_coordinate2, right_coordinate2, top_coordinate2);
    setcolor(BLACK);
    setfontsize(16);
    settextrotation(0);

    std::string message = "***** Starting Hightlighting Intersections *****";

    drawtext_in(input_center, message);


    t_bound_box _boundary = get_visible_world();

    for (unsigned i = 0; i < street_graph.number_of_intersections; i++) {

        t_point position = street_graph.intersection_position_vector[i];

        //std::cout<<position.x << "***" <<position.y <<std::endl;

        if (position.x <= _boundary.right() && position.y >= _boundary.bottom()) {

            Surface high_light = load_png_from_file("highlighter.png");

            draw_surface_intersection(high_light, position.x, position.y);
        }
    }

    message = "***** Highlighting Intersections Completed *****";

    setcolor(255, 140, 0);
    fillrect(left_coordinate2, bottom_coordinate2, right_coordinate2, top_coordinate2);

    setcolor(BLACK);
    setfontsize(16);
    settextrotation(0);

    drawtext_in(input_center, message);*/

}

// Highlight POI in the visible world

void highlight_POI() {

    t_bound_box message_display_box = get_visible_world();

    float left_offset2 = message_display_box.get_width() * left_offset2_const;
    float left_coordinate2 = message_display_box.left() + left_offset2;
    float right_offset2 = message_display_box.get_width() * right_offset2_const;
    float right_coordinate2 = message_display_box.right() - right_offset2;
    float top_offset2 = message_display_box.get_height() * top_offset2_const;
    float top_coordinate2 = message_display_box.top() - top_offset2;
    float bottom_offset2 = message_display_box.get_height() * bottom_offset2_const;
    float bottom_coordinate2 = message_display_box.bottom() + bottom_offset2;

    setcolor(255, 140, 0);
    fillrect(left_coordinate2, bottom_coordinate2, right_coordinate2, top_coordinate2);

    t_bound_box input_center(left_coordinate2, bottom_coordinate2, right_coordinate2, top_coordinate2);
    setcolor(BLACK);
    setfontsize(16);
    settextrotation(0);

    std::string message = "***** Starting Hightlighting POI *****";

    drawtext_in(input_center, message);

    t_bound_box _boundary = get_visible_world();

    draw_POI_icon_by_type();

    setcolor(255, 140, 0);
    fillrect(left_coordinate2, bottom_coordinate2, right_coordinate2, top_coordinate2);

    setcolor(BLACK);
    setfontsize(16);
    settextrotation(0);

    message = "***** Highlighting Completed *****";

    drawtext_in(input_center, message);

}

void act_on_button_press(float x, float y, t_event_buttonPressed event) {

    drawscreen();

    t_point target = t_point(x, y);

    Surface mouse_click = load_png_from_file("mouse-click.png");

    draw_surface(mouse_click, x, y);

}

bool to_upper = false;

bool first_input = true;

unsigned input_count = 0;

void act_on_keypress1(char c, int keysym) {



    if (first_input) {

        input_count = 1;

        if (c <= 126 && c >= 32 || keysym == 65293 || c == 8) {

            if (test.size() != 0)
                to_upper = false;

            if (keysym == 32)
                to_upper = true;
            else if (test.size() == 0)
                to_upper = true;

            if (to_upper) {

                c = toupper(c);

                to_upper = false;
            }

            if (keysym == 65288) {

                if (test.size() > 0) {
                    test.pop_back();
                }
            } else {

                test += c;
            }

            if (test.size() >= 3) {

                drawscreen();

                setfontsize(15);

                possibile_input.clear();

                possibile_input = search_street_helper(test);

                set_suggest_box(possibile_input);

            }
        }

        set_search_engine1();
    } else {

        input_count = 2;

        if (keysym == XK_Return) {

            matched_intersection_position.clear();

            //std::cout << test << std::endl;
            //std::cout << test2 << std::endl;

            std::vector<unsigned> matched_intersection = find_intersection_ids_from_street_names(test, test2);

            /*for (unsigned a = 0; a < matched_intersection.size(); a++)
                std::cout << matched_intersection[a] << std::endl;*/

            for (unsigned i = 0; i < matched_intersection.size(); i++) {

                t_point postion = LatLon_XY_converter(getIntersectionPosition(matched_intersection[i]));

                matched_intersection_position.push_back(postion);

            }

            test.clear();
            test2.clear();

            first_input = true;

            input_count = 0;

            drawscreen();
        }

        if (c <= 126 && c >= 32 || keysym == 65293 || c == 8) {

            if (test2.size() != 0)
                to_upper = false;

            if (keysym == XK_space)
                to_upper = true;
            else if (test2.size() == 0)
                to_upper = true;

            if (to_upper) {

                c = toupper(c);

                to_upper = false;
            }

            if (keysym == XK_BackSpace) {

                if (test2.size() > 0) {
                    test2.pop_back();
                }
            } else {

                test2 += c;
            }

            if (test2.size() >= 3) {

                drawscreen();

                setfontsize(15);

                possibile_input.clear();

                possibile_input = search_street_helper(test2);

                (possibile_input);

            }
        }
    }
    set_search_engine1();

}



// Initialize search bar by accepting enter key

void act_on_keypress2(char c, int keysym) {

    //std::cout << "find_this_street:  "<< find_this_street <<std::endl;

    if (find_this_street.size() != 0)
        to_upper = false;

    if (keysym == XK_space)
        to_upper = true;
    else if (find_this_street.size() == 0)
        to_upper = true;

    if (to_upper) {

        c = toupper(c);

        to_upper = false;

    }

    if (keysym == XK_BackSpace) {

        if (find_this_street.size() > 0) {
            find_this_street.pop_back();
        }
    } else {

        find_this_street += c;
    }

    if (find_this_street.size() >= 3) {

        drawscreen();

        setfontsize(15);

        possibile_input.clear();

        possibile_input = search_street_helper(find_this_street);

        set_suggest_box(possibile_input);

    }

    if (keysym == XK_Return) {

        find_this_street.resize(find_this_street.size() - 1);

        matched_street_number.clear();

        matched_street_number = streetID_from_name[find_this_street];

        find_this_street.clear();

        drawscreen();

    }

    set_search_engine2();
}

// Highlight user click intersection

void highlight_matched_intersection() {

    //std::cout << "matched intersection ---->    " << matched_intersection_position.size() << std::endl;

    Surface search_result = load_png_from_file("mouse-click.png");

    for (unsigned i = 0; i < matched_intersection_position.size(); i++) {

        t_point this_matched_intersection_position = matched_intersection_position[i];

        draw_surface(search_result, this_matched_intersection_position.x, this_matched_intersection_position.y);
    }
}

/*****************************************************************
 * Display matched street for street searching                   *
 *****************************************************************/

void highlight_matched_street() {

    //std::cout << "matched street ---->    " << matched_street_number.size() << std::endl;

    for (unsigned i = 0; i < matched_street_number.size(); i++) {

        std::unordered_set<unsigned> this_matched_street_segment = street_graph.street_street_segments[matched_street_number[i]];

        auto set_itr = this_matched_street_segment.begin();

        for (unsigned j = 0; j < this_matched_street_segment.size(); j++) {

            if (j == 0) {

                unsigned from_intersection = getStreetSegmentInfo(*this_matched_street_segment.begin()).from;

                t_point from_intersection_position = street_graph.intersection_position_vector[from_intersection];

                Surface flag = load_png_from_file("flag.png");

                draw_surface(flag, from_intersection_position.x, from_intersection_position.y);

            }

            highlight_this_segment(*set_itr);

            set_itr++;
        }
    }

}

/*****************************************************************
 * Display path for intersection - intersection path searching   *
 *****************************************************************/

void highlight_path_intersection() {

    for (unsigned i = 0; i < path_segment_intersection.size(); i++) {
        for (unsigned j = 0; j < path_segment_intersection[i].size(); j++) {

            highlight_this_segment(path_segment_intersection[i][j]);
        }
    }
}

/******************************************************************
 * Display path for intersection - POI name path searching        *
 ******************************************************************/

void highlight_path_POI() {

    for (unsigned i = 0; i < path_segment_POI.size(); i++) {

        for (unsigned j = 0; j < path_segment_POI[i].size(); j++) {

            highlight_this_segment(path_segment_POI[i][j]);
        }
    }
}

/**********************************************************************************
 * Display path for mouse click intersection - intersection path searching        *
 **********************************************************************************/

void highlight_path_click() {

    for (unsigned i = 0; i < path_segment_click.size(); i++) {

        highlight_this_segment(path_segment_click[i]);
    }
}

void hightlight_path_terminal() {

    if (print_path_termainal) {

        t_point path_start_intersection_pos = street_graph.intersection_position_vector[path_start];

        Surface start = load_png_from_file("start.png");
        draw_surface(start, path_start_intersection_pos.x, path_start_intersection_pos.y);

        t_point path_end_intersection_pos = street_graph.intersection_position_vector[path_end];

        Surface end = load_png_from_file("end.png");
        draw_surface_intersection(end, path_end_intersection_pos.x, path_end_intersection_pos.y);
    }

}

/*****************************************************************
 * Display path Description after path searching is completed    *
 *****************************************************************/

void print_path_description(std::vector<unsigned> path) {

    if (path.size() == 0) {

        std::cout << "-----------------------------------------------------------------------" << std::endl;
        std::cout << "              Path Description Not Available!                          " << std::endl;
        std::cout << "-----------------------------------------------------------------------" << std::endl;

        return;
    } else {
        std::cout << "-----------------------------------------------------------------------" << std::endl;
        std::cout << "                           Path Description                            " << std::endl;
        std::cout << "-----------------------------------------------------------------------" << std::endl;

        double distance = 0;
        double distance_temp = 0;

        std::string street_name1, street_name2;

        std::vector<pathInfo> path_street_segment;

        std::vector<std::string> path_turn;

        unsigned terminal1_1, terminal1_2, terminal2_1, terminal2_2;

        for (unsigned i = 1; i < path.size(); i++) {

            street_name1 = street_graph.segment_street_name[path[i - 1]];
            street_name2 = street_graph.segment_street_name[path[i]];

            distance += segment_length_vector[path[i - 1]];
            distance_temp = segment_length_vector[path[i]];

            if (street_name1 != street_name2) {

                pathInfo this_street;

                this_street.name = street_name1;
                this_street.length = distance;
                this_street.time = find_street_segment_travel_time(path[i - 1]);
                path_street_segment.push_back(this_street);

                if (i == (path.size() - 1)) {

                    pathInfo this_street;

                    this_street.name = street_name2;
                    this_street.length = distance_temp;
                    this_street.time = find_street_segment_travel_time(path[i]);
                    path_street_segment.push_back(this_street);
                }

                std::vector<t_point> turning_point;

                terminal1_1 = getStreetSegmentInfo(path[i - 1]).from;

                terminal1_2 = getStreetSegmentInfo(path[i - 1]).to;

                terminal2_1 = getStreetSegmentInfo(path[i]).from;

                terminal2_2 = getStreetSegmentInfo(path[i]).to;

                if (terminal1_1 == terminal2_1) {
                    turning_point.push_back(street_graph.intersection_position_vector[terminal1_2]);
                    turning_point.push_back(street_graph.intersection_position_vector[terminal1_1]);
                    turning_point.push_back(street_graph.intersection_position_vector[terminal2_2]);
                } else {
                    turning_point.push_back(street_graph.intersection_position_vector[terminal1_2]);
                    turning_point.push_back(street_graph.intersection_position_vector[terminal1_1]);
                    turning_point.push_back(street_graph.intersection_position_vector[terminal2_1]);
                }



                float angle1 = atan2(turning_point[1].y - turning_point[0].y, turning_point[1].x - turning_point[0].x) * DEG_TO_RAD;

                float angle2 = atan2(turning_point[2].y - turning_point[1].y, turning_point[2].x - turning_point[1].x) * DEG_TO_RAD;

                float angle_difference = angle2 - angle1;

                if (angle_difference > 180)
                    angle_difference -= 360;
                else if (angle_difference < -180)
                    angle_difference += 360;

                if (angle_difference < 180 && angle_difference > 0)
                    path_turn.push_back("turn left");
                else if (angle_difference < 0 && angle_difference > -180)
                    path_turn.push_back("turn right");
                else if (angle_difference == -180)
                    path_turn.push_back("turn around");
                else if (angle_difference == 0)
                    path_turn.push_back("keep going straightly");


            } else if (i == path.size() - 1) {

                pathInfo this_street;
                this_street.name = street_name1;
                this_street.length = distance + distance_temp;
                this_street.time = find_street_segment_travel_time(path[i - 1]) + find_street_segment_travel_time(path[i]);
                path_street_segment.push_back(this_street);
            }
        }

        unsigned total_travle_time = 0;

        unsigned total_travle_distance = 0;

        for (unsigned j = 0; j < path_street_segment.size(); j++) {

            std::cout << "-----------------------------------------------------------------------" << std::endl;
            std::cout << std::setprecision(2) << " Go to " << path_street_segment[j].name << " for " << path_street_segment[j].length << " meters, " << path_street_segment[j].time << " seconds." << std::endl;

            if (j < path_turn.size()) {
                std::cout << " Please " << path_turn[j] << std::endl;
            }

            total_travle_time += path_street_segment[j].time;

            total_travle_distance += path_street_segment[j].length;

        }


        std::cout << "-----------------------------------------------------------------------" << std::endl;
        std::cout << std::setprecision(2) << " The total time cost is: " << total_travle_time << " seconds, distance is: " << total_travle_distance << " meters." << std::endl;
        std::cout << "-----------------------------------------------------------------------" << std::endl;
        std::cout << " Arrived Destination!!" << std::endl;
        std::cout << "-----------------------------------------------------------------------" << std::endl;
    }

    return;
}

void highlight_this_segment(unsigned segment_id) {

    //std::cout << "Highlighting segment" << std::endl;

    StreetSegmentInfo segment_info = getStreetSegmentInfo(segment_id);

    setlinewidth(5);

    if (zoom_level >= zoom_level_4)
        setlinewidth(5);

    setcolor(0, 0, 128);

    LatLon start_point = getIntersectionPosition(segment_info.from);
    LatLon end_point = getIntersectionPosition(segment_info.to);

    float start_lon = start_point.lon();
    float start_lat = start_point.lat();

    float start_x = (start_lon * cos(street_graph.avgLat * DEG_TO_RAD));
    float start_y = start_lat;

    float end_lon = end_point.lon();
    float end_lat = end_point.lat();

    float end_x = (end_lon * cos(street_graph.avgLat * DEG_TO_RAD));
    float end_y = end_lat;

    t_point start_t_point = t_point(start_x, start_y);

    t_point end_t_point = t_point(end_x, end_y);

    unsigned curve_point = segment_info.curvePointCount;

    for (unsigned j = 0; j <= curve_point; j++) {

        if (curve_point == 0) {

            drawline(start_x, start_y, end_x, end_y);


        } else if (curve_point == 1) {

            LatLon curve_point_position = getStreetSegmentCurvePoint(segment_id, 0);

            float curve_point_x = (curve_point_position.lon() * cos(street_graph.avgLat * DEG_TO_RAD));

            float curve_point_y = curve_point_position.lat();

            t_point curve_t_point = t_point(curve_point_x, curve_point_y);

            drawline(start_t_point, curve_t_point);

            drawline(end_t_point, curve_t_point);

        } else {

            for (unsigned k = 1; k < (curve_point); k++) {

                LatLon curve_point_position1 = getStreetSegmentCurvePoint(segment_id, k - 1);

                LatLon curve_point_position2 = getStreetSegmentCurvePoint(segment_id, k);

                float curve_point_x1 = (curve_point_position1.lon() * cos(street_graph.avgLat * DEG_TO_RAD));

                float curve_point_y1 = curve_point_position1.lat();

                t_point curve_t_point1 = t_point(curve_point_x1, curve_point_y1);

                float curve_point_x2 = (curve_point_position2.lon() * cos(street_graph.avgLat * DEG_TO_RAD));

                float curve_point_y2 = curve_point_position2.lat();

                t_point curve_t_point2 = t_point(curve_point_x2, curve_point_y2);

                drawline(curve_t_point1, curve_t_point2);

                if (k == 1) {

                    drawline(start_t_point, curve_t_point1);

                }
                if (k == (curve_point - 1)) {

                    drawline(end_t_point, curve_t_point2);
                }
            }
        }
    }

}

// Return closest intersection information when mouse click detected

unsigned path_find_counter = 0;

std::vector<unsigned> intersection_terminal;

void act_on_intersection_info(float x, float y, t_event_buttonPressed event) {

    print_path_termainal = true;

    path_segment_intersection.clear();

    path_segment_click.clear();

    path_segment_POI.clear();

    clearscreen();

    drawscreen();

    Surface mouse_click = load_png_from_file("mouse-click.png");

    draw_surface(mouse_click, x, y);

    t_bound_box initial_coords = t_bound_box(boundary[3], boundary[2], boundary[1], boundary[0]);

    t_point target_xy = t_point(x, y);

    LatLon target_latlon = XY_LatLon_Converter(target_xy);

    unsigned closest_intersection = find_closest_intersection(target_latlon);

    t_point closest_intersection_position = street_graph.intersection_position_vector[closest_intersection];

    std::vector<std::string> intersection_street_names = find_intersection_street_names(closest_intersection);

    std::unordered_map<std::string, unsigned> intersection_streets;

    std::string intersection_message;

    for (unsigned i = 0; i < intersection_street_names.size(); i++) {

        std::string check_name = intersection_street_names[i];

        if (intersection_streets.empty() && check_name != "<unknown>") {
            intersection_streets.insert(std::make_pair(check_name, i));

        } else if (intersection_streets.find(check_name) == intersection_streets.end() && check_name != "<unknown>") {

            intersection_streets.insert(std::make_pair(check_name, i));
        }
    }

    auto map_itr = intersection_streets.begin();

    for (unsigned j = 0; j < intersection_streets.size(); j++) {

        if (j == 0)
            intersection_message = "Intersection Streets: " + (*map_itr).first;

        else
            intersection_message = intersection_message + " And " + (*map_itr).first;

        map_itr++;
    }

    t_bound_box message_display_box = get_visible_world();

    float left_offset2 = message_display_box.get_width() * left_offset2_const;
    float left_coordinate2 = message_display_box.left() + left_offset2;
    float right_offset2 = message_display_box.get_width() * right_offset2_const;
    float right_coordinate2 = message_display_box.right() - right_offset2;
    float top_offset2 = message_display_box.get_height() * top_offset2_const;
    float top_coordinate2 = message_display_box.top() - top_offset2;
    float bottom_offset2 = message_display_box.get_height() * bottom_offset2_const;
    float bottom_coordinate2 = message_display_box.bottom() + bottom_offset2;

    setcolor(PINK);
    fillrect(left_coordinate2, bottom_coordinate2, right_coordinate2, top_coordinate2);

    t_bound_box input_center(left_coordinate2, bottom_coordinate2, right_coordinate2, top_coordinate2);
    setcolor(BLACK);
    setfontsize(16);
    settextrotation(0);
    drawtext_in(input_center, intersection_message);

    /******************************************************************
     *  When two intersections are clicked (path_find_counter == 2),  *
     *  The system would start searching for path between these two   *
     *  intersections. And the path will be highlighted.              *  
     ******************************************************************/
    path_find_counter++;
    intersection_terminal.push_back(closest_intersection);

    if (path_find_counter == 2) {

        path_find_counter = 0;

        std::vector<unsigned> path = find_path_between_intersections(intersection_terminal[0], intersection_terminal[1], 0);

        path_start = intersection_terminal[0];

        path_end = intersection_terminal[1];

        path_segment_click = path;

        print_path_description(path_segment_click);

        highlight_path_click();

        intersection_terminal.clear();
    }
}


// Initialization of search bar

void set_search_engine1() {

    t_bound_box search_box = get_visible_world();

    float left_offset = search_box.get_width() * left_offset1_const;
    float left_coordinate = search_box.left() + left_offset;
    float right_offset = search_box.get_width() * right_offset1_const;
    float right_coordinate = search_box.right() - right_offset;
    float top_offset = search_box.get_height() * top_offset1_const;
    float top_coordinate = search_box.top() - top_offset;
    float bottom_offset = search_box.get_height() * bottom_offset1_const;
    float bottom_coordinate = search_box.bottom() + bottom_offset;

    //outer border
    float left_offset2 = search_box.get_width() * left_offset2_const2;
    float left_coordinate2 = search_box.left() + left_offset2;
    float right_offset2 = search_box.get_width() * right_offset2_const2;
    float right_coordinate2 = search_box.right() - right_offset2;
    float top_offset2 = search_box.get_height() * top_offset2_const2;
    float top_coordinate2 = search_box.top() - top_offset2;
    float bottom_offset2 = search_box.get_height() * bottom_offset2_const2;
    float bottom_coordinate2 = search_box.bottom() + bottom_offset2;

    setcolor(PINK);
    fillrect(left_coordinate2, bottom_coordinate2, right_coordinate2, top_coordinate2);

    setcolor(WHITE);
    fillrect(left_coordinate, bottom_coordinate, right_coordinate, top_coordinate);

    box_height = top_coordinate2 - bottom_coordinate2;

    t_bound_box input_center(left_coordinate, bottom_coordinate, right_coordinate, top_coordinate);
    setcolor(BLACK);
    setfontsize(20);
    settextrotation(0);
    drawtext_in(input_center, test);


    setcolor(PINK);
    fillrect(left_coordinate2 + (right_coordinate2 - left_coordinate2 + left_offset2), bottom_coordinate2, right_coordinate2 + (right_coordinate2 - left_coordinate2 + left_offset2), top_coordinate2);

    setcolor(WHITE);
    fillrect(left_coordinate + (right_coordinate2 - left_coordinate2 + left_offset2), bottom_coordinate, right_coordinate + (right_coordinate2 - left_coordinate2 + left_offset2), top_coordinate);

    t_bound_box input_center2(left_coordinate + (right_coordinate2 - left_coordinate2 + left_offset2), bottom_coordinate, right_coordinate + (right_coordinate2 - left_coordinate2 + left_offset2), top_coordinate);
    setcolor(BLACK);
    drawtext_in(input_center2, test2);

}

void set_search_engine2() {

    t_bound_box search_box = get_visible_world();

    float left_offset = search_box.get_width() * left_offset1_const;
    float left_coordinate = search_box.left() + left_offset;
    float right_offset = search_box.get_width() * right_offset1_const;
    float right_coordinate = search_box.right() - right_offset;
    float top_offset = search_box.get_height() * top_offset1_const;
    float top_coordinate = search_box.top() - top_offset;
    float bottom_offset = search_box.get_height() * bottom_offset1_const;
    float bottom_coordinate = search_box.bottom() + bottom_offset;

    //outer border
    float left_offset2 = search_box.get_width() * left_offset2_const2;
    float left_coordinate2 = search_box.left() + left_offset2;
    float right_offset2 = search_box.get_width() * right_offset2_const2;
    float right_coordinate2 = search_box.right() - right_offset2;
    float top_offset2 = search_box.get_height() * top_offset2_const2;
    float top_coordinate2 = search_box.top() - top_offset2;
    float bottom_offset2 = search_box.get_height() * bottom_offset2_const2;
    float bottom_coordinate2 = search_box.bottom() + bottom_offset2;

    box_height = top_coordinate2 - bottom_coordinate2;

    setcolor(PINK);
    fillrect(left_coordinate2, bottom_coordinate2, right_coordinate2, top_coordinate2);

    setcolor(WHITE);
    fillrect(left_coordinate, bottom_coordinate, right_coordinate, top_coordinate);

    t_bound_box input_center(left_coordinate, bottom_coordinate, right_coordinate, top_coordinate);
    setcolor(BLACK);
    setfontsize(15);
    settextrotation(0);
    drawtext_in(input_center, find_this_street);
}

void set_search_bar() {

    t_bound_box screen_bound = get_visible_world();

    float xc = 0.5 * (screen_bound.right() + screen_bound.left());

    float yc = 0.5 * (screen_bound.bottom() + screen_bound.top());

    Surface list = load_png_from_file("path_intersection.png");

    draw_surface_without_offset(list, xc - 0.5 * screen_bound.get_width(), yc + 0.5 * screen_bound.get_height());


}

void set_suggest_box(std::vector<std::string> suggest_street) {
    
    t_bound_box search_box = get_visible_world();

    float left_offset2 = search_box.get_width() * left_offset2_const2;
    float left_coordinate2 = search_box.left() + left_offset2;
    float right_offset2 = search_box.get_width() * right_offset2_const2;
    float right_coordinate2 = search_box.right() - right_offset2;
    float top_offset2 = search_box.get_height() * top_offset2_const2;
    float top_coordinate2 = search_box.top() - top_offset2;
    float bottom_offset2 = search_box.get_height() * bottom_offset2_const2;
    float bottom_coordinate2 = search_box.bottom() + bottom_offset2;

    settextrotation(0);
    setfontsize(15);

    unsigned counter = 0;

    if (suggest_street.size() < 5) {

        counter = 5 - suggest_street.size();
    }

    unsigned counter2 = 0;

    unsigned counter3 = 1;

    while (counter < 5) {

        t_bound_box text_bound = t_bound_box(left_coordinate2, bottom_coordinate2 - box_height*counter3, right_coordinate2, top_coordinate2 - box_height * counter3);

        setcolor(t_color(135, 206, 250, 120));
        fillrect(text_bound);

        setcolor(BLACK);
        drawtext_in(text_bound, suggest_street[counter2]);

        counter++;
        counter2++;
        counter3++;
    }

}

/************************************************************
 * When user act one term in suggest box, the search bar    *
 * would be auto filled                                     *
 ************************************************************/

void act_on_choose_street(float x, float y, t_event_buttonPressed event) {

    t_bound_box search_box = get_visible_world();

    float left_offset2 = search_box.get_width() * left_offset2_const2;
    float left_coordinate2 = search_box.left() + left_offset2;
    float right_offset2 = search_box.get_width() * right_offset2_const2;
    float right_coordinate2 = search_box.right() - right_offset2;
    float top_offset2 = search_box.get_height() * top_offset2_const2;
    float top_coordinate2 = search_box.top() - top_offset2;
    float bottom_offset2 = search_box.get_height() * bottom_offset2_const2;
    float bottom_coordinate2 = search_box.bottom() + bottom_offset2;


    if (input_count == 0) {
        if (x > left_coordinate2 && x < right_coordinate2 && y < top_coordinate2 - box_height) {
            if (y > bottom_coordinate2 - box_height) {

                find_this_street = possibile_input[0];

            } else if (y > bottom_coordinate2 - 2 * box_height) {

                find_this_street = possibile_input[1];

            } else if (y > bottom_coordinate2 - 3 * box_height) {

                find_this_street = possibile_input[2];

            } else if (y > bottom_coordinate2 - 4 * box_height) {

                find_this_street = possibile_input[3];

            } else if (y > bottom_coordinate2 - 5 * box_height) {

                find_this_street = possibile_input[4];

            }
        }

        search_bar_flag = true;
    }
    if (input_count == 1) {

        if (x > left_coordinate2 && x < right_coordinate2 && y < top_coordinate2 - box_height) {
            if (y > bottom_coordinate2 - box_height) {

                test = possibile_input[0];

            } else if (y > bottom_coordinate2 - 2 * box_height) {

                test = possibile_input[1];

            } else if (y > bottom_coordinate2 - 3 * box_height) {

                test = possibile_input[2];

            } else if (y > bottom_coordinate2 - 4 * box_height) {

                test = possibile_input[3];

            } else if (y > bottom_coordinate2 - 5 * box_height) {

                test = possibile_input[4];

            }
        }

        first_input = false;

        search_bar_flag2 = true;

        test2.clear();
    }

    if (input_count == 2) {

        if (x > left_coordinate2 && x < right_coordinate2 && y < top_coordinate2 - box_height) {
            if (y > bottom_coordinate2 - box_height) {

                test2 = possibile_input[0];

            } else if (y > bottom_coordinate2 - 2 * box_height) {

                test2 = possibile_input[1];

            } else if (y > bottom_coordinate2 - 3 * box_height) {

                test2 = possibile_input[2];

            } else if (y > bottom_coordinate2 - 4 * box_height) {

                test2 = possibile_input[3];

            } else if (y > bottom_coordinate2 - 5 * box_height) {

                test2 = possibile_input[4];

            }
        }

        search_bar_flag2 = true;
    }


    drawscreen();

}

void draw_search_bar(bool flag) {

    if (flag)
        set_search_engine2();

}

void draw_search_bar2(bool flag) {

    if (flag)
        set_search_engine1();

}

/************************************************************
 * When user act on reset button, the screen would be       *
 * cleaned and redraw                                       *
 ************************************************************/

void act_on_reset(void (*drawscreen_ptr) (void)) {

    search_bar_flag = false;
    search_bar_flag2 = false;

    clearscreen();

    drawscreen();

    matched_intersection_position.clear();

    matched_street_number.clear();

    print_path_termainal = false;

    path_segment_intersection.clear();

    path_segment_click.clear();

    path_segment_POI.clear();

    event_loop(act_on_button_press, NULL, NULL, drawscreen);

}

/*************************************************************
 *  When user act on help button, an instruction of buttons  *
 *  would be displayed on the screen                         *
 *************************************************************/
void act_on_help(void (*drawscreen_ptr) (void)) {

    clearscreen();

    t_bound_box screen_bound = get_visible_world();

    clearscreen();

    float xc = 0.5 * (screen_bound.right() + screen_bound.left());

    float yc = 0.5 * (screen_bound.bottom() + screen_bound.top());

    Surface help = load_png_from_file("help.png");

    draw_surface_without_offset(help, xc - 0.3 * screen_bound.get_width(), yc + 0.4 * screen_bound.get_height());

    event_loop(act_on_button_press, NULL, NULL, drawscreen);
}

void act_on_city(void (*drawscreen_ptr) (void)) {

    search_bar_flag = false;
    search_bar_flag2 = false;

    clearscreen();

    t_bound_box screen_bound = get_visible_world();

    clearscreen();

    float xc = 0.5 * (screen_bound.right() + screen_bound.left());

    float yc = 0.5 * (screen_bound.bottom() + screen_bound.top());

    std::string mapName = "Select Map City";

    t_color backGround(PINK);

    init_graphics(mapName, backGround);

    set_drawing_buffer(ON_SCREEN);

    set_visible_world(screen_bound);


    Surface map = load_png_from_file("map.png");

    draw_surface_without_offset(map, xc - 0.3 * screen_bound.get_width(), yc + 0.5 * screen_bound.get_height());

    Surface list = load_png_from_file("1list.png");

    draw_surface_without_offset(list, xc - 0.5 * screen_bound.get_width(), yc + 0.5 * screen_bound.get_height());

    event_loop(act_on_choose_city, NULL, NULL, draw_new_map);
}


// Load desired city map based on user input

void act_on_choose_city(float x, float y, t_event_buttonPressed event) {

    t_bound_box screen_bound = get_visible_world();

    float y_start = screen_bound.top() - (0.01115242) * screen_bound.get_height();

    float y_difference = (0.05160726) * screen_bound.get_height();

    if (y < y_start && y > (y_start - y_difference)) {

        std::cout << "toronto" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/toronto_canada.osm.bin";
        this_city_name = "Toronto";

    } else if (y < (y_start - y_difference) && y > (y_start - 2 * y_difference)) {
        std::cout << "beijing" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/beijing_china.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/beijing_china.osm.bin";
        this_city_name = "Beijing";

    } else if (y < (y_start - 2 * y_difference) && y > (y_start - 3 * y_difference)) {

        std::cout << "cario" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/cairo_egypt.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/cairo_egypt.osm.bin";
        this_city_name = "Cario";

    } else if (y < (y_start - 3 * y_difference) && y > (y_start - 4 * y_difference)) {
        std::cout << "golden" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/golden-horseshoe_canada.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/golden-horseshoe_canada.osm.bin";
        this_city_name = "Toronto";

    } else if (y < (y_start - 4 * y_difference) && y > (y_start - 5 * y_difference)) {
        std::cout << "hamilton" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/hamilton_canada.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/hamilton_canada.osm.bin";
        this_city_name = "Hamilton";

    } else if (y < (y_start - 5 * y_difference) && y > (y_start - 6 * y_difference)) {
        std::cout << "hongkong" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/hong-kong_china.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/hong-kong_china.osm.bin";
        this_city_name = "Hong Kong";

    } else if (y < (y_start - 6 * y_difference) && y > (y_start - 7 * y_difference)) {
        std::cout << "iceland" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/iceland.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/iceland.osm.bin";
        this_city_name = "Icelan";


    } else if (y < (y_start - 7 * y_difference) && y > (y_start - 8 * y_difference)) {
        std::cout << "london" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/london_england.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/london_england.osm.bin";
        this_city_name = "London";

    } else if (y < (y_start - 8 * y_difference) && y > (y_start - 9 * y_difference)) {
        std::cout << "moscow" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/moscow_russia.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/moscow_russia.osm.bin";
        this_city_name = "Moscow";

    } else if (y < (y_start - 9 * y_difference) && y > (y_start - 10 * y_difference)) {
        std::cout << "New delhi" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/new-delhi_india.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/new-delhi_india.osm.bin";
        this_city_name = "New Delhi";

    } else if (y < (y_start - 10 * y_difference) && y > (y_start - 11 * y_difference)) {
        std::cout << "New York" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/new-york_usa.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/new-york_usa.osm.bin";
        this_city_name = "New York";

    } else if (y < (y_start - 11 * y_difference) && y > (y_start - 12 * y_difference)) {
        std::cout << "Rio-de-janeiro" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/rio-de-janeiro_brazil.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/rio-de-janeiro_brazil.osm.bin";
        this_city_name = "Rio-de-janeiro";


    } else if (y < (y_start - 12 * y_difference) && y > (y_start - 13 * y_difference)) {
        std::cout << "Saint Helena" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/saint-helena.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/saint-helena.osm.bin";
        this_city_name = "Saint Helena";


    } else if (y < (y_start - 13 * y_difference) && y > (y_start - 14 * y_difference)) {
        std::cout << "singapore" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/singapore.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/singapore.osm.bin";
        this_city_name = "Singapore";


    } else if (y < (y_start - 14 * y_difference) && y > (y_start - 15 * y_difference)) {
        std::cout << "sydney" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/sydney_australia.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/sydney_australia.osm.bin";
        this_city_name = "Sydney";


    } else if (y < (y_start - 15 * y_difference) && y > (y_start - 16 * y_difference)) {
        std::cout << "tehran" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/tehran_iran.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/tehran_iran.osm.bin";
        this_city_name = "Tehran";


    } else if (y < (y_start - 16 * y_difference) && y > (y_start - 17 * y_difference)) {
        std::cout << "tokyo" << std::endl;
        this_map_path = "/cad2/ece297s/public/maps/tokyo_japan.streets.bin";
        osm_path = "/cad2/ece297s/public/maps/tokyo_japan.osm.bin";
        this_city_name = "Tokyo";
    }


    close_map();

    clear_container();

    closeOSMDatabase();

    load_map(this_map_path);

    bool load_osm = loadOSMDatabaseBIN(osm_path);

    if (load_osm)
        std::cout << "load osm success" << std::endl;

    street_graph.number_of_nodes = getNumberOfNodes();

    street_graph.numebr_of_ways = getNumberOfWays();

    street_graph.number_of_relations = getNumberOfRelations();

    //initialize containers when load a new city information
    initialize_feature_id_vector();
    initialize_feature_area_map();
    initialize_OSMNode_Map();
    initialize_OSMWay_Map();
    initialize_OSMRelation_Map();
    initialize_station_info();
    initialize_station_tree();
    initialize_transport_lines();

    draw_new_map();

}

//another version of draw map (don't create button again)

void draw_new_map() {

    t_bound_box initial_coords = t_bound_box(boundary[3], boundary[2], boundary[1], boundary[0]);

    std::string mapName = "Group 81: Mapper";

    t_color backGround(220, 220, 220);

    init_graphics(mapName, backGround);

    set_drawing_buffer(ON_SCREEN);

    set_visible_world(initial_coords);

    std::string message = "Curren City: " + this_city_name;

    update_message(message);

    set_keypress_input(true);

    set_mouse_move_input(true);

    event_loop(act_on_button_press, NULL, NULL, drawscreen);

    t_bound_box old_coords = get_visible_world();


}

//clear all containers

void clear_container() {

    feature_area_set.clear();

    feature_area_map.clear();

    feature_area_vector.clear();

    OSMID_OSMNode.clear();

    OSMID_OSMWay.clear();

    OSMID_OSMRelation.clear();

    OSMID_OSMEntity.clear();

    OSMNode_vector.clear();

    OSMWay_vector.clear();

    OSMRelation_vector.clear();

    OSMEntity_Point_Tags.clear();

    bus_station_location.clear();

    bus_station_name.clear();

    subway_station_location.clear();

    subway_station_name.clear();

    ferry_station_location.clear();

    ferry_station_name.clear();

    transport_line.clear();

    subway_line.clear();

    bus_line.clear();

    ferry_line.clear();

    park_feature.clear();

    beach_feature.clear();

    lake_feature.clear();

    island_feature.clear();

    shoreline_feature.clear();

    river_feature.clear();

    buliding_feature.clear();

    golfcourse_feature.clear();

    greenspace_feature.clear();

    stream_feature.clear();

    unknown_feature.clear();

    bus_station_rtree.clear();

    matched_intersection_position.clear();

    path_segment_intersection.clear();

    print_path_termainal = false;

    path_segment_click.clear();

    path_segment_POI.clear();

    subway_station_rtree.clear();

}

//response when act on intersection button

void act_on_search_intersection(void (*drawscreen_ptr) (void)) {

    matched_intersection_position.clear();
    matched_street_number.clear();

    path_segment_intersection.clear();

    print_path_termainal = false;

    path_segment_click.clear();

    path_segment_POI.clear();

    search_bar_flag = false;

    search_bar_flag2 = false;

    clearscreen();

    drawscreen();

    set_search_engine1();

    event_loop(act_on_choose_street, NULL, act_on_keypress1, drawscreen);

}

void act_on_search_path_intersection(void (*drawscreen_ptr) (void)) {

    path_segment_intersection.clear();

    path_segment_click.clear();

    path_segment_POI.clear();

    print_path_termainal = true;

    std::string line;

    std::string input;

    //do {
    clearscreen();

    drawscreen();

    std::string street1, street2, street3, street4;

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << "               Start Intersection - Intersection Path Search              |\n";

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << " Please Enter the  First Street of First Intersection -> ";

    std::getline(cin, line);

    stringstream lineStream1(line);

    lineStream1 >> input;

    street1 = input;

    lineStream1 >> input;

    while (!lineStream1.fail()) {

        street1 += " " + input;
        lineStream1 >> input;
    }

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << " Please Enter the Second Street of First Intersection -> ";

    std::getline(cin, line);

    stringstream lineStream2(line);

    lineStream2 >> input;

    street2 = input;

    lineStream2 >> input;

    while (!lineStream2.fail()) {
        street2 += " " + input;
        lineStream2 >> input;
    }

    std::vector<unsigned> intersection1 = find_intersection_ids_from_street_names(street1, street2);

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << " The First Intersection is -> " << street1 << " And " << street2 << std::endl;
    std::cout << "----------------------------------------------------------------------------\n";
    std::cout << " " << street1 << " And " << street2 << " Have " << intersection1.size() << " Intersections!" << std::endl;


    std::cout << "----------------------------------------------------------------------------\n";
    std::cout << " Please Enter the  First Street of Second Intersection -> ";

    std::getline(cin, line);

    stringstream lineStream3(line);

    lineStream3 >> input;

    street3 = input;

    lineStream3 >> input;

    while (!lineStream3.fail()) {
        street3 += " " + input;
        lineStream3 >> input;
    }

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << " Please Enter the Second Street of Second Intersection -> ";

    std::getline(cin, line);

    stringstream lineStream4(line);

    lineStream4 >> input;

    street4 = input;

    lineStream4 >> input;

    while (!lineStream4.fail()) {
        street4 += " " + input;
        lineStream4 >> input;
    }

    std::vector<unsigned> intersection2 = find_intersection_ids_from_street_names(street3, street4);

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << " The Second Intersection is -> " << street3 << " And " << street4 << std::endl;
    std::cout << "----------------------------------------------------------------------------\n";
    std::cout << " " << street3 << " And " << street4 << " Have " << intersection2.size() << " Intersections!" << std::endl;
    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << "               Now Searching for Best Path...... \n";

    std::vector< std::vector<unsigned> > best_path;

    unsigned counter = 0;

    for (unsigned i = 0; i < intersection1.size(); i++) {
        for (unsigned j = 0; j < intersection2.size(); j++) {

            best_path.push_back(find_path_between_intersections(intersection1[i], intersection2[j], 0));

            counter++;
        }
    }

    path_start = intersection1[0];

    path_end = intersection2[0];

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << "                   Searching Completed!! \n";
    std::cout << "---------------------------------------------------------------------------\n";

    if (best_path.size() > 0) {

        std::cout << "                       Path Found! \n";

        path_segment_intersection = best_path;

        print_path_description(path_segment_intersection[0]);

    } else
        std::cout << "                 Path Not Found! \n";


    std::cout << "---------------------------------------------------------------------------\n";

    clearscreen();

    drawscreen();

    path_start = intersection1[0];

    path_end = intersection2[0];

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << "               Click on any button to continue\n";
    std::cout << "---------------------------------------------------------------------------\n";

    event_loop(act_on_button_press, NULL, NULL, drawscreen);

}

void act_on_search_path_POI(void (*drawscreen_ptr) (void)) {

    print_path_termainal = true;

    std::string line;

    std::string input;

    path_segment_intersection.clear();

    path_segment_click.clear();

    path_segment_POI.clear();

    clearscreen();

    drawscreen();

    std::string street1, street2, POI;

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << "               Start Intersection - Intersection Path Search              |\n";
    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << " Please Enter the  First Street of First Intersection -> ";

    std::getline(cin, line);

    stringstream lineStream5(line);

    lineStream5 >> input;

    street1 = input;

    lineStream5 >> input;

    while (!lineStream5.fail()) {
        street1 += " " + input;
        lineStream5 >> input;
    }
    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << " Please Enter the Second Street of First Intersection -> ";

    std::getline(cin, line);

    stringstream lineStream6(line);

    lineStream6 >> input;

    street2 = input;

    lineStream6 >> input;

    while (!lineStream6.fail()) {
        street2 += " " + input;
        lineStream6 >> input;
    }

    std::vector<unsigned> intersection1 = find_intersection_ids_from_street_names(street1, street2);

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << " " << street1 << " And " << street2 << " has " << intersection1.size() << " intersection" << std::endl;
    std::cout << "----------------------------------------------------------------------------\n";
    std::cout << " Please Enter the name of point of interest: ";

    std::getline(cin, line);

    stringstream lineStream7(line);

    lineStream7 >> input;

    POI = input;

    lineStream7 >> input;

    while (!lineStream7.fail()) {
        POI += " " + input;
        lineStream7 >> input;
    }

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << "               Now Searching for Best Path...... \n";

    std::vector< std::vector<unsigned> > best_path;

    for (unsigned i = 0; i < intersection1.size(); i++) {

        best_path.push_back(find_path_to_point_of_interest(intersection1[i], POI, 0));
    }

    if (intersection1.size() > 0)
        path_start = intersection1[0];

    if (best_path.size() > 0) {

        path_end = best_path[0].back();
    }

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << "                   Searching Completed!! \n";
    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << "                There are " << best_path.size() << " possible path \n";
    std::cout << "---------------------------------------------------------------------------\n";

    path_segment_POI = best_path;

    clearscreen();

    drawscreen();

    /*std::cout << "                  Continue on Search? (yes/no)\n";
    std::cout << "---------------------------------------------------------------------------\n";*/

    /*std::getline(cin, line);

    stringstream lineStream8(line);

    lineStream8 >> input;*/

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << "               Click on any button to continue\n";
    std::cout << "---------------------------------------------------------------------------\n";

    event_loop(act_on_button_press, NULL, NULL, drawscreen);

    /*} while (input == "yes");

    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << "                   Map Is Reseted! \n";
    std::cout << "---------------------------------------------------------------------------\n";


    act_on_reset(*drawscreen_ptr);*/


}

//response when act on street button

void act_on_search_Street(void (*drawscreen_ptr) (void)) {

    search_bar_flag = false;
    search_bar_flag2 = false;

    matched_intersection_position.clear();
    matched_street_number.clear();

    path_segment_intersection.clear();

    print_path_termainal = false;

    path_segment_click.clear();

    path_segment_POI.clear();

    clearscreen();

    drawscreen();

    set_search_engine2();

    find_this_street.clear();

    event_loop(act_on_choose_street, NULL, act_on_keypress2, drawscreen);

}


//search for an intersection id and return surrounding street names

std::vector<t_point> search_intersection(std::string input) {

    std::string argument1, temp, argument2;

    std::vector<std::string> argument_vector;

    stringstream inputStream(input);

    std::vector<t_point> intersection_position_result;

    inputStream >> argument1;

    inputStream >> temp;

    while (temp != "And") {

        argument1 = argument1 + " " + temp;

        inputStream >> temp;

    }

    inputStream >> argument2;

    inputStream >> temp;

    while (!inputStream.fail()) {

        argument2 = argument2 + " " + temp;

        inputStream >> temp;
    }

    //std::cout << "*** argument 1 is: --->  " << argument1 << std::endl;

    //std::cout << "*** argument 2 is: --->  " << argument2 << std::endl;


    vector<unsigned> intersectionID = find_intersection_ids_from_street_names(argument1, argument2);

    for (unsigned j = 0; j < intersectionID.size(); j++) {

        intersection_position_result.push_back(street_graph.intersection_position_vector[intersectionID[j]]);

    }

    return (intersection_position_result);
}

//response when act on find button

void act_on_find_intersection(void (*drawscreen_ptr) (void)) {

    search_bar_flag = false;
    search_bar_flag2 = false;

    matched_intersection_position.clear();
    matched_street_number.clear();

    path_segment_intersection.clear();

    print_path_termainal = false;

    path_segment_click.clear();

    path_segment_POI.clear();

    drawscreen();

    highlight_intersection();

    event_loop(act_on_intersection_info, NULL, NULL, act_on_find_intersection_loop);

}

void act_on_find_intersection_loop(void) {

    clearscreen();

    drawscreen();

    highlight_intersection();

}

//response when act on POI button

void act_on_find_POI(void (*drawscreen_ptr) (void)) {

    search_bar_flag = false;
    search_bar_flag2 = false;

    matched_intersection_position.clear();
    matched_street_number.clear();

    path_segment_intersection.clear();

    print_path_termainal = false;

    path_segment_click.clear();

    path_segment_POI.clear();

    drawscreen();

    highlight_POI();

    event_loop(act_on_POI_info, NULL, NULL, act_on_find_POI_loop);

}

//return the (closet) POI name when click a position on map

void act_on_POI_info(float x, float y, t_event_buttonPressed event) {

    t_bound_box initial_coords = t_bound_box(boundary[3], boundary[2], boundary[1], boundary[0]);

    t_point target_xy = t_point(x, y);

    LatLon target_latlon = XY_LatLon_Converter(target_xy);

    unsigned closest_POI = find_closest_point_of_interest(target_latlon);

    t_point closest_POI_position = street_graph.POI_position_vector[closest_POI];

    std::string POI_name = getPointOfInterestName(closest_POI);

    t_bound_box message_display_box = get_visible_world();

    float left_offset2 = message_display_box.get_width() * left_offset2_const;
    float left_coordinate2 = message_display_box.left() + left_offset2;
    float right_offset2 = message_display_box.get_width() * right_offset2_const;
    float right_coordinate2 = message_display_box.right() - right_offset2;
    float top_offset2 = message_display_box.get_height() * top_offset2_const;
    float top_coordinate2 = message_display_box.top() - top_offset2;
    float bottom_offset2 = message_display_box.get_height() * bottom_offset2_const;
    float bottom_coordinate2 = message_display_box.bottom() + bottom_offset2;

    setcolor(255, 140, 0);
    fillrect(left_coordinate2, bottom_coordinate2, right_coordinate2, top_coordinate2);

    t_bound_box input_center(left_coordinate2, bottom_coordinate2, right_coordinate2, top_coordinate2);
    setcolor(BLACK);
    setfontsize(16);
    settextrotation(0);
    drawtext_in(input_center, POI_name);

}

void act_on_find_POI_loop(void) {

    clearscreen();

    drawscreen();

    highlight_POI();
}

//response when act on transportation button

void act_on_find_transportation(void (*drawscreen_ptr) (void)) {

    search_bar_flag = false;
    search_bar_flag2 = false;

    matched_intersection_position.clear();
    matched_street_number.clear();

    path_segment_intersection.clear();

    print_path_termainal = false;

    path_segment_click.clear();

    path_segment_POI.clear();

    drawscreen();

    draw_bus_staions();

    draw_subway_staions();

    draw_transport_lines("subway");

    draw_transport_lines("ferry");

    event_loop(act_on_station_info, NULL, NULL, act_on_find_transportation_loop);
}

//return the (closet) station name when click a position on map

void act_on_station_info(float x, float y, t_event_buttonPressed event) {

    t_bound_box initial_coords = t_bound_box(boundary[3], boundary[2], boundary[1], boundary[0]);

    t_point target_xy = t_point(x, y);

    t_point closest_station_position = find_closest_bus_station(target_xy);

    std::string station_name = get_bus_station_name(closest_station_position);

    t_bound_box message_display_box = get_visible_world();

    float left_offset2 = message_display_box.get_width() * left_offset2_const;
    float left_coordinate2 = message_display_box.left() + left_offset2;
    float right_offset2 = message_display_box.get_width() * right_offset2_const;
    float right_coordinate2 = message_display_box.right() - right_offset2;
    float top_offset2 = message_display_box.get_height() * top_offset2_const;
    float top_coordinate2 = message_display_box.top() - top_offset2;
    float bottom_offset2 = message_display_box.get_height() * bottom_offset2_const;
    float bottom_coordinate2 = message_display_box.bottom() + bottom_offset2;
    setcolor(255, 140, 0);
    fillrect(left_coordinate2, bottom_coordinate2, right_coordinate2, top_coordinate2);

    t_bound_box input_center(left_coordinate2, bottom_coordinate2, right_coordinate2, top_coordinate2);
    setcolor(BLACK);
    setfontsize(16);
    settextrotation(0);
    drawtext_in(input_center, station_name);

}

void act_on_find_transportation_loop(void) {

    clearscreen();

    drawscreen();

    draw_bus_staions();

    draw_subway_staions();
}

//return bus station name for a bus station position

std::string get_bus_station_name(t_point station_position) {

    std::string station_name;

    for (unsigned i = 0; i < bus_station_location.size(); i++) {

        if (bus_station_location[i].x == station_position.x && bus_station_location[i].y == station_position.y)
            station_name = bus_station_name[i];
    }

    return station_name;
}

//return subway station name for a subway station position

std::string get_subway_station_name(t_point station_position) {

    std::string station_name;

    for (unsigned i = 0; i < subway_station_location.size(); i++) {

        if (subway_station_location[i].x == station_position.x && subway_station_location[i].y == station_position.y)
            station_name = subway_station_name[i];
    }

    return station_name;
}

//draw street names within current screen boundary

void draw_street_names() {

    std::unordered_map<std::string, std::string> check_street_name;

    t_bound_box current_bound = get_visible_world();

    for (unsigned i = 0; i < segment_segment_position_vector.size(); i++) {

        float check_bound_x = 0.5 * (segment_segment_position_vector[i][segment_segment_position_vector[i].size() - 1].x + segment_segment_position_vector[i][0].x);

        float check_bound_y = 0.5 * (segment_segment_position_vector[i][segment_segment_position_vector[i].size() - 1].y + segment_segment_position_vector[i][0].y);

        if (check_bound_x > current_bound.left() && check_bound_x < current_bound.right() && check_bound_y > current_bound.bottom() && check_bound_y < current_bound.top()) {

            std::string street_name = getStreetName(getStreetSegmentInfo(i).streetID);

            bool check_oneway = getStreetSegmentInfo(i).oneWay;

            if (street_name != "<unknown>") {

                if (check_street_name.find(street_name) == check_street_name.end()) {

                    for (unsigned j = 0; j < segment_segment_position_vector[i].size(); j++) {

                        if (segment_segment_position_vector[i].size() == 2) {//no curve point

                            float angle = atan((segment_segment_position_vector[i][0].y - segment_segment_position_vector[i][1].y) / (segment_segment_position_vector[i][0].x - segment_segment_position_vector[i][1].x)) / DEG_TO_RAD;

                            float xc = 0.5 * (segment_segment_position_vector[i][1].x + segment_segment_position_vector[i][0].x);

                            float yc = 0.5 * (segment_segment_position_vector[i][1].y + segment_segment_position_vector[i][0].y);

                            settextrotation(angle);

                            setfontsize(7);

                            if (zoom_level > zoom_level_5)
                                setfontsize(18);

                            setcolor(BLACK);

                            drawtext(xc, yc, street_name, fabs(segment_segment_position_vector[i][1].x - segment_segment_position_vector[i][0].x), fabs(segment_segment_position_vector[i][1].y - segment_segment_position_vector[i][0].y));

                            check_street_name.insert(std::make_pair(street_name, "drew"));

                        } else {
                            std::pair<t_point, t_point> t_point_pair = find_longest_segment_segment(segment_segment_position_vector[i]);

                            float angle = atan((t_point_pair.first.y - t_point_pair.second.y) / (t_point_pair.first.x - t_point_pair.second.x)) / DEG_TO_RAD;

                            float xc = 0.5 * (t_point_pair.second.x + t_point_pair.first.x);

                            float yc = 0.5 * (t_point_pair.second.y + t_point_pair.first.y);

                            settextrotation(angle);

                            setfontsize(7);

                            if (zoom_level >= zoom_level_7)
                                setfontsize(20);

                            setcolor(BLACK);

                            drawtext(xc, yc, street_name, fabs(t_point_pair.second.x - t_point_pair.first.x), fabs(t_point_pair.second.y - t_point_pair.first.y));

                            check_street_name.insert(std::make_pair(street_name, "drew"));
                        }
                    }
                }
            }
        }
    }

    check_street_name.clear();
}

//find the longest street segment's segment

std::pair<t_point, t_point> find_longest_segment_segment(std::vector<t_point> this_segment) {

    t_point t_point1, t_point2;

    float longest_length;
    float temp_length;

    for (unsigned i = 1; i < this_segment.size(); i++) {

        t_point temp_t_point1, temp_t_point2;

        if (i == 1) {
            temp_t_point1 = this_segment[0];
            temp_t_point2 = this_segment[1];

            longest_length = find_distance_between_t_point(temp_t_point1, temp_t_point2);
        } else {

            temp_t_point1 = this_segment[i - 1];
            temp_t_point2 = this_segment[i];

            temp_length = find_distance_between_t_point(temp_t_point1, temp_t_point2);

            if (temp_length > longest_length) {
                longest_length = temp_length;
                t_point1 = temp_t_point1;
                t_point2 = temp_t_point2;
            }
        }
    }

    std::pair<t_point, t_point> result = std::make_pair(t_point1, t_point2);

    return (result);
}

//calculate distance between two t_point

float find_distance_between_t_point(t_point t_point1, t_point t_point2) {

    float distance = sqrt(pow(t_point1.x - t_point2.x, 2) + pow(t_point1.y - t_point2.y, 2));

    return distance;
}


//search for street suggestions for auto completion

std::vector<std::string> search_street_helper(std::string input) {

    std::unordered_map<std::string, unsigned> check_inserted;

    std::vector<std::string> matched_street_name;

    unsigned counter;

    for (unsigned i = 0; i < street_graph.street_name_vector.size(); i++) {

        counter = input.size();

        for (unsigned j = 0; j < input.size(); j++) {

            if (input[j] == street_graph.street_name_vector[i][j]) {
                counter--;
            }
        }

        if (counter == 0) {
            matched_street_name.push_back(street_graph.street_name_vector[i]);
        }
        
    }

    return matched_street_name;
}