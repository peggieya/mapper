#ifndef FIND_PATH_H
#define FIND_PATH_H

#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "structure.h"
#include "m1.h"
#include "m2_helper.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <stack> 
#include <vector>
#include <deque>
#include <queue> 
#include <climits>
#include <list>

#define HI_SPD_LIM 120

struct cell {
    unsigned cell_index;

    float distance_from_start = INT_MAX;

    unsigned this_edge_index;

    std::vector<unsigned> path_intersection_id;

    std::vector<unsigned> path_segment_id;
};

struct intersection {
    int index = -1;

    int from = -1;

    int through = -1;

    float cost = INT_MAX;
};

struct POIcell {
    unsigned POI_id;

    std::string POI_name;

    POI_position position;

    point POI_coord;


};


class compare_cell {
public:

    bool const operator()(intersection &cellX, intersection &cellY) {
        return (cellX.cost > cellY.cost);
    }
};

class compare_POI {
public:

    bool const operator()(POI_info &POI1, POI_info &POI2) {
        return (POI1.distance < POI2.distance);
    }
};

class compare_POI2 {
public:

    bool const operator()(POI_info &POI1, POI_info &POI2) {
        return (POI1.distance < POI2.distance);
    }
};

class path_cell {
private:

public:

    std::priority_queue<intersection, std::vector<intersection>, compare_cell> cell_queue;
};

std::vector<unsigned> back_trace_path(const unsigned intersect_id_start, const unsigned intersect_id_end, std::vector<intersection> intersection_trace);

//std::vector <POI_info> find_POI(std::string point_of_interest_name, unsigned intersect_id_start);

std::vector<unsigned> find_path_to_point_of_interest_helper(const unsigned intersect_id_start, const double turn_penalty, std::vector<unsigned> POI_intersection);

#endif /* FIND_PATH_H */

