//#include "structure.h"
//#include "m4_helper.h"
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
//
//#include <boost/geometry.hpp>
//#include <boost/geometry/geometries/point.hpp>
//#include <boost/geometry/index/rtree.hpp>
//#include <boost/geometry/geometries/box.hpp>
//#include <boost/foreach.hpp>
//
////#define TIME_LIMIT 19
//
//extern streetGraph street_graph;
//
//extern std::vector<std::vector<intersection_edge_info> > connected_intersections;
//
//std::vector<std::vector<swap_node> > intersection_cost_vector;
//
//std::vector<std::vector<unsigned> > better_path_node_vector;
//
//std::vector<std::vector<unsigned> > combine_section_vector;
//
//std::vector<std::vector<unsigned> > compare_sub_path;
//
//std::vector<float> compare_sub_path_cost;
//
//std::vector<unsigned> sectionA;
//
//std::vector<unsigned> sectionB;
//
//std::vector<unsigned> sectionC;
//
//std::vector<unsigned> sectionA_rev;
//
//std::vector<unsigned> sectionB_rev;
//
//std::vector<unsigned> sectionC_rev;
//
//std::vector<bool> valid_vector;
//
//unsigned find_closest_depot(unsigned start_id, const std::vector<unsigned>& depots, const float turn_penalty) {
//
//    std::vector<intersection> intersection_trace;
//
//    intersection_trace.resize(street_graph.number_of_intersections);
//
//    intersection_trace[start_id].index = start_id;
//
//    intersection_trace[start_id].cost = 0;
//
//    path_cell find_path;
//
//    std::vector<unsigned> best_path;
//
//    find_path.cell_queue.push(intersection_trace[start_id]);
//
//    while (!find_path.cell_queue.empty()) {
//
//        intersection top_cell = find_path.cell_queue.top();
//
//        find_path.cell_queue.pop();
//
//        for (unsigned i = 0; i < depots.size(); i++) {
//
//            if (top_cell.index == depots[i]) {
//
//                return depots[i];
//            }
//        }
//
//        if (top_cell.cost <= intersection_trace[top_cell.index].cost) {
//
//            std::vector<intersection_edge_info> connected_intersection_info = connected_intersections[top_cell.index];
//
//            for (unsigned i = 0; i < connected_intersection_info.size(); i++) {
//
//                unsigned to = connected_intersection_info[i].connected_intersection;
//
//                float cost_temp = top_cell.cost + connected_intersection_info[i].travlle_time_to_next_intersection;
//
//                if (top_cell.from != -1) {
//
//                    unsigned streetID1 = getStreetSegmentInfo(top_cell.through).streetID;
//
//                    unsigned streetID2 = getStreetSegmentInfo(connected_intersection_info[i].edge_index).streetID;
//
//                    if (streetID1 != streetID2)
//                        cost_temp += turn_penalty;
//                }
//
//                if (intersection_trace[to].cost > cost_temp) {
//
//                    intersection_trace[to].cost = cost_temp;
//
//                    intersection_trace[to].index = connected_intersection_info[i].connected_intersection;
//
//                    intersection_trace[to].from = top_cell.index;
//
//                    intersection_trace[to].through = connected_intersection_info[i].edge_index;
//
//                    find_path.cell_queue.push(intersection_trace[to]);
//
//                }
//            }
//        }
//    }
//
//    /*bgi::rtree<depot_position, bgi::quadratic<16> > depot_rtree;
//
//    for (unsigned i = 0; i < depots.size(); i++) {
//
//        t_point this_depot_position = street_graph.intersection_position_vector[depots[i]];
//
//        point_float point_depot = point_float(this_depot_position.x, this_depot_position.y);
//
//        depot_rtree.insert(std::make_pair(point_depot, depots[i]));
//    }
//
//    t_point start_position = street_graph.intersection_position_vector[start_id];
//
//    point_float start_position_point = point_float(start_position.x, start_position.y);
//
//    std::vector<depot_position> nearest_depot_temp;
//
//    depot_rtree.query(bgi::nearest(start_position_point, 1), std::back_inserter(nearest_depot_temp));
//
//    return nearest_depot_temp[0].second;*/
//}
//
//void two_opt(std::vector<unsigned> path_node, const std::vector<DeliveryInfo>& deliveries,
//        const std::vector<unsigned>& depots, const float turn_penalty, unsigned key) {
//
//
//    std::vector<unsigned> new_path_node = path_node;
//
//    std::vector<unsigned> better_path_node = path_node;
//
//    std::vector<unsigned> best_path_node;
//
//    unsigned best_time = compute_path_cost(path_node, turn_penalty, depots);
//
//    ////std::cout << "the best time is:  " << best_time << std::endl;
//
//    //std::vector<unsigned> new_path;
//
//    bool path_valid = false;
//
//    bool better_path_found = true;
//
//    bool first_swap = true;
//
//    //
//    //        //std::cout << "new round!" << std::endl;
//
//    //
//    //                for (unsigned hehe = 0; hehe < new_path_node.size(); hehe++) {
//    //            
//    //                    //std::cout << new_path_node[hehe] << "  ";
//    //                }
//    //            
//    //                //std::cout << "\n-------------------------------------------\n" << std::endl;
//
//
//
//    while (better_path_found) {
//
//        ////std::cout << "\n\n********************** New Round ************************\n\n" << std::endl;
//
//        better_path_found = false;
//
//        if (!first_swap) {
//
//            path_node = better_path_node;
//
//            new_path_node = better_path_node;
//        }
//
//        for (unsigned i = 0; i < new_path_node.size(); i++) {
//            for (unsigned j = (i + 1); j < new_path_node.size(); j++) {
//
//                new_path_node = path_node;
//
//                unsigned counter1 = i;
//
//                unsigned counter2 = j;
//
//                while (counter1 <= j) {
//
//                    new_path_node[counter1] = path_node[counter2];
//
//                    counter1++;
//
//                    counter2--;
//                }
//
//                path_valid = check_path_node_valid(new_path_node, deliveries);
//
//                if (path_valid) {
//
//                    ////std::cout << "\n %%%%%%%%%%%%%%  Valid  %%%%%%%%%%%5\n" << std::endl;
//
//                    unsigned temp_time = compute_path_cost(new_path_node, turn_penalty, depots);
//
//                    if (temp_time < best_time) {
//
//                        ////std::cout << "\n %%%%%%%%%%%%%%  Better  %%%%%%%%%%%5\n" << std::endl;
//
//                        best_time = temp_time;
//
//                        better_path_found = true;
//
//                        better_path_node = new_path_node;
//
//                    }
//                }
//            }
//        }
//
//        first_swap = false;
//    }
//
//    better_path_node_vector[key] = better_path_node;
//
//
//}
//
//void combine_section_thread(unsigned key) {
//}
//
//void check_valid_thread(unsigned key, const std::vector<DeliveryInfo>& deliveries) {
//
//    valid_vector[key] = check_path_node_valid(combine_section_vector[key], deliveries);
//}
//
//std::vector<unsigned> two_opt_better(std::vector<unsigned> path_node, const std::vector<DeliveryInfo>& deliveries,
//        const std::vector<unsigned>& depots, const float turn_penalty) {
//
//    auto startTime = std::chrono::high_resolution_clock::now();
//
//    auto currentTime = std::chrono::high_resolution_clock::now();
//
//    auto wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
//
//    unsigned best_time = compute_path_cost(path_node, turn_penalty, depots);
//
//    //std::cout << "\n the best time is -->  " << best_time << std::endl;
//
//    std::vector<unsigned> path_node_opt = path_node;
//
//    bool better_path_found = true;
//
//    w
//    combine_section_vector.resize(48);
//
//    bool first_opt = true;
//
//    unsigned adder = 2;
//
//    unsigned TIME_LIMIT = 23;
//
//    if (path_node.size() >= 100 && path_node.size() < 200) {
//
//        adder = 6;
//
//    } else if (path_node.size() >= 200 && path_node.size() < 300) {
//
//        adder = 10;
//    } else if (path_node.size() >= 300) {
//
//        adder = 20;
//    }
//
//    std::vector<unsigned> second_best_node_temp;
//
//    std::vector<unsigned> best_node_temp;
//    
//    std::vector<unsigned> temp_path;
//
//    while (better_path_found) {
//
//        better_path_found = false;
//
//        if (!first_opt) {
//            
//            temp_path = path_node;
//
//            path_node = path_node_opt;
//        }
//
//        for (unsigned i = 0; i < path_node.size(); i++) {
//            for (unsigned j = i + 1; j < path_node.size(); j = j + adder) {
//
//                if (wallClock.count() > TIME_LIMIT) {
//
//                    return path_node_opt;
//                }
//
//                compare_sub_path_cost.clear();
//
//                compare_sub_path_cost.resize(8);
//
//                compare_sub_path.clear();
//
//                compare_sub_path.resize(8);
//
//                combine_section_vector.clear();
//
//                combine_section_vector.resize(48);
//
//                sectionA.clear();
//                sectionB.clear();
//                sectionC.clear();
//
//                sectionA_rev.clear();
//                sectionB_rev.clear();
//                sectionC_rev.clear();
//
//                unsigned counter1 = 0;
//
//                while (counter1 < i) {
//
//                    sectionA.push_back(path_node[counter1]);
//
//                    counter1++;
//                }
//
//                unsigned counter2 = j;
//
//                while (counter1 <= j) {
//
//                    sectionB.push_back(path_node[counter2]);
//
//                    counter1++;
//
//                    counter2--;
//                }
//
//                unsigned counter3 = j + 1;
//
//                while (counter3 < path_node.size()) {
//
//                    sectionC.push_back(path_node[counter3]);
//
//                    counter3++;
//                }
//
//                sectionA_rev = sectionA;
//                std::reverse(sectionA_rev.begin(), sectionA_rev.end());
//
//                sectionB_rev = sectionB;
//                std::reverse(sectionB_rev.begin(), sectionB_rev.end());
//
//                sectionC_rev = sectionC;
//                std::reverse(sectionC_rev.begin(), sectionC_rev.end());
//
//                std::thread t1(two_opt_sub1, deliveries, turn_penalty, depots);
//                std::thread t2(two_opt_sub2, deliveries, turn_penalty, depots);
//                std::thread t3(two_opt_sub3, deliveries, turn_penalty, depots);
//                std::thread t4(two_opt_sub4, deliveries, turn_penalty, depots);
//                std::thread t5(two_opt_sub5, deliveries, turn_penalty, depots);
//                std::thread t6(two_opt_sub6, deliveries, turn_penalty, depots);
//                std::thread t7(two_opt_sub7, deliveries, turn_penalty, depots);
//                std::thread t8(two_opt_sub8, deliveries, turn_penalty, depots);
//
//                t1.join();
//                t2.join();
//                t3.join();
//                t4.join();
//                t5.join();
//                t6.join();
//                t7.join();
//                t8.join();
//
//                for (unsigned a = 0; a < compare_sub_path.size(); a++) {
//
//                    if (compare_sub_path[a].size() > 0) {
//
//                        float temp_time = compare_sub_path_cost[a];
//
//                        if (temp_time < best_time) {
//
//                            path_node_opt = compare_sub_path[a];
//
//                            best_time = temp_time;
//
//                            better_path_found = true;
//                        }
//                    }
//                }
//
//                currentTime = std::chrono::high_resolution_clock::now();
//
//                wallClock = std::chrono::duration_cast<std::chrono::duration<double>> (currentTime - startTime);
//            }
//        }
//
//        first_opt = false;
//
//        best_node_temp = path_node_opt;
//
//        //std::cout << "time used --> " << wallClock.count() << std::endl;
//    }
//    //
//    //    for (unsigned t = 0; t < path_node_opt.size(); t++) {
//    //
//    //        std::cout << path_node_opt[t] << "  ";
//    //    }
//
//    return path_node_opt;
//}
//
//void two_opt_sub1(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots) {
//
//    std::vector<unsigned> combine_temp;
//
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_section_vector[0] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_section_vector[1] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_section_vector[2] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_section_vector[3] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_section_vector[4] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_section_vector[5] = combine_temp;
//    combine_temp.clear();
//
//    bool first_valid = true;
//
//    float best_time;
//
//    float temp_time;
//
//    for (unsigned i = 0; i < 6; i++) {
//
//        bool if_valid = check_path_node_valid(combine_section_vector[i], deliveries);
//
//        if (first_valid && if_valid) {
//
//            compare_sub_path[0] = combine_section_vector[i];
//
//            best_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            compare_sub_path_cost[0] = best_time;
//
//            first_valid = false;
//
//        } else if (if_valid) {
//
//            temp_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            if (temp_time < best_time) {
//
//                compare_sub_path[0] = combine_section_vector[i];
//
//                compare_sub_path_cost[0] = temp_time;
//
//                best_time = temp_time;
//            }
//        }
//    }
//}
//
//void two_opt_sub2(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots) {
//
//    std::vector<unsigned> combine_temp;
//
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_section_vector[6] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_section_vector[7] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_section_vector[8] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_section_vector[9] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_section_vector[10] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_section_vector[11] = combine_temp;
//    combine_temp.clear();
//
//    bool first_valid = true;
//
//    float best_time;
//
//    float temp_time;
//
//    for (unsigned i = 6; i < 12; i++) {
//
//        bool if_valid = check_path_node_valid(combine_section_vector[i], deliveries);
//
//        if (first_valid && if_valid) {
//
//            compare_sub_path[1] = combine_section_vector[i];
//
//            best_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            compare_sub_path_cost[1] = best_time;
//
//            compare_sub_path_cost[1] = best_time;
//
//            first_valid = false;
//
//        } else if (if_valid) {
//
//            temp_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            if (temp_time < best_time) {
//
//                compare_sub_path[1] = combine_section_vector[i];
//
//                compare_sub_path_cost[1] = temp_time;
//
//                best_time = temp_time;
//            }
//        }
//    }
//}
//
//void two_opt_sub3(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots) {
//
//    std::vector<unsigned> combine_temp;
//
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_section_vector[12] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_section_vector[13] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_section_vector[14] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_section_vector[15] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_section_vector[16] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_section_vector[17] = combine_temp;
//    combine_temp.clear();
//
//    bool first_valid = true;
//
//    float best_time;
//
//    float temp_time;
//
//    for (unsigned i = 12; i < 18; i++) {
//
//        bool if_valid = check_path_node_valid(combine_section_vector[i], deliveries);
//
//        if (first_valid && if_valid) {
//
//            compare_sub_path[2] = combine_section_vector[i];
//
//            best_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            compare_sub_path_cost[2] = best_time;
//
//            first_valid = false;
//
//        } else if (if_valid) {
//
//            temp_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            if (temp_time < best_time) {
//
//                compare_sub_path[2] = combine_section_vector[i];
//
//                compare_sub_path_cost[2] = temp_time;
//
//                best_time = temp_time;
//            }
//        }
//    }
//}
//
//void two_opt_sub4(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots) {
//
//    std::vector<unsigned> combine_temp;
//
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_section_vector[18] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_section_vector[19] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_section_vector[20] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_section_vector[21] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_section_vector[22] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_section_vector[23] = combine_temp;
//    combine_temp.clear();
//
//    bool first_valid = true;
//
//    float best_time;
//
//    float temp_time;
//
//    for (unsigned i = 18; i < 24; i++) {
//
//        bool if_valid = check_path_node_valid(combine_section_vector[i], deliveries);
//
//        if (first_valid && if_valid) {
//
//            compare_sub_path[3] = combine_section_vector[i];
//
//            best_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            compare_sub_path_cost[3] = best_time;
//
//            first_valid = false;
//
//        } else if (if_valid) {
//
//            temp_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            if (temp_time < best_time) {
//
//                compare_sub_path[3] = combine_section_vector[i];
//
//                compare_sub_path_cost[3] = temp_time;
//
//                best_time = temp_time;
//            }
//        }
//    }
//}
//
//void two_opt_sub5(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots) {
//
//    std::vector<unsigned> combine_temp;
//
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_section_vector[24] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_section_vector[25] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_section_vector[26] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_section_vector[27] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_section_vector[28] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_section_vector[29] = combine_temp;
//    combine_temp.clear();
//
//    bool first_valid = true;
//
//    float best_time;
//
//    float temp_time;
//
//    for (unsigned i = 24; i < 30; i++) {
//
//        bool if_valid = check_path_node_valid(combine_section_vector[i], deliveries);
//
//        if (first_valid && if_valid) {
//
//            compare_sub_path[4] = combine_section_vector[i];
//
//            best_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            compare_sub_path_cost[4] = best_time;
//
//            first_valid = false;
//
//        } else if (if_valid) {
//
//            temp_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            if (temp_time < best_time) {
//
//                compare_sub_path[4] = combine_section_vector[i];
//
//                compare_sub_path_cost[4] = temp_time;
//
//                best_time = temp_time;
//            }
//        }
//    }
//}
//
//void two_opt_sub6(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots) {
//
//    std::vector<unsigned> combine_temp;
//
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_section_vector[30] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_section_vector[31] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_section_vector[32] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_section_vector[33] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_section_vector[34] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_section_vector[35] = combine_temp;
//    combine_temp.clear();
//
//    bool first_valid = true;
//
//    float best_time;
//
//    float temp_time;
//
//    for (unsigned i = 30; i < 36; i++) {
//
//        bool if_valid = check_path_node_valid(combine_section_vector[i], deliveries);
//
//        if (first_valid && if_valid) {
//
//            compare_sub_path[5] = combine_section_vector[i];
//
//            best_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            compare_sub_path_cost[5] = best_time;
//
//            first_valid = false;
//
//        } else if (if_valid) {
//
//            temp_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            if (temp_time < best_time) {
//
//                compare_sub_path[5] = combine_section_vector[i];
//
//                compare_sub_path_cost[5] = temp_time;
//
//                best_time = temp_time;
//            }
//        }
//    }
//}
//
//void two_opt_sub7(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots) {
//
//    std::vector<unsigned> combine_temp;
//
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_section_vector[36] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_section_vector[37] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_section_vector[38] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_section_vector[39] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_section_vector[40] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_section_vector[41] = combine_temp;
//    combine_temp.clear();
//
//    bool first_valid = true;
//
//    float best_time;
//
//    float temp_time;
//
//    for (unsigned i = 36; i < 42; i++) {
//
//        bool if_valid = check_path_node_valid(combine_section_vector[i], deliveries);
//
//        if (first_valid && if_valid) {
//
//            compare_sub_path[6] = combine_section_vector[i];
//
//            best_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            compare_sub_path_cost[6] = best_time;
//
//            first_valid = false;
//
//        } else if (if_valid) {
//
//            temp_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            if (temp_time < best_time) {
//
//                compare_sub_path[6] = combine_section_vector[i];
//
//                compare_sub_path_cost[6] = temp_time;
//
//                best_time = temp_time;
//            }
//        }
//    }
//}
//
//void two_opt_sub8(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots) {
//
//    std::vector<unsigned> combine_temp;
//
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_section_vector[42] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_section_vector[43] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionC_rev.begin(), sectionC_rev.end());
//    combine_section_vector[44] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_section_vector[45] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionA.begin(), sectionA.end());
//    combine_temp.insert(combine_temp.end(), sectionB_rev.begin(), sectionB_rev.end());
//    combine_section_vector[46] = combine_temp;
//    combine_temp.clear();
//
//    combine_temp.insert(combine_temp.end(), sectionC.begin(), sectionC.end());
//    combine_temp.insert(combine_temp.end(), sectionB.begin(), sectionB.end());
//    combine_temp.insert(combine_temp.end(), sectionA_rev.begin(), sectionA_rev.end());
//    combine_section_vector[47] = combine_temp;
//    combine_temp.clear();
//
//    bool first_valid = true;
//
//    float best_time;
//
//    float temp_time;
//
//    for (unsigned i = 42; i < 48; i++) {
//
//        bool if_valid = check_path_node_valid(combine_section_vector[i], deliveries);
//
//        if (first_valid && if_valid) {
//
//            compare_sub_path[7] = combine_section_vector[i];
//
//            best_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            compare_sub_path_cost[7] = best_time;
//
//            first_valid = false;
//
//        } else if (if_valid) {
//
//            temp_time = compute_path_cost(combine_section_vector[i], turn_penalty, depots);
//
//            if (temp_time < best_time) {
//
//                compare_sub_path[7] = combine_section_vector[i];
//
//                compare_sub_path_cost[7] = temp_time;
//
//                best_time = temp_time;
//            }
//        }
//    }
//}
//
//bool check_path_node_valid(std::vector<unsigned> path_node, const std::vector<DeliveryInfo>& deliveries) {
//
//    bool picked_up = false;
//
//    bool is_dropoff = false;
//
//    unsigned this_pickup;
//
//    for (unsigned i = 0; i < path_node.size(); i++) {
//
//        for (unsigned j = 0; j < deliveries.size(); j++) {
//
//            if (deliveries[j].dropOff == path_node[i]) {
//
//                is_dropoff = true;
//
//                this_pickup = deliveries[j].pickUp;
//            }
//        }
//
//        if (is_dropoff) {
//
//            for (unsigned k = 0; k < i; k++) {
//
//                if (path_node[k] == this_pickup) {
//
//                    picked_up = true;
//
//                    path_node[k] = INT_MAX;
//
//                    k = INT_MAX;
//                }
//
//                if (!picked_up)
//                    return false;
//            }
//
//            picked_up = false;
//        }
//    }
//
//    return true;
//}
//
//std::vector<unsigned> find_delivery_path(const std::vector<unsigned> path_node, const std::vector<DeliveryInfo>& deliveries,
//        const std::vector<unsigned>& depots, const float turn_penalty) {
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
//    delivery_path = find_path_between_intersections(start_depot_id, start_id, turn_penalty);
//
//    unsigned end_id = path_node[1];
//
//    //std::cout << "\nConstructing path between nodes --->  " << start_depot_id << " and " << start_id << std::endl;
//    //std::cout << "The path size is --->  " << delivery_path.size() << std::endl;
//
//    for (unsigned j = 1; j < path_node.size(); j++) {
//
//        start_id = path_node[j - 1];
//
//        end_id = path_node[j];
//
//        delivery_path_temp = find_path_between_intersections(start_id, end_id, turn_penalty);
//
//        //std::cout << "\nConstructing path between nodes --->  " << start_id << " and " << end_id << std::endl;
//        //std::cout << "The path size is --->  " << delivery_path_temp.size() << std::endl;
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
//    if (delivery_path_temp.size() == 0) {
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
//    //std::cout << "\nConstructing path between nodes --->  " << end_id << " and " << end_depot_id << std::endl;
//    //std::cout << "The path size is --->  " << delivery_path_temp.size() << std::endl;
//
//    for (unsigned l = 0; l < delivery_path_temp.size(); l++) {
//
//        delivery_path.push_back(delivery_path_temp[l]);
//    }
//
//
//    return delivery_path;
//
//}
//
//unsigned order_node_helper(const unsigned intersect_id_start, const double turn_penalty, std::vector<unsigned> valid_nodes) {
//
//    std::vector<intersection> intersection_trace;
//
//    intersection_trace.resize(street_graph.number_of_intersections);
//
//    intersection_trace[intersect_id_start].index = intersect_id_start;
//
//    intersection_trace[intersect_id_start].cost = 0;
//
//    path_cell find_path;
//
//    std::vector<unsigned> best_path;
//
//    find_path.cell_queue.push(intersection_trace[intersect_id_start]);
//
//    while (!find_path.cell_queue.empty()) {
//
//        intersection top_cell = find_path.cell_queue.top();
//
//        find_path.cell_queue.pop();
//
//        for (unsigned i = 0; i < valid_nodes.size(); i++) {
//
//            if (top_cell.index == valid_nodes[i]) {
//
//                return valid_nodes[i];
//            }
//        }
//
//        if (top_cell.cost <= intersection_trace[top_cell.index].cost) {
//
//            std::vector<intersection_edge_info> connected_intersection_info = connected_intersections[top_cell.index];
//
//            for (unsigned i = 0; i < connected_intersection_info.size(); i++) {
//
//                unsigned to = connected_intersection_info[i].connected_intersection;
//
//                float cost_temp = top_cell.cost + connected_intersection_info[i].travlle_time_to_next_intersection;
//
//                if (top_cell.from != -1) {
//
//                    unsigned streetID1 = getStreetSegmentInfo(top_cell.through).streetID;
//
//                    unsigned streetID2 = getStreetSegmentInfo(connected_intersection_info[i].edge_index).streetID;
//
//                    if (streetID1 != streetID2)
//                        cost_temp += turn_penalty;
//                }
//
//                if (intersection_trace[to].cost > cost_temp) {
//
//                    intersection_trace[to].cost = cost_temp;
//
//                    intersection_trace[to].index = connected_intersection_info[i].connected_intersection;
//
//                    intersection_trace[to].from = top_cell.index;
//
//                    intersection_trace[to].through = connected_intersection_info[i].edge_index;
//
//                    find_path.cell_queue.push(intersection_trace[to]);
//
//                }
//            }
//        }
//    }
//}
//
//std::vector<unsigned> large_amount_search(const std::vector<DeliveryInfo>& deliveries, const std::vector<unsigned>& depots, const float turn_penalty) {
//
//    std::unordered_map<IntersectionIndex, std::vector<IntersectionIndex> > delivery_node_map;
//
//    std::vector<unsigned> pick_up;
//
//    for (unsigned i = 0; i < deliveries.size(); i++) {
//
//        std::vector<IntersectionIndex> valid_dropoff;
//
//        if (delivery_node_map.find(deliveries[i].pickUp) == delivery_node_map.end()) {
//
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
//    unsigned start_id = deliveries[0].pickUp;
//
//
//    std::vector<unsigned> ordered_delivery_nodes = order_nodes(start_id, pick_up, delivery_node_map, turn_penalty);
//
//    std::vector<unsigned> ordered_delivery_nodes_temp = ordered_delivery_nodes;
//
//    for (unsigned a = 0; a < depots.size(); a++) {
//
//        ordered_delivery_nodes_temp.push_back(depots[a]);
//    }
//
//    intersection_cost_vector.resize(ordered_delivery_nodes_temp.size());
//
//    std::vector<std::thread> load_cost_thread_group(ordered_delivery_nodes_temp.size());
//
//    for (unsigned j = 0; j < ordered_delivery_nodes_temp.size(); ++j) {
//
//        load_cost_thread_group.at(j) = std::thread(load_nodes_time_cost, ordered_delivery_nodes_temp[j], ordered_delivery_nodes_temp, j, turn_penalty);
//    }
//
//    for (unsigned b = 0; b < ordered_delivery_nodes_temp.size(); ++b) {
//
//        load_cost_thread_group.at(b).join();
//    }
//
//    //std::cout << "extreme greedy start!\n" << std::endl;
//
//    //    for (unsigned c = 0; c < ordered_delivery_nodes.size(); c++) {
//    //
//    //        std::cout << ordered_delivery_nodes[c] << "  " << std::endl;
//    //    }
//
//    //ordered_delivery_nodes = extreme_greedy_order_node(deliveries, depots, turn_penalty, delivery_node_map);
//
//    //    std::cout << " \n*************************************************************************************\n " << std::endl;
//    //
////    for (unsigned d = 0; d < ordered_delivery_nodes.size(); d++) {
////
////        std::cout << ordered_delivery_nodes[d] << "  " << std::endl;
////    }
////
////    std::cout << " \n*************************************************************************************\n " << std::endl;
//
//    //std::cout << "extreme greedy finished!\n" << std::endl;
//
//    ordered_delivery_nodes.erase(std::unique(ordered_delivery_nodes.begin(), ordered_delivery_nodes.end()), ordered_delivery_nodes.end());
//
////    for (unsigned d = 0; d < ordered_delivery_nodes.size(); d++) {
////
////        std::cout << ordered_delivery_nodes[d] << "  " << std::endl;
////    }
//
//    std::vector<unsigned> best_path_node = two_opt_better(ordered_delivery_nodes, deliveries, depots, turn_penalty);
//
//    std::vector<unsigned> best_path = find_delivery_path(best_path_node, deliveries, depots, turn_penalty);
//
//    return best_path;
//}
//
//std::vector<unsigned> order_nodes(unsigned start_id, std::vector<unsigned> pick_up,
//        std::unordered_map<IntersectionIndex, std::vector<IntersectionIndex> > delivery_node_map,
//        const float turn_penalty) {
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
//    bool first = true;
//
//    while (!delivery_node_map_temp.empty()) {
//
//        unsigned next_node = order_node_helper(start, turn_penalty, valid_nodes);
//
//        ordered_delivery_nodes.push_back(next_node);
//
//        for (unsigned k = 0; k < valid_nodes.size(); k++) {
//
//            if (valid_nodes[k] == next_node) {
//
//                auto itr = valid_nodes.begin() + k;
//
//                valid_nodes.erase(itr);
//            }
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
//
//        //first = false;
//    }
//
//    while (!valid_nodes.empty()) {
//
//        unsigned next_node = order_node_helper(start, turn_penalty, valid_nodes);
//
//        for (unsigned k = 0; k < valid_nodes.size(); k++) {
//
//            if (valid_nodes[k] == next_node) {
//
//                auto itr = valid_nodes.begin() + k;
//
//                valid_nodes.erase(itr);
//            }
//        }
//
//        ordered_delivery_nodes.push_back(next_node);
//
//        start = next_node;
//    }
//
//    //ordered_delivery_nodes.erase(std::unique(ordered_delivery_nodes.begin(), ordered_delivery_nodes.end()), ordered_delivery_nodes.end());
//
//    return ordered_delivery_nodes;
//}
//
//void load_nodes_time_cost(unsigned start, std::vector<unsigned> node, unsigned index, const float turn_penalty) {
//
//    std::vector<swap_node> node_vector;
//
//    std::vector<intersection> intersection_trace;
//
//    intersection_trace.resize(street_graph.number_of_intersections);
//
//    intersection_trace[start].index = start;
//
//    intersection_trace[start].cost = 0;
//
//    path_cell find_path;
//
//    find_path.cell_queue.push(intersection_trace[start]);
//
//    while (!find_path.cell_queue.empty()) {
//
//        intersection top_cell = find_path.cell_queue.top();
//
//        find_path.cell_queue.pop();
//
//        for (unsigned i = 0; i < node.size(); i++) {
//
//            if (top_cell.index == node[i]) {
//
//                ////std::cout << "Found!" <<std::endl;
//
//                swap_node temp_node;
//
//                temp_node.start = start;
//
//                temp_node.destinataion = top_cell.index;
//
//                temp_node.time_cost = top_cell.cost;
//
//                node_vector.push_back(temp_node);
//
//                auto itr = node.begin() + i;
//
//                node.erase(itr);
//
//                if (node.empty()) {
//
//                    intersection_cost_vector[index] = node_vector;
//
//                    return;
//                }
//            }
//        }
//
//
//        if (top_cell.cost <= intersection_trace[top_cell.index].cost) {
//
//            std::vector<intersection_edge_info> connected_intersection_info = connected_intersections[top_cell.index];
//
//            for (unsigned i = 0; i < connected_intersection_info.size(); i++) {
//
//                unsigned to = connected_intersection_info[i].connected_intersection;
//
//                float cost_temp = top_cell.cost + connected_intersection_info[i].travlle_time_to_next_intersection;
//
//                if (top_cell.from != -1) {
//
//                    unsigned streetID1 = getStreetSegmentInfo(top_cell.through).streetID;
//
//                    unsigned streetID2 = getStreetSegmentInfo(connected_intersection_info[i].edge_index).streetID;
//
//                    if (streetID1 != streetID2)
//                        cost_temp += turn_penalty;
//                }
//
//                if (intersection_trace[to].cost > cost_temp) {
//
//                    intersection_trace[to].cost = cost_temp;
//
//                    intersection_trace[to].index = connected_intersection_info[i].connected_intersection;
//
//                    intersection_trace[to].from = top_cell.index;
//
//                    intersection_trace[to].through = connected_intersection_info[i].edge_index;
//
//                    find_path.cell_queue.push(intersection_trace[to]);
//                }
//            }
//        }
//    }
//
//    intersection_cost_vector[index] = node_vector;
//
//    return;
//}
//
//float compute_path_cost(std::vector<unsigned> node, const float turn_penalty, const std::vector<unsigned>& depots) {
//
//    unsigned start = node[0];
//
//    unsigned end = node[node.size()-1];
//
//    unsigned start_depot = find_closest_depot(start, depots, turn_penalty);
//
//    unsigned end_depot = find_closest_depot(end, depots, turn_penalty);
//
//    std::vector<unsigned> new_node;
//
//    new_node.push_back(start_depot);
//
//    for (unsigned a = 0; a < node.size(); a++) {
//
//        new_node.push_back(node[a]);
//
//        new_node.push_back(end_depot);
//    }
//
//
//    float cost = 0;
//
//    bool finding = true;
//
//    for (unsigned i = 1; i < new_node.size(); ++i) {
//
//        unsigned from = new_node[i - 1];
//
//        unsigned to = new_node[i];
//
//        finding = true;
//
//        for (unsigned j = 0; j < intersection_cost_vector.size(); ++j) {
//
//            for (unsigned l = 0; l < intersection_cost_vector[j].size(); l++) {
//
//                if (intersection_cost_vector[j][0].start == from && finding) {
//
//                    finding = false;
//
//                    for (unsigned k = 0; k < intersection_cost_vector[j].size(); k++) {
//
//                        if (intersection_cost_vector[j][k].destinataion == to) {
//
//                            cost += intersection_cost_vector[j][k].time_cost;
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//    return cost;
//}
//
//std::vector<unsigned> extreme_greedy_order_node(const std::vector<DeliveryInfo>& deliveries,
//        const std::vector<unsigned>& depots, const float turn_penalty, std::unordered_map<IntersectionIndex,
//        std::vector<IntersectionIndex> > delivery_node_map) {
//
//    std::unordered_map<IntersectionIndex, std::vector<IntersectionIndex> > delivery_node_map_temp = delivery_node_map;
//
//    std::vector<std::vector<unsigned> > compare_path_vector;
//
//    std::vector<unsigned> compare_path_time_vector;
//
//    std::vector<unsigned> compare_path;
//
//    std::vector<unsigned> best_path;
//
//    unsigned destinationID;
//
//    //bool bad_path = false;
//
//    for (unsigned i = 0; i < depots.size(); i++) {
//
//        //std::cout << "extreme greedy round --> " << i << "\n" << std::endl;
//
//        //std::vector<unsigned> best_path;
//
//        std::vector<unsigned> best_path_temp;
//
//        std::vector<unsigned> intersection_id;
//
//        float discrete_time = 0;
//
//        unsigned start_id = depots[i];
//
//        //best_path_temp.push_back(start_id);
//
//        for (unsigned j = 0; j < deliveries.size(); j++) {
//
//            intersection_id.push_back(deliveries[j].pickUp);
//        }
//
//        while (intersection_id.size() > 0) {
//
//            float single_time = DBL_MAX;
//
//            float single_time_temp = 0;
//
//            //std::cout << "intersection_id size --> " << intersection_id.size() << "\n" << std::endl;
//
//            for (unsigned k = 0; k < intersection_cost_vector.size(); k++) {
//
//                if (intersection_cost_vector[k][0].start == start_id) {
//
//                    for (unsigned m = 0; m < intersection_cost_vector[k].size(); m++) {
//
//                        for (unsigned b = 0; b < intersection_id.size(); b++) {
//
//                            if (intersection_id[b] == intersection_cost_vector[k][m].destinataion) {
//
//                                single_time_temp = intersection_cost_vector[k][m].time_cost;
//
//                                destinationID = intersection_cost_vector[k][m].destinataion;
//
//                                if (single_time_temp < single_time) {
//
//                                    single_time = single_time_temp;
//
//                                    destinationID = intersection_cost_vector[k][m].destinataion;
//                                }
//                            }
//                        }
//                    }
//
//                    discrete_time += single_time;
//
//                    best_path_temp.push_back(destinationID);
//
//                    k = INT_MAX;
//                }
//            }
//
//            for (unsigned a = 0; a < intersection_id.size(); a++) {
//
//                if (intersection_id[a] == destinationID) {
//
//                    auto itr = intersection_id.begin() + a;
//
//                    intersection_id.erase(itr);
//
//                    //std::cout << "earsing!" << std::endl;
//
//                    intersection_id.shrink_to_fit();
//                }
//            }
//
//
//            if (delivery_node_map_temp.find(destinationID) != delivery_node_map_temp.end()) {
//
//                for (unsigned n = 0; n < delivery_node_map_temp.find(destinationID)->second.size(); n++) {
//
//                    intersection_id.push_back(delivery_node_map_temp .find(destinationID)->second[n]);
//                }
//            }
//
//            start_id = destinationID;
//
//            delivery_node_map_temp.erase(destinationID);
//        }
//
//
//        //        unsigned end_depot = find_closest_depot(destinationID, depots, turn_penalty);
//        //
//        //        for (unsigned c = 0; c < intersection_cost_vector.size(); c++) {
//        //
//        //            if (intersection_cost_vector[c][0].start == destinationID) {
//        //
//        //                for (unsigned d = 1; d < intersection_cost_vector[c].size(); d++) {
//        //
//        //                    if (intersection_cost_vector[c][d].destinataion == end_depot) {
//        //
//        //                        discrete_time += intersection_cost_vector[c][d].time_cost;
//        //                        
//        //                       d = INT_MAX;
//        //                       
//        //                       c = INT_MAX;
//        //                    }
//        //                }
//        //            }
//        //        }
////
//        //best_path_temp.push_back(end_depot);
//
//        compare_path_time_vector.push_back(discrete_time);
//
//        compare_path_vector.push_back(best_path_temp);
//    }
//
//    unsigned min_index = 0;
//
//    float min_cost = compare_path_time_vector[0];
//
//    for (unsigned i = 1; i < compare_path_time_vector.size(); i++) {
//
//        if (compare_path_time_vector[i] < min_cost) {
//
//            min_cost = compare_path_time_vector[i];
//
//            min_index = i;
//        }
//    }
//
//    return compare_path_vector[min_index];
//}
