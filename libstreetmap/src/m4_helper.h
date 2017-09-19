#ifndef M4_HELPER_H
#define M4_HELPER_H

#include "structure.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m4.h"
#include "m2_helper.h"
#include "structure.h"
#include "find_path.h"
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

unsigned find_closest_depot(unsigned start_id, const std::vector<unsigned>& depots, const float turn_penalty);

void two_opt(std::vector<unsigned> path_node, const std::vector<DeliveryInfo>& deliveries, const std::vector<unsigned>& depots, const float turn_penalty, unsigned key);

std::vector<unsigned> two_opt_better(std::vector<unsigned> path_node, const std::vector<DeliveryInfo>& deliveries,const std::vector<unsigned>& depots, const float turn_penalty);

bool check_path_node_valid(std::vector<unsigned> path_node, const std::vector<DeliveryInfo>& deliveries);

std::vector<unsigned> find_delivery_path(const std::vector<unsigned> path_node, const std::vector<DeliveryInfo>& deliveries, const std::vector<unsigned>& depots, const float turn_penalty);

unsigned order_node_helper(const unsigned intersect_id_start, const double turn_penalty, std::vector<unsigned> valid_nodes);

std::vector<unsigned> order_nodes(unsigned start_id, std::vector<unsigned> pick_up, std::unordered_map<IntersectionIndex, std::vector<IntersectionIndex> > delivery_node_map, const float turn_penalty);

std::vector<unsigned> large_amount_search(const std::vector<DeliveryInfo>& deliveries, const std::vector<unsigned>& depots, const float turn_penalty);

void load_nodes_time_cost(unsigned start, std::vector<unsigned> node, unsigned index, const float turn_penalty);

float compute_path_cost(std::vector<unsigned> node, const float turn_penalty, const std::vector<unsigned>& depots);

void combine_section_thread(unsigned key);

void check_valid_thread(unsigned key, const std::vector<DeliveryInfo>& deliveries);

void two_opt_sub1(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots);

void two_opt_sub2(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots);

void two_opt_sub3(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots);

void two_opt_sub4(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots);

void two_opt_sub5(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots);

void two_opt_sub6(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots);

void two_opt_sub7(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots);

void two_opt_sub8(const std::vector<DeliveryInfo> deliveries, const float turn_penalty, const std::vector<unsigned>& depots);

std::vector<unsigned> extreme_greedy_order_node(const std::vector<DeliveryInfo>& deliveries, const std::vector<unsigned>& depots, const float turn_penalty, std::unordered_map<IntersectionIndex, std::vector<IntersectionIndex> > delivery_node_map);


#endif /* M4_HELPER_H */

