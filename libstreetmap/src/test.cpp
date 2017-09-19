#include "find_path.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "structure.h"
#include "m1.h"
#include "m3.h"
#include "m2.h"
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
#include <queue>

/*std::vector<unsigned> find_path_between_intersections(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty) {

    std::vector<intersection> intersection_trace;

    intersection_trace[intersect_id_start].index = intersect_id_start;

    intersection_trace[intersect_id_start].cost = 0;

    path_cell find_path;

    vector<unsigned> best_path;

    vector<unsigned> best_path_temp;

    find_path.cell_queue.push(intersection_trace[intersect_id_start]);

    while (!find_path.cell_queue.empty()) {

        intersection top_cell = find_path.cell_queue.top();

        find_path.cell_queue.pop();

        /*if(top_cell.index == intersect_id_end)
         track back 
         
         */

        /*if(top_cell.cost <= intersection_trace[top_cell.index].cost) {

            std::vector<intersection_edge_info> connected_intersection_info = connected_intersections[top_cell.index];

            for (unsigned i = 0; i < connected_intersections.size(); i++) {

                unsigned to = connected_intersection_info[i].connected_intersection;

                float cost_temp = top_cell.cost + connected_intersection_info[i].travlle_time_to_next_intersection;

                if (top_cell.from != -1) {

                    unsigned streetID1 = getStreetSegmentInfo(top_cell.from).streetID;

                    unsigned streetID2 = getStreetSegmentInfo(to).streetID;

                    if (streetID1 != streetID2)
                        cost_temp += turn_penalty;
                }

                if (intersection_trace[to].cost > cost_temp) {

                    intersection_trace[to].cost = cost_temp;

                    intersection_trace[to].from = top_cell.index;

                    intersection_trace[to].through = connected_intersection_info[i].edge_index;
                    
                    find_path.cell_queue.push(intersection_trace[to].cost);
                }
            }
        }
    }
    
    return best_path;
}*/