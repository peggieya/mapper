//#include "structure.h"
//#include "m4_helper.h"
//#include "m4_thread.h"
//#include "m1.h"
//#include "m2.h"
//#include "m3.h"
//#include "m4.h"
//#include "m2_helper.h"
//#include <string>
//#include <unordered_map>
//#include <unordered_set>
//#include <vector>
//#include <iostream>
//#include <sstream>
//#include <algorithm>
//#include <cmath>
//#include <math.h>
//#include <stack> 
//#include <vector>
//#include <deque>
//#include <queue> 
//#include <climits>
//#include <list>
//#include <thread>
//#include <stdlib.h> 
//#include <time.h> 
//#include <ctime>
//
//#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)
//
//extern streetGraph street_graph;
//
//extern std::vector<std::vector<intersection_edge_info> > connected_intersections;
//
//std::unordered_map<IntersectionIndex, std::vector<IntersectionIndex> > delivery_node_map;
//
//std::unordered_map<IntersectionIndex, IntersectionIndex> dropoff_pickup_map;
//
//std::vector<std::vector<unsigned> > ordered_nodes_vector;
//
//std::vector<std::vector<unsigned> > ordered_path_vector;
//
//std::vector<float> ordered_path_travel_time_vector;
//
//std::vector<unsigned> pick_up;
//
//std::vector<unsigned> find_path_thread(const std::vector<DeliveryInfo>& deliveries, const std::vector<unsigned>& depots, const float turn_penalty) {
//
//    for (unsigned i = 0; i < deliveries.size(); i++) {
//
//        dropoff_pickup_map.insert(std::make_pair(deliveries[i].dropOff, deliveries[i].pickUp));
//
//        std::vector<IntersectionIndex> valid_dropoff;
//
//        if (delivery_node_map.find(deliveries[i].pickUp) == delivery_node_map.end()) {
//
//            pick_up.push_back(deliveries[i].pickUp);
//
//            valid_dropoff.push_back(deliveries[i].dropOff);
//
//            delivery_node_map.insert(std::make_pair(deliveries[i].pickUp, valid_dropoff));
//
//        } else {
//
//            delivery_node_map.find(deliveries[i].pickUp)->second.push_back(deliveries[i].dropOff);
//        }
//    }
//
//    ordered_nodes_vector.resize(pick_up.size());
//
//    ordered_path_vector.resize(pick_up.size());
//
//    ordered_path_travel_time_vector.resize(pick_up.size());
//
//    std::vector<std::thread> thread_group1(pick_up.size());
//
//    for (unsigned i = 0; i < pick_up.size(); ++i) {
//
//        thread_group1.at(i) = std::thread(order_node_thread_helper, pick_up[i], pick_up, turn_penalty, i);
//    }
//
//    for (unsigned a = 0; a < pick_up.size(); ++a) {
//
//        thread_group1.at(a).join();
//    }
//
//    std::vector<std::thread> thread_group2(ordered_nodes_vector.size());
//
//    for (unsigned j = 0; j < ordered_nodes_vector.size(); ++j) {
//        thread_group2.at(j) = std::thread(find_path_thread_helper, ordered_nodes_vector[j], depots, turn_penalty, j);
//    }
//
//    for (unsigned b = 0; b < pick_up.size(); ++b) {
//
//        thread_group2.at(b).join();
//    }
//
//    std::vector<std::thread> thread_group3(ordered_path_vector.size());
//
//    for (unsigned k = 0; k < ordered_path_vector.size(); ++k) {
//        thread_group3.at(k) = std::thread(compute_path_travel_time_thread_helper, ordered_path_vector[k], turn_penalty, k);
//    }
//
//    for (unsigned c = 0; c < pick_up.size(); ++c) {
//
//        thread_group3.at(c).join();
//    }
//
//    unsigned min_time = ordered_path_travel_time_vector[0];
//
//    unsigned min_index = 0;
//
//    for (unsigned m = 1; m < ordered_path_travel_time_vector.size(); m++) {
//
//        unsigned temp_time = ordered_path_travel_time_vector[m];
//
//        if (temp_time < min_time) {
//
//            min_time = temp_time;
//
//            min_index = m;
//        }
//    }
//
//    return ordered_path_vector[min_index];
//}
//
//void order_node_thread_helper(unsigned start_id, std::vector<unsigned> pick_up, const float turn_penalty, unsigned counter) {
//
//    std::unordered_map<IntersectionIndex, std::vector<IntersectionIndex> > delivery_node_map_temp = delivery_node_map;
//
//    std::vector<IntersectionIndex> ordered_delivery_nodes;
//
//    std::vector<IntersectionIndex> valid_nodes = pick_up;
//
//    unsigned start = start_id;
//
//    for (unsigned j = 0; j < delivery_node_map_temp.find(start)->second.size(); j++) {
//
//        valid_nodes.push_back((delivery_node_map_temp.find(start)->second)[j]);
//
//    }
//
//    while (!delivery_node_map_temp.empty()) {
//
//        unsigned next_node = order_node_helper(start, turn_penalty, valid_nodes);
//
//        ordered_delivery_nodes.push_back(next_node);
//
//        auto itr = valid_nodes.begin();
//
//        for (unsigned k = 0; k < valid_nodes.size(); k++) {
//
//            if (*itr == next_node) {
//
//                valid_nodes.erase(itr);
//            }
//
//            itr++;
//        }
//
//        if (delivery_node_map_temp.find(next_node) != delivery_node_map_temp.end()) {
//
//            for (unsigned l = 0; l < delivery_node_map_temp.find(next_node)->second.size(); l++) {
//
//                valid_nodes.push_back((delivery_node_map_temp.find(next_node)->second)[l]);
//            }
//
//            delivery_node_map_temp.erase(delivery_node_map_temp.find(next_node));
//        }
//
//        start = next_node;
//    }
//
//    while (!valid_nodes.empty()) {
//
//        unsigned next_node = order_node_helper(start, turn_penalty, valid_nodes);
//
//        auto itr = valid_nodes.begin();
//
//        for (unsigned k = 0; k < valid_nodes.size(); k++) {
//
//            if (*itr == next_node) {
//
//                valid_nodes.erase(itr);
//            }
//
//            itr++;
//        }
//
//        ordered_delivery_nodes.push_back(next_node);
//
//        start = next_node;
//    }
//
//    ordered_delivery_nodes.erase(std::unique(ordered_delivery_nodes.begin(), ordered_delivery_nodes.end()), ordered_delivery_nodes.end());
//
//    ordered_nodes_vector[counter] = ordered_delivery_nodes;
//}
//
//void find_path_thread_helper(const std::vector<unsigned> path_node, const std::vector<unsigned>& depots, const float turn_penalty, unsigned counter) {
//
//    std::vector<unsigned> depots_temp = depots;
//
//    unsigned start_id = path_node[0];
//
//    unsigned start_depot_id = find_closest_depot(start_id, depots, turn_penalty);
//
//    std::vector<unsigned> delivery_path_temp;
//
//    std::vector<unsigned> delivery_path;
//
//    delivery_path_temp = find_path_between_intersections(start_depot_id, start_id, turn_penalty);
//
//    while (delivery_path_temp.size() == 0) {
//
//        auto itr = depots_temp.begin();
//
//        while (*itr != start_depot_id)
//            itr++;
//
//        depots_temp.erase(itr);
//
//        depots_temp.shrink_to_fit();
//
//        start_depot_id = find_closest_depot(start_id, depots_temp, turn_penalty);
//
//        delivery_path_temp = find_path_between_intersections(start_depot_id, start_id, turn_penalty);
//    }
//
//    delivery_path = delivery_path_temp;
//
//    unsigned end_id = path_node[1];
//
//    for (unsigned j = 1; j < path_node.size(); j++) {
//
//        start_id = path_node[j - 1];
//
//        end_id = path_node[j];
//
//        delivery_path_temp = find_path_between_intersections(start_id, end_id, turn_penalty);
//
//        for (unsigned k = 0; k < delivery_path_temp.size(); k++) {
//
//            delivery_path.push_back(delivery_path_temp[k]);
//
//        }
//    }
//
//    unsigned end_depot_id = find_closest_depot(end_id, depots_temp, turn_penalty);
//
//    delivery_path_temp = find_path_between_intersections(end_id, end_depot_id, turn_penalty);
//
//    depots_temp = depots;
//
//    while (delivery_path_temp.size() == 0) {
//
//        auto itr = depots_temp.begin();
//
//        while (*itr != end_depot_id)
//            itr++;
//
//        depots_temp.erase(itr);
//
//        depots_temp.shrink_to_fit();
//
//        end_depot_id = find_closest_depot(end_id, depots_temp, turn_penalty);
//
//        delivery_path_temp = find_path_between_intersections(end_id, end_depot_id, turn_penalty);
//    }
//
//    for (unsigned l = 0; l < delivery_path_temp.size(); l++) {
//
//        delivery_path.push_back(delivery_path_temp[l]);
//    }
//
//    ordered_path_vector[counter] = delivery_path;
//}
//
//void compute_path_travel_time_thread_helper(const std::vector<unsigned>& path, const double turn_penalty, unsigned counter) {
//
//    float travelTime = 0;
//
//    unsigned numOfTurns = 0;
//
//    if (path.size() == 0) {
//        ordered_path_travel_time_vector[counter] = travelTime;
//    } else {
//
//        for (unsigned i = 0; i < path.size(); i++) {
//
//            travelTime += find_street_segment_travel_time(path[i]);
//
//            if (i > 0) {
//
//                if (getStreetSegmentInfo(path[i]).streetID != getStreetSegmentInfo(path[i - 1]).streetID)
//
//                    numOfTurns++;
//            }
//        }
//        ordered_path_travel_time_vector[counter] = (travelTime + numOfTurns * turn_penalty);
//    }
//}