#include "m2.h"
#include "m1.h"
#include "m2_helper.h"
#include "StreetsDatabaseAPI.h"
#include "structure.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <ctime>
#include <time.h>
#include "m3.h"
#include "find_path.h"
#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

extern streetGraph street_graph;

extern std::vector<unsigned> this_path;

extern std::vector<std::vector<intersection_edge_info> > connected_intersections;

//extern std::vector<unsigned> this_path_intersection;


// Returns the time required to travel along the path specified, in seconds. 
// The path is given as a vector of street segment ids, and this function 
// can assume the vector either forms a legal path or has size == 0.
// The travel time is the sum of the length/speed-limit of each street 
// segment, plus the given turn_penalty (in seconds) per turn implied by the path. 
// A turn occurs when two consecutive street segments have different street IDs.

double compute_path_travel_time(const std::vector<unsigned>& path,
        const double turn_penalty) {

    double travelTime = 0;
    unsigned numOfTurns = 0;

    if (path.size() == 0) {
        return travelTime;
    } else {

        for (unsigned i = 0; i < path.size(); i++) {

            travelTime += find_street_segment_travel_time(path[i]);

            if (i > 0) {

                if (getStreetSegmentInfo(path[i]).streetID != getStreetSegmentInfo(path[i - 1]).streetID)

                    numOfTurns++;
            }
        }
        return (travelTime + numOfTurns * turn_penalty);
    }
}

/******************************************************************************
 * Pass in two intersection id and find the quickest way between them.        *
 * Use Dijkstra to explore the map (path) and keeping checking the new        *
 * start point (shortest path in the queue) until the reach the end point.    *
 * Back trace the segment to reconstruct the path.                            *
 ******************************************************************************/

std::vector<unsigned> find_path_between_intersections(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty) {

    std::vector<intersection> intersection_trace;

    intersection_trace.resize(street_graph.number_of_intersections);

    intersection_trace[intersect_id_start].index = intersect_id_start;

    intersection_trace[intersect_id_start].cost = 0;

    path_cell find_path;

    std::vector<unsigned> best_path;

    find_path.cell_queue.push(intersection_trace[intersect_id_start]);

    while (!find_path.cell_queue.empty()) {

        intersection top_cell = find_path.cell_queue.top();

        find_path.cell_queue.pop();

        if (top_cell.index == (int) intersect_id_end) {

            best_path = back_trace_path(intersect_id_start, intersect_id_end, intersection_trace);

            return best_path;
        }

        if (top_cell.cost <= intersection_trace[top_cell.index].cost) {

            std::vector<intersection_edge_info> connected_intersection_info = connected_intersections[top_cell.index];

            for (unsigned i = 0; i < connected_intersection_info.size(); i++) {

                unsigned to = connected_intersection_info[i].connected_intersection;

                float cost_temp = top_cell.cost + connected_intersection_info[i].travlle_time_to_next_intersection;

                if (top_cell.from != -1) {

                    unsigned streetID1 = getStreetSegmentInfo(top_cell.through).streetID;

                    unsigned streetID2 = getStreetSegmentInfo(connected_intersection_info[i].edge_index).streetID;

                    if (streetID1 != streetID2)
                        cost_temp += turn_penalty;
                }

                if (intersection_trace[to].cost > cost_temp) {

                    intersection_trace[to].cost = cost_temp;

                    intersection_trace[to].index = connected_intersection_info[i].connected_intersection;

                    intersection_trace[to].from = top_cell.index;

                    intersection_trace[to].through = connected_intersection_info[i].edge_index;

                    find_path.cell_queue.push(intersection_trace[to]);
                }
            }
        }
    }

    return best_path;
}

/*********************************************************************************
 * User input a POI name, find closet intersection to each POI match the input.  *
 * Pass the intersections to find_path_to_point_of_interest_helper               *
 *********************************************************************************/

std::vector<unsigned> find_path_to_point_of_interest(const unsigned intersect_id_start,
        const std::string point_of_interest_name,
        const double turn_penalty) {

    double mintime = INT_MAX;

    double cprtime = 0;

    double time_ref;

    double time1;

    std::vector <unsigned> route;

    std::vector <unsigned> route_ref;

    std::vector <unsigned> returnedroute;

    returnedroute.clear();

    std::vector <unsigned> POI_intersection;

    for (unsigned i = 0; i < street_graph.POI_names.size(); i++) {

        if (street_graph.POI_names[i] == point_of_interest_name) {

            POI_intersection.push_back(street_graph.POI_info_vector[i].cloest_intersection);
        }
    }

    if (POI_intersection.size() == 0) return route;

    route = find_path_to_point_of_interest_helper(intersect_id_start, turn_penalty, POI_intersection);

    return route;
}


