#include "find_path.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "structure.h"
#include "m2_helper.h"
#include "m1.h"
#include "m3.h"
#include "m2.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <math.h>
#include <deque>
#include <queue> 

//path_cell explore_map;

extern streetGraph street_graph;

extern std::vector<std::vector<intersection_edge_info> > connected_intersections;

double timeScore = 0;


/******************************************************************************
 * Back trace the intersection nodes in cloest queue set to find the path     *
 ******************************************************************************/

std::vector<unsigned> back_trace_path(const unsigned intersect_id_start, const unsigned intersect_id_end, std::vector<intersection> intersection_trace) {

    std::vector<unsigned> back_trace;

    std::vector<unsigned> back_trace_temp;

    unsigned end_id = intersect_id_end;

    while (end_id != intersect_id_start) {

        back_trace_temp.push_back(intersection_trace[end_id].through);

        end_id = intersection_trace[end_id].from;
    }

    for (int i = back_trace_temp.size() - 1; i >= 0; i--) {

        back_trace.push_back(back_trace_temp[i]);

    }

    return back_trace;
}



/********************************************************************************
 *  A helper function to find a path between an intersection and a POI name     *
 *  - Take start intersection id and a vector of intersection index             *
 *    (closest intersection to each POI that match the input)                   *
 *                                                                              *
 *  - Use Dijkstra to explore the map (path) and keeping checking the new       *
 *    start point in one in the POI_intersection_vector (POI_intersection)      *
 *                                                                              *
 *  - If (new start point) == (one term in POI_intersection_vector)             *
 *    The shortest path is found                                                *  
 *                                                                              *    
 *  - Back trace the path and return the ordered segment                        *   
 ********************************************************************************/

