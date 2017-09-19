/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m4_thread.h
 * Author: zhan2413
 *
 * Created on April 3, 2017, 4:09 AM
 */

#ifndef M4_THREAD_H
#define M4_THREAD_H

std::vector<unsigned> find_path_thread(const std::vector<DeliveryInfo>& deliveries, const std::vector<unsigned>& depots, const float turn_penalty);

void order_node_thread_helper(unsigned start_id, std::vector<unsigned> pick_up, const float turn_penalty, unsigned counter);

void find_path_thread_helper(const std::vector<unsigned> path_node, const std::vector<unsigned>& depots, const float turn_penalty, unsigned counter);

void compute_path_travel_time_thread_helper(const std::vector<unsigned>& path, const double turn_penalty, unsigned counter);



#endif /* M4_THREAD_H */

