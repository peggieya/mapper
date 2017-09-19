#pragma once
#include <vector>
#include <map>

#include "m4.h"


namespace ece297test {

bool courier_path_is_legal(const std::vector<DeliveryInfo>& deliveries,
                          const std::vector<unsigned>& depots,
                          const std::vector<unsigned>& path);

bool valid_courier_problem(const std::vector<DeliveryInfo>& deliveries,
                           const std::vector<unsigned>& depots);

bool is_depot(const std::vector<unsigned>& depots,
              const unsigned intersection_id);


void pick_up_at_intersection(const std::vector<DeliveryInfo>& deliveries,
                             const std::multimap<unsigned,size_t>& intersections_to_pick_up,
                             const unsigned curr_intersection,
                             std::vector<bool>& deliveries_picked_up);

void drop_off_at_intersection(const std::vector<DeliveryInfo>& deliveries,
                              const std::multimap<unsigned,size_t>& intersections_to_drop_off,
                              const std::vector<bool>& deliveries_picked_up,
                              const unsigned curr_intersection,
                              std::vector<bool>& deliveries_dropped_off);

bool delivered_all_packages(const std::vector<DeliveryInfo>& deliveries,
                            const std::vector<bool>& deliveries_picked_up,
                            const std::vector<bool>& deliveries_dropped_off);

bool determine_start_intersection(const std::vector<unsigned>& path,
                                  const std::vector<unsigned>& depots,
                                  unsigned& start_intersection);

bool traverse_segment(const std::vector<unsigned>& path,
                      const unsigned path_idx,
                      const unsigned curr_intersection,
                      unsigned& next_intersection);

}