std::vector<unsigned> find_path_to_point_of_interest_helper(const unsigned intersect_id_start, const double turn_penalty, std::vector<unsigned> POI_intersection) {
    
    std::vector<intersection> intersection_trace;

    intersection_trace.resize(street_graph.number_of_intersections);

    intersection_trace[intersect_id_start].index = intersect_id_start;

    intersection_trace[intersect_id_start].cost = 0;

    //find_path -> stores a priority queue that stores intersection cells in order
    //(based on the distance between the current intersection cell and the start point) 
    path_cell find_path;

    std::vector<unsigned> best_path;

    //put the start intersection cell into the priority queue
    find_path.cell_queue.push(intersection_trace[intersect_id_start]);

    while (!find_path.cell_queue.empty()) {

        //store the top of priority queue in a temp cell before pop
        intersection top_cell = find_path.cell_queue.top();

        find_path.cell_queue.pop();

        //check if the temp top intersection cell index is one matched POI 
        //YES -> back trace the path
        for (unsigned i = 0; i < POI_intersection.size(); i++) {
            
            if (top_cell.index == POI_intersection[i]) {

                unsigned intersect_id_end = POI_intersection[i];
                
                best_path = back_trace_path(intersect_id_start, intersect_id_end, intersection_trace);

                return best_path;
            }
        }

        if (top_cell.cost <= intersection_trace[top_cell.index].cost) {

            std::vector<intersection_edge_info> connected_intersection_info = connected_intersections[top_cell.index];

            //explore valid segments around a intersection, and store them in priority queue
            for (unsigned i = 0; i < connected_intersection_info.size(); i++) {

                unsigned to = connected_intersection_info[i].connected_intersection;

                float cost_temp = top_cell.cost + connected_intersection_info[i].travlle_time_to_next_intersection;

                //check if street id are different, add turn penalty
                if (top_cell.from != -1) {

                    unsigned streetID1 = getStreetSegmentInfo(top_cell.through).streetID;

                    unsigned streetID2 = getStreetSegmentInfo(connected_intersection_info[i].edge_index).streetID;

                    if (streetID1 != streetID2)
                        cost_temp += turn_penalty;
                }

                //initialize queue members
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















/*-------------------------------------------------------------------------------*/

/********************************************************
 *    Some unused functions, maybe useful later         *
 ********************************************************/

/*std::vector<unsigned> back_trace;

std::vector<unsigned> back_trace_temp;

unsigned temp_end = intersect_id_end;

while (temp_end != intersect_id_start) {

    back_trace_temp.push_back(intersection_trace[temp_end].through);

    temp_end = intersection_trace[temp_end].from;
}

for (int i = back_trace_temp.size() - 1; i >= 0; i--) {

    back_trace.push_back(back_trace_temp[i]);

}

return back_trace;*/
//}

/*std::vector<unsigned> find_path_intersection(cell start_cell) {



}*/




/*std::vector<unsigned> return_path(cell& end_cell){
    
    return end_cell.path_intersection_id;
    
}*/

//std::vector<cell> path_explore(std::vector<cell> one_path) {
//
//    std::vector<cell> second_path;
//
//    weight_reference = compute_path_travel_time(explore_map.finding_path_intersection, 1);
//
//    return second_path;
//}

/*void reset_start_point() {

    explore_map.cell_queue.pop_back();

    unsigned new_start_id = (explore_map.cell_queue.end()->cell_index);

    map_explore(new_start_id);
}

cell* find_min_distance_cell_from_start() {

    cell* min_cell = NULL;

    if (explore_map.cell_queue.size() != 0) {

        double shortest_distance_from_start = explore_map.cell_queue[0].distance_from_start;

        double shortest_distance_from_start_temp = 0;

        unsigned result_cell_index = 0;


        for (unsigned i = 1; i < explore_map.cell_queue.size(); i++) {

            shortest_distance_from_start_temp = explore_map.cell_queue[i].distance_from_start;

            if (shortest_distance_from_start_temp < shortest_distance_from_start)
                result_cell_index = i;

        }

        min_cell = explore_map.cell_queue[i];
    }

    return min_cell;

}*/



/*std::vector<unsigned> AStarSearch(unsigned source, unsigned dest, const double turn_penalty) {

    //std::cout << "-------------- A Star Searching Start --------------------" << std::endl;

    std::vector<unsigned> path;

    intersection_graph.resize(street_graph.number_of_intersections);

    if (source == dest || source >= street_graph.number_of_intersections || dest >= street_graph.number_of_intersections || source < 0 || dest < 0) {

        return path;
    }

    cleanGraph();

    typedef std::pair<double, unsigned> NodePair;

    std::priority_queue< NodePair, std::vector<NodePair>, std::greater<NodePair> > openSet;

    intersection_graph[source].interID = source;

    intersection_graph[source].gScore = 0;

    intersection_graph[source].hScore = findConsistentH(source, dest);

    intersection_graph[source].fScore = intersection_graph[source].hScore;

    NodePair insertToOpen(intersection_graph[source].fScore, source);

    openSet.push(insertToOpen);

    unsigned currentID = source;

    GNode* currentNode;

    currentNode = &(intersection_graph[source]);

    bool found = false;

    while (!openSet.empty() && !found) {

        currentID = openSet.top().second;

        if (currentID == dest) {

            found = true;

            break;
        }

        currentNode = &(intersection_graph[currentID]);

        currentNode->visited = true;

        NodePair top_temp = openSet.top();

        openSet.pop();

        currentNode->connections = connected_intersections[currentNode->interID];

        /*std::cout << "connection size:  "<< currentNode->connections.size() <<std::endl;
        
        for(unsigned i = 0 ; i < currentNode->connections.size(); i++){
            
            std::cout<< "\n\nFrom this intersection " << currentID << std::endl;
            std::cout<< "To this intersection:  " << currentNode->connections[i].interID << std::endl;
            std::cout<< "Segment index: " << currentNode->connections[i].segID << std::endl;
            std::cout<< "Segment weight: " << currentNode->connections[i].deltaTime << std::endl;
            
        }*/

/*std::vector<InterPath> neighbours = currentNode->connections;

for (int i = 0; i < (int) neighbours.size(); i++) {

    unsigned interID = neighbours[i].interID;

    GNode* thisNode;

    thisNode = &(intersection_graph[interID]);

    //if (!thisNode->visited) {

    double tentativeG = currentNode->gScore + neighbours[i].deltaTime;

    StreetSegmentInfo secondSeg = getStreetSegmentInfo(currentNode->arrivedFrom);

    if (neighbours[i].streetOfSeg != secondSeg.streetID && currentNode->interID != source)

        tentativeG += turn_penalty;

    double tentativeH = findConsistentH(interID, dest);

    double tentativeF = tentativeG + tentativeH;

    if (tentativeF < thisNode->fScore) {

        thisNode->gScore = tentativeG;

        thisNode->hScore = tentativeH;

        thisNode->fScore = tentativeF;

        thisNode->arrivedFrom = neighbours[i].segID;
                
        thisNode->visited = true;

        NodePair insertToOpen(tentativeF, interID);

        openSet.push(insertToOpen);
    }
    //}
    thisNode = NULL;
}
}

currentNode = NULL;

for (unsigned i = 0; i < intersection_graph.size(); i++) {

if (intersection_graph[i].visited) {

    std::cout << "\n\nsegment from:   " << i << std::endl;
    std::cout << "segment end:   " << intersection_graph[i].interID << std::endl;
    std::cout << "segment index:   " << intersection_graph[i].arrivedFrom << std::endl;
}


}

if (found)
path = retrace(source, dest, intersection_graph);

return path;
}*/

/*void cleanGraph() {
    for (int i = 0; i < (int) intersection_graph.size(); i++) {
        intersection_graph[i].arrivedFrom = 0;
        intersection_graph[i].visited = false;
        intersection_graph[i].fScore = INT_MAX;
    }
}*/

/*double findConsistentH(unsigned current, unsigned dest) {

    LatLon currentLoc = getIntersectionPosition(current);

    LatLon destLoc = getIntersectionPosition(dest);

    double magSQ = helper_approximate_mag_sq(currentLoc, destLoc);

    double distanceInKm = sqrt(magSQ) * EARTH_RADIUS_IN_METERS / 1000;

    double time = distanceInKm / HI_SPD_LIM * 60;

    return time;untitled folder 2
}

double helper_approximate_mag_sq(LatLon point1, LatLon point2) {

    double approximate_magnitude_squared;

    double x1, x2, y1, y2;

    x1 = point1.lat() * DEG_TO_RAD * cos(street_graph.avgLat);

    x2 = point2.lon() * DEG_TO_RAD * cos(street_graph.avgLat);

    y1 = point1.lat() * DEG_TO_RAD;

    y2 = point2.lat() * DEG_TO_RAD;

    approximate_magnitude_squared = pow(y2 - y1, 2) + pow(x2 - x1, 2);

    return approximate_magnitude_squared;
}*/

/*std::vector<unsigned> retrace(unsigned source, unsigned dest, std::vector<GNode> trace) {

    std::list<unsigned> setup;

    unsigned navigator = dest;

    while (navigator != source) {

        unsigned segment = intersection_graph[navigator].arrivedFrom;

        setup.push_front(segment);

        StreetSegmentInfo datasheet = getStreetSegmentInfo(segment);

        if (datasheet.from == navigator) {

            navigator = datasheet.to;

        } else if (datasheet.to == navigator) {

            navigator = datasheet.from;

        } else
            assert(true);
    }

    std::vector<unsigned> path;

    for (auto iter = setup.begin(); iter != setup.end(); ++iter) {

        path.push_back(*iter);
    }

    return path;
}*/

//std::vector<GNode> intersection_graph;

/*std::vector<unsigned> find_intersection_path(cell start_cell, const unsigned end_id, const double turn_penalty) {

    std::vector<intersection> intersection_trace;

    unsigned start_id = start_cell.cell_index;

    intersection_trace.resize(street_graph.number_of_intersections);

    std::vector<unsigned> shortest_path;

    path_cell find_path;

    find_path.cell_queue.push(start_cell);

    std::vector<intersection_edge_info> connected_intersection_edge = connected_intersections[start_id];

    if (connected_intersection_edge.size() == 0)
        return shortest_path;

    for (unsigned i = 0; i < connected_intersection_edge.size(); i++) {

        cell connected_cell;

        connected_cell.cell_index = connected_intersection_edge[i].connected_intersection;

        connected_cell.distance_from_start = connected_intersection_edge[i].travlle_time_to_next_intersection;

        connected_cell.this_edge_index = connected_intersection_edge[i].edge_index;

        find_path.cell_queue.push(connected_cell);
    }

    cell top_cell = find_path.cell_queue.top();

    find_path.cell_queue.pop();

    if (find_path.cell_queue.empty())
        return shortest_path;

    if (intersection_trace[find_path.cell_queue.top().cell_index].cost > find_path.cell_queue.top().distance_from_start) {

        intersection_trace[find_path.cell_queue.top().cell_index].cost = find_path.cell_queue.top().distance_from_start;

        intersection_trace[find_path.cell_queue.top().cell_index].from = top_cell.cell_index;

        intersection_trace[find_path.cell_queue.top().cell_index].through = find_path.cell_queue.top().this_edge_index;
    }

    while (!find_path.cell_queue.empty()) {

        //std::cout << "queue size =   " << find_path.cell_queue.size() << std::endl;

        cell new_start_cell = find_path.cell_queue.top();

        if (new_start_cell.cell_index == end_id) {

            shortest_path = back_trace_path(start_cell.cell_index, end_id, intersection_trace);

            return shortest_path;
        }

        connected_intersection_edge = connected_intersections[new_start_cell.cell_index];

        if (connected_intersection_edge.size() != 0) {

            for (unsigned i = 0; i < connected_intersection_edge.size(); i++) {

                cell connected_cell;

                connected_cell.cell_index = connected_intersection_edge[i].connected_intersection;

                connected_cell.distance_from_start = connected_intersection_edge[i].travlle_time_to_next_intersection + start_cell.distance_from_start;

                connected_cell.distance_from_start += new_start_cell.distance_from_start;

                unsigned cell_street_id1 = getStreetSegmentInfo(connected_cell.cell_index).streetID;

                unsigned cell_street_id2 = getStreetSegmentInfo(new_start_cell.cell_index).streetID;

                if (cell_street_id1 != cell_street_id2) {

                    connected_cell.distance_from_start += turn_penalty;
                }

                connected_cell.this_edge_index = connected_intersection_edge[i].edge_index;

                find_path.cell_queue.push(connected_cell);
            }

            cell top_cell = find_path.cell_queue.top();

            find_path.cell_queue.pop();

            if (find_path.cell_queue.empty())
                return shortest_path;

            if (intersection_trace[find_path.cell_queue.top().cell_index].cost > find_path.cell_queue.top().distance_from_start) {

                intersection_trace[find_path.cell_queue.top().cell_index].cost = find_path.cell_queue.top().distance_from_start;

                intersection_trace[find_path.cell_queue.top().cell_index].from = top_cell.cell_index;

                intersection_trace[find_path.cell_queue.top().cell_index].through = find_path.cell_queue.top().this_edge_index;
            }
        } else {

            find_path.cell_queue.pop();
        }
    }

    return shortest_path;
}*/


/*std::vector <POI_info> find_POI(std::string point_of_interest_name) {

    std::vector <POI_info> close_POI;

    for (unsigned i = 0; i < street_graph.POI_names.size(); i++) {

        if (street_graph.POI_names[i] == point_of_interest_name) {

            close_POI.push_back(street_graph.POI_info_vector[i]);
        }
    }
    return close_POI;
}*/