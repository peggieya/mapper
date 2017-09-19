//#include "m4.h"
//#include "m4_helper.h"
//#include "StreetsDatabaseAPI.h"
//#include "m4_thread.h"
//#include "m1.h"
//#include "m2.h"
//#include "m3.h"
//#include "m2_helper.h"
//#include "structure.h"
//#include <string>
//#include <unordered_map>
//#include <unordered_set>
//#include <vector>
//#include <iostream>
//#include <sstream>
//#include <algorithm>
//#include <cmath>
//#include <math.h>
//
//extern streetGraph street_graph;
//
//std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries, const std::vector<unsigned>& depots, const float turn_penalty) {
//
//    std::vector<unsigned> travel_path;
//
//    //std::cout << "the imput data size is:  " << deliveries.size() <<std::endl;
//
//    if (deliveries.size() < 20) {
//
//        travel_path = find_path_thread(deliveries, depots, turn_penalty);
//    } else
//        travel_path = large_amount_search(deliveries, depots, turn_penalty);
//
//
//    //float travle_time = compute_path_travel_time(travel_path, turn_penalty);
//
//    //std::cout << "path travle time ---> " << travle_time << std::endl;
//
//    return travel_path;
//
//}