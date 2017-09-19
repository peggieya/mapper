#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "structure.h"
#include "graphics.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <math.h>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/foreach.hpp>

#ifndef M2_HELPER_H
#define M2_HELPER_H

struct feature_info {
    unsigned feature_index;
    
    FeatureType feature_type;
    
    int feature_point;
    
    std::vector<t_point> feature_point_vector;
    
    bool is_closed;
    
    t_color color;
    
    float area_length;
};

struct POI_info {
    
    unsigned POI_index;
    
    std::string POI_type;
    
    std::string POI_name;
    
    t_point POI_position;

    float distance = INT_MAX;

    unsigned cloest_intersection;

};

struct pathInfo {
    
    std::string name;
    
    double length;
    
    double time;
    
    unsigned from;
    
    unsigned to;
};

struct path_intersection_info{
    
    unsigned intersectionID;
    
    t_point intersection_pos;
    
    bool turn;
    
};

void clear_container();

void draw_new_map();

void draw_all_street_segment(void);

void draw_highway(void);

void draw_main_street(void);

void draw_local_street(void);

void drawscreen(void);

t_point LatLon_XY_converter(LatLon input);

LatLon XY_LatLon_Converter(t_point XY);

void draw_feature(void);

void draw_segment(unsigned segment_id);

void initialize_feature_area_map();

void search_intersection_loop();

std::vector<t_point> search_intersection(std::string input);

void initialize_vocabulary_map();

std::string search(std::string input);

unsigned find_zoom_level_by_height();

unsigned find_zoom_level_by_width();

void zoom_level_1_draw(void);
void zoom_level_2_draw(void);
void zoom_level_3_draw(void);
void zoom_level_4_draw(void);
void zoom_level_5_draw(void);
void zoom_level_6_draw(void);
void zoom_level_7_draw(void);
void zoom_level_8_draw(void);
void zoom_level_9_draw(void);
void zoom_level_10_draw(void);

void draw_natural_feature(void);

float get_poly_area(unsigned numOfPoint, std::vector<t_point> relVertex);

void draw_feature_layer();

void draw_feature_by_index(std::vector<unsigned> feature_index);

void initialize_feature_id_vector();

void initialize_POI_info_vector();

void draw_feature_vector(std::vector<feature_info> feature_vector);

void draw_feature_name(std::vector<feature_info> feature_vector);

void draw_city_name();

void draw_POI_icon_by_type();

void initialize_OSMNode_Map();

void initialize_OSMWay_Map();

void initialize_OSMRelation_Map();

void initialize_station_info();

void initialize_transport_lines();

void initialize_station_info_heler(std::string transport, const OSMNode* this_node, std::unordered_map<std::string, std::string> check_tags);

void initialize_transport_lines_helper(std::string transport, const OSMWay* this_way);

void draw_bus_staions();

void draw_ferry_staions();

void draw_subway_staions();

void act_on_mouse_move(float x, float y);

void act_on_button_press(float x, float y, t_event_buttonPressed event);

void act_on_keypress1(char c, int keysym);

void act_on_keypress2(char c, int keysym);

void highlight_matched_intersection();

void highlight_matched_street();

void highlight_path_intersection();

void highlight_path_POI();

void highlight_path_click();

void hightlight_path_terminal();

void print_path_description(std::vector<unsigned> path);

void act_on_search_intersection(void (*drawscreen_ptr) (void));

void act_on_search_Street(void (*drawscreen_ptr) (void));

void act_on_find_intersection(void (*drawscreen_ptr) (void));

void act_on_find_POI(void (*drawscreen_ptr) (void));

void act_on_find_transportation(void (*drawscreen_ptr) (void));

void draw_transport_lines(std::string transport);

void act_on_intersection_info(float x, float y, t_event_buttonPressed event);

void act_on_find_intersection_loop(void);

void highlight_intersection();

void highlight_POI();

void act_on_POI_info(float x, float y, t_event_buttonPressed event);

void act_on_find_POI_loop(void);

void set_search_engine1();

void set_search_engine2();

void highlight_this_segment(unsigned segment_id);

void act_on_reset(void (*drawscreen_ptr) (void));

void act_on_help(void (*drawscreen_ptr) (void));

void act_on_city(void (*drawscreen_ptr) (void));

void act_on_input(char c, int keysym);

void act_on_choose_city(float x, float y, t_event_buttonPressed event);

void initialize_station_tree();

t_point find_closest_bus_station(t_point my_position);

void act_on_station_info(float x, float y, t_event_buttonPressed event);

void act_on_choose_street(float x, float y, t_event_buttonPressed event);

void act_on_find_transportation_loop(void);

std::string get_bus_station_name(t_point station_position);

std::string get_subway_station_name(t_point station_position);

void draw_street_names();

std::pair<t_point, t_point> find_longest_segment_segment(std::vector<t_point> this_segment);

float find_distance_between_t_point(t_point t_point1, t_point t_point2);

std::vector<std::string> search_street_helper(std::string input);

void set_suggest_box(std::vector<std::string> suggest_street);

void draw_search_bar(bool flag);

void draw_search_bar2(bool flag);

void set_search_bar();

void set_search_bar2();

void act_on_search_path_intersection(void (*drawscreen_ptr) (void));

void act_on_search_path_POI(void (*drawscreen_ptr) (void));

void act_on_keypress3(char c, int keysym);

void act_on_keypress4(char c, int keysym);

void set_search_engine_path_intersection();


#endif /* M2_HELPER_H */



