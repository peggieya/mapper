#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "structure.h"
#include "m1.h"
#include "m2_helper.h"
#include "m4_helper.h"
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
#include <thread>

//global variable for conatiner in structure.h
streetGraph street_graph;
/*
 * The reason why we did not put the following containers into the structure.h
 * was that, we finished the structure.h & .cpp first
 * and then when we tried to move the following container into other files
 * many unexpected crash and bugs happened
 * thus, after group meeting, the team decided to leave them here
 */

//other global variables
std::vector<double> segment_length_vector;

std::vector<double> street_length_vector;

std::vector< std::vector<t_point> > segment_segment_position_vector;

std::unordered_map<std::string, std::vector<StreetIndex> > streetID_from_name;

std::vector<double> segment_speed_limit;

//std::vector<std::vector<swap_node> > swap_nodes;

std::vector<double> boundary;
//top + right + bottom + left

std::vector<StreetSegmentIndex> highway_segment;

std::vector<StreetSegmentIndex> main_street_segment;

std::vector<StreetSegmentIndex> loacal_street_segment;

std::vector<StreetSegmentIndex> secondary_street_segment;

std::vector<IntersectionIndex> main_intersection;

//std::vector<t_point> intersection_position_vector;

std::vector<t_point> POI_position_vector;

std::vector<std::vector<intersection_edge_info> > connected_intersections;

bool load = true;

bool load_map(std::string map_path) {

    std::cout << "start loading map" << std::endl;
    //Load your map related data structures here

    if (map_path == "/this/path/does/not/exist") {
        return false;
    }

    bool load_success = loadStreetsDatabaseBIN(map_path);

    if (load) {
        loadOSMDatabaseBIN("/cad2/ece297s/public/maps/toronto_canada.osm.bin");
        std::cout << "loading toronto_canada osm bin" << std::endl;
        load = false;
    }


    double top, right, bottom, left;
    if (load_success) {

        //if successfully load map, load date
        street_graph.build();

        //reserve space the street_length_vector
        street_length_vector.resize(street_graph.number_of_streets);

        unsigned segmentCount = street_graph.number_of_segments;

        unsigned streetCount = street_graph.number_of_streets;

        segment_segment_position_vector.resize(street_graph.number_of_segments);


        //go through all segments and load data
        for (StreetSegmentIndex segmentID = 0; segmentID < segmentCount; segmentID++) {

            double segment_length = 0;

            //get each segment information
            StreetSegmentInfo checkSegment = getStreetSegmentInfo(segmentID);

            double speed_limit = checkSegment.speedLimit;

            //segment_speed_limit: vector: segment index -> segment limited speed(double)
            segment_speed_limit.push_back(speed_limit);


            unsigned street_index = checkSegment.streetID;

            //street_street_segments: 2d vector: street index -> all segments of street
            //get the street id of the segment then put it at according index
            street_graph.street_street_segments[street_index].insert(segmentID);


            POIIndex from = checkSegment.from; //cast type

            POIIndex to = checkSegment.to; //cast type

            //start point of the segment
            LatLon intersection1 = getIntersectionPosition(from);

            //end point of the segment
            LatLon intersection2 = getIntersectionPosition(to);

            //check the curve points number of the segment
            //one curve point only: segment length = start->end point
            if (checkSegment.curvePointCount == 0) {
                segment_length_vector.push_back(find_distance_between_two_points(intersection1, intersection2));

            }//one curve point only: segment length = start->curve point->end point
            else if (checkSegment.curvePointCount == 1) {

                LatLon curvepoint = getStreetSegmentCurvePoint(segmentID, 0);

                segment_length += find_distance_between_two_points(intersection1, curvepoint);

                segment_length += find_distance_between_two_points(curvepoint, intersection2);

                segment_length_vector.push_back(segment_length);

            }//more than one curve point
                //segment length1 = sum of all curve point->curve point
                //segment length2 = start->first curve point + last curve point->end point
                //segment length = segment length1 + segment length2
            else {

                for (unsigned i = 0; i < (checkSegment.curvePointCount - 1); i++) {

                    LatLon curvepoint1 = getStreetSegmentCurvePoint(segmentID, i);

                    LatLon curvepoint2 = getStreetSegmentCurvePoint(segmentID, (i + 1));

                    if (i == 0) {
                        segment_length += find_distance_between_two_points(intersection1, curvepoint1);
                    }

                    segment_length += find_distance_between_two_points(curvepoint1, curvepoint2);

                    if (i == (checkSegment.curvePointCount - 2)) {
                        segment_length += find_distance_between_two_points(curvepoint2, intersection2);

                    }

                }
                //segment_length_vector: segment index -> segment length(double)
                segment_length_vector.push_back(segment_length);

                //std::cout << segment_length << std::endl;
            }

            if (speed_limit >= 80) {
                highway_segment.push_back(segmentID);
                main_intersection.push_back(checkSegment.from);
                main_intersection.push_back(checkSegment.to);

            } else if (speed_limit >= 55)
                main_street_segment.push_back(segmentID);
            else if (speed_limit >= 20)
                secondary_street_segment.push_back(segmentID);
            else
                loacal_street_segment.push_back(segmentID);

        }


        //go through all street and load data
        for (StreetIndex streetID = 0; streetID < streetCount; streetID++) {

            //get all segments on the street (set)
            //then transfer the set to a vector contains the segment index
            std::vector<StreetSegmentIndex> street_segments(street_graph.street_street_segments[streetID].begin(),
                    street_graph.street_street_segments[streetID].end());

            double street_length = 0;

            //street length = sum of all street segment length
            for (unsigned count = 0; count < street_segments.size(); count++) {

                unsigned segmentID = street_segments[count];

                street_length += (segment_length_vector[segmentID]);

            }
            //street_length_vector: street index -> street length(double)
            street_length_vector[streetID] = (street_length);


            //name_streetID: all streets index with the same name(street_name)
            std::vector<StreetIndex> name_streetID;

            //get street name of current street index
            std::string street_name = getStreetName(streetID);

            //try find (street_name) in the map
            //if not found->  streets with street_name haven't be loaded into the map
            //if found-> there are streets index with street_name already exist in the map
            //then insert the current street index to the according vector
            //            auto itr = streetID_from_name.find(street_name);

            streetID_from_name[street_name].push_back(streetID);

        }



        for (unsigned i = 0; i < street_graph.number_of_feature; i++) {

            std::vector<t_point> feature_point_vector;

            unsigned count = getFeaturePointCount(i);

            for (unsigned j = 0; j < count; j++) {

                LatLon feature_point = getFeaturePoint(i, j);

                t_point feature_t_point = LatLon_XY_converter(feature_point);

                feature_point_vector.push_back(feature_t_point);
                //std::cout << "x: " << feature_t_point.x << "    y: " << feature_t_point.y << std::endl;

            }
            street_graph.feature_point_vector_vector.push_back(feature_point_vector);
        }

        for (unsigned i = 0; i < street_graph.number_of_intersections; i++) {

            if (i == 0) {
                top = getIntersectionPosition(i).lat();
                right = getIntersectionPosition(i).lon();
                bottom = getIntersectionPosition(i + 1).lat();
                left = getIntersectionPosition(i + 1).lon();
            } else if (i > 1) {

                double temp_top, temp_bottom, temp_right, temp_left;

                temp_top = getIntersectionPosition(i).lat();

                temp_right = getIntersectionPosition(i).lon();

                temp_bottom = getIntersectionPosition(i).lat();

                temp_left = getIntersectionPosition(i).lon();

                if (temp_top > top)
                    top = temp_top;
                if (temp_right > right)
                    right = temp_right;
                if (temp_bottom < bottom)
                    bottom = temp_bottom;
                if (temp_left < left)
                    left = temp_left;
            }
        }

        street_graph.avgLat = (top + bottom) / 2;

        top = (top);
        bottom = (bottom);

        right = ((right * cos(street_graph.avgLat * DEG_TO_RAD)));
        left = ((left * cos(street_graph.avgLat * DEG_TO_RAD)));

        boundary.push_back(top);
        boundary.push_back(right);
        boundary.push_back(bottom);
        boundary.push_back(left);

        street_graph.boundary_box = t_bound_box(left, bottom, right, top);

        for (unsigned i = 0; i < street_graph.number_of_segments; i++) {

            StreetSegmentInfo checkSegment = getStreetSegmentInfo(i);

            POIIndex from = checkSegment.from; //cast type

            POIIndex to = checkSegment.to; //cast type

            //start point of the segment
            LatLon intersection1 = getIntersectionPosition(from);

            //std::cout << intersection1.lat() << std::endl;

            //end point of the segment
            LatLon intersection2 = getIntersectionPosition(to);

            t_point intersection1_xy = LatLon_XY_converter(intersection1);

            //std::cout << intersection1_xy.get<0>() << std::endl;

            t_point intersection2_xy = LatLon_XY_converter(intersection2);

            segment_segment_position_vector[i].push_back(intersection1_xy);


            for (unsigned j = 0; j < checkSegment.curvePointCount; j++) {

                LatLon curvepoint = getStreetSegmentCurvePoint(i, j);

                t_point curvepoint_xy = LatLon_XY_converter(curvepoint);

                segment_segment_position_vector[i].push_back(curvepoint_xy);
            }

            segment_segment_position_vector[i].push_back(intersection2_xy);
        }

        for (unsigned i = 0; i < street_graph.number_of_interest_point; i++) {

            LatLon position = getPointOfInterestPosition(i);

            float x = (position.lon() * cos(street_graph.avgLat * DEG_TO_RAD));

            float y = position.lat();

            t_point position_xy = t_point(x, y);

            street_graph.POI_position_vector.push_back(position_xy);
        }

        for (unsigned i = 0; i < street_graph.number_of_intersections; i++) {

            LatLon position = getIntersectionPosition(i);

            float x = (position.lon() * cos(street_graph.avgLat * DEG_TO_RAD));

            float y = position.lat();

            // std::cout << x << " *** " << y << std::endl;

            t_point position_xy = t_point(x, y);

            street_graph.intersection_position_vector.push_back(position_xy);
        }


        for (unsigned i = 0; i < street_graph.number_of_interest_point; i++) {

            POI_info this_POI;

            this_POI.POI_index = i;

            this_POI.POI_position = LatLon_XY_converter(getPointOfInterestPosition(i));

            this_POI.POI_name = getPointOfInterestName(i);

            this_POI.POI_type = getPointOfInterestType(i);

            this_POI.cloest_intersection = find_closest_intersection(getPointOfInterestPosition(i));

            street_graph.POI_info_vector.push_back(this_POI);

            //std::cout << this_POI.POI_position.x << " & " << this_POI.POI_position.y << std::endl;
        }

        for (unsigned counter = 0; counter < street_graph.number_of_intersections; counter++) {

            std::vector<unsigned> this_connected_intersection = street_graph.adjacent_intersections.find(counter)->second;

            std::vector<unsigned> this_connected_segment = street_graph.intersection_street_segments[counter];

            std::vector<intersection_edge_info> this_connected_edge_info;

            if (this_connected_intersection.size() > 0) {

                for (unsigned i = 0; i < this_connected_intersection.size(); i++) {
                    for (unsigned j = 0; j < this_connected_segment.size(); j++) {

                        StreetSegmentInfo checkInfo = getStreetSegmentInfo(this_connected_segment[j]);

                        if (checkInfo.from == this_connected_intersection[i] && (!checkInfo.oneWay) && checkInfo.to == counter) {

                            intersection_edge_info this_edge_info;

                            this_edge_info.connected_intersection = checkInfo.from;

                            this_edge_info.edge_index = this_connected_segment[j];

                            double this_edge_length = segment_length_vector[(this_connected_segment[j])];

                            this_edge_info.travlle_time_to_next_intersection = (this_edge_length / checkInfo.speedLimit * 3.6);

                            this_edge_info.segment_length_to_next_intersection = this_edge_length;

                            this_edge_info.streetOfSeg = checkInfo.streetID;

                            this_connected_edge_info.push_back(this_edge_info);

                        } else if (checkInfo.to == this_connected_intersection[i] && checkInfo.from == counter) {

                            intersection_edge_info this_edge_info;

                            this_edge_info.connected_intersection = checkInfo.to;

                            this_edge_info.edge_index = this_connected_segment[j];

                            double this_edge_length = segment_length_vector[(this_connected_segment[j])];

                            this_edge_info.travlle_time_to_next_intersection = (this_edge_length / checkInfo.speedLimit * 3.6);

                            this_edge_info.segment_length_to_next_intersection = this_edge_length;

                            this_edge_info.streetOfSeg = checkInfo.streetID;

                            this_connected_edge_info.push_back(this_edge_info);
                        }
                    }
                }
            }


            connected_intersections.push_back(this_connected_edge_info);

        }

//        std::cout << "Total number of intersection --->" << street_graph.number_of_intersections << std::endl;
//
//
//
//        std::vector<std::thread> load_cost_thread_group(street_graph.all_intersection.size());
//
//        intersection_cost_vector.resize(street_graph.all_intersection.size());
//        
//        for (unsigned j = 0; j < street_graph.all_intersection.size(); j++) {
//
//            //load_cost_thread_group.at(j) = std::thread(load_nodes_time_cost, street_graph.all_intersection[j], street_graph.all_intersection, j, 15);
//
//            std::thread t1(load_nodes_time_cost, street_graph.all_intersection[j], street_graph.all_intersection, j, 15);
//            std::thread t2(load_nodes_time_cost, street_graph.all_intersection[j+1], street_graph.all_intersection, j+1, 15);
//            std::thread t3(load_nodes_time_cost, street_graph.all_intersection[j+2], street_graph.all_intersection, j+2, 15);
//            std::thread t4(load_nodes_time_cost, street_graph.all_intersection[j+3], street_graph.all_intersection, j+3, 15);
//
//            t1.join();
//            t2.join();
//            t3.join();
//            t4.join();
//            
//            j += 3;
//            
//            std::cout << "Thread one round finished" << std::endl;
//        }
//        
        

//        for (unsigned a = 0; a < street_graph.all_intersection.size(); ++a) {
//
//            load_cost_thread_group.at(a).join();
//        }
        
    }

    std::cout << "Load map finished" << std::endl;

    return (load_success);
}

void close_map() {

    //Clean-up your map related data structures here

    segment_length_vector.clear();

    street_graph.intersection_street_segments.clear();

    street_graph.street_street_segments.clear();

    street_graph.all_street_intersections.clear();

    street_length_vector.clear();

    street_graph.adjacent_intersections.clear();

    street_graph.intersection_rtree.clear();

    street_graph.POI_rtree.clear();

    street_graph.feature_point_vector_vector.clear();

    street_graph.street_name_vector.clear();

    street_graph.intersection_position_vector.clear();

    street_graph.POI_info_vector.clear();

    street_length_vector.clear();

    segment_segment_position_vector.clear();

    streetID_from_name.clear();

    segment_speed_limit.clear();

    boundary.clear();

    highway_segment.clear();

    main_street_segment.clear();

    loacal_street_segment.clear();

    secondary_street_segment.clear();

    main_intersection.clear();

    //intersection_position_vector.clear();

    POI_position_vector.clear();

    connected_intersections.clear();

    closeStreetDatabase();
}

//return street id(s) from the given street name
//return a length 0 vector if name not found

std::vector<unsigned> find_street_ids_from_name(std::string street_name) {

    std::vector<StreetIndex> street_ids_from_name;

    //check corner case: name does not exist in the map    
    auto find = streetID_from_name.find(street_name);

    if (find == streetID_from_name.end()) {

        return street_ids_from_name; //return an empty vector

    }

    //input name exist
    //get data from structure: map: street name->all street id with the same name
    street_ids_from_name = streetID_from_name.find(street_name)->second;

    std::sort(street_ids_from_name.begin(), street_ids_from_name.end());
    street_ids_from_name.erase(std::unique(street_ids_from_name.begin(), street_ids_from_name.end()), street_ids_from_name.end());

    return (street_ids_from_name);

}

//return the street segments for the given intersection

std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id) {

    //corner case: input data out of range;
    if (intersection_id > street_graph.number_of_intersections) {

        std::vector<unsigned> empty_vector;

        return empty_vector;

    }
    return (street_graph.intersection_street_segments[intersection_id]);

}

// Returns the street names at the given intersection ( includes duplicate street
// names in returned vector )

std::vector<std::string> find_intersection_street_names(unsigned intersection_id) {

    //corner case: input data out of range;
    if (intersection_id > street_graph.number_of_intersections) {

        std::vector<std::string> empty_vector;

        return empty_vector;

    }

    return (street_graph.intersection_street_segment_names[intersection_id]);
}

// Returns true if you can get from intersection1 to intersection2 using a single
// street segment ( hint : check for 1 - way streets too)
// corner case : an intersection is considered to be connected to itself

bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2) {

    //corner case: input data out of range;
    if (intersection_id1 > street_graph.number_of_intersections || intersection_id2 > street_graph.number_of_intersections) {

        return false;

    }

    if (intersection_id1 == intersection_id2) {

        return true;
    }

    std::vector<IntersectionIndex> adjacent_intersections = street_graph.adjacent_intersections.find(intersection_id1)->second;


    for (unsigned i = 0; i < adjacent_intersections.size(); i++) {

        if ((intersection_id2) == adjacent_intersections[i])

            return true;
    }

    return false;
}

//Returns all intersections you can get from current intersection using a single
// street segment 

std::vector<unsigned> find_adjacent_intersections(unsigned intersection_id) {

    //corner case: input data out of range;
    if (intersection_id > street_graph.number_of_intersections) {

        std::vector<unsigned> empty_vector;

        return empty_vector;

    }

    return (street_graph.adjacent_intersections.find(intersection_id) -> second);
}

//Returns all street segments for the given street

std::vector<unsigned> find_street_street_segments(unsigned street_id) {

    //corner case: input data out of range;
    if (street_id > street_graph.number_of_streets) {

        std::vector<unsigned> empty_vector;

        return empty_vector;

    }

    std::vector<StreetSegmentIndex> street_segment_vector(street_graph.street_street_segments[street_id].begin(), street_graph.street_street_segments[street_id].end());

    return street_segment_vector;

}

// Returns all intersections along the a given street

std::vector<unsigned> find_all_street_intersections(unsigned street_id) {

    //corner case: input data out of range;
    if (street_id > street_graph.number_of_streets) {

        std::vector<unsigned> empty_vector;

        return empty_vector;

    }

    std::vector<IntersectionIndex> street_intersections = (street_graph.all_street_intersections[street_id]);

    std::sort(street_intersections.begin(), street_intersections.end());

    street_intersections.erase(std::unique(street_intersections.begin(), street_intersections.end()), street_intersections.end());

    street_intersections.resize(std::distance(street_intersections.begin(), street_intersections.end()));

    return street_intersections;

}

//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
//However street names are not guarenteed to be unique , so more than 1
//intersection id may exist

//find all street index with name1 & name2
//find all intersections on streets with name1 & name2
//find same items of the two vectors contains intersections and return them

std::vector < unsigned > find_intersection_ids_from_street_names(std::string street_name1, std::string street_name2) {


    std::vector<IntersectionIndex> intersectionIDs;

    //get all street index with street_name1
    std::vector<StreetIndex> street1IDs = find_street_ids_from_name(street_name1);

    //get all street index with street_name2
    std::vector<StreetIndex> street2IDs = find_street_ids_from_name(street_name2);

    std::vector<IntersectionIndex> intersection_name1;
    std::vector<IntersectionIndex> intersection_name2;

    //get all intersections on street (street_name1)
    for (unsigned i = 0; i < street1IDs.size(); i++) {
        std::vector<IntersectionIndex> intersection_temp1 = find_all_street_intersections(street1IDs[i]);
        intersection_name1.reserve(intersection_name1.size() + intersection_temp1.size());
        intersection_name1.insert(intersection_name1.end(), intersection_temp1.begin(), intersection_temp1.end());

    }
    //get all intersections on street (street_name2)
    for (unsigned j = 0; j < street2IDs.size(); j++) {
        std::vector<IntersectionIndex> intersection_temp2 = find_all_street_intersections(street2IDs[j]);
        intersection_name2.reserve(intersection_name1.size() + intersection_temp2.size());
        intersection_name2.insert(intersection_name2.end(), intersection_temp2.begin(), intersection_temp2.end());
    }

    //sort & find all same terms in the two vectors->same intersection points on two street-> intersection of two streets
    std::sort(intersection_name1.begin(), intersection_name1.end());
    std::sort(intersection_name2.begin(), intersection_name2.end());
    std::set_intersection(intersection_name1.begin(), intersection_name1.end(), intersection_name2.begin(), intersection_name2.end(), std::back_inserter(intersectionIDs));

    return intersectionIDs;

}


// Returns the distance between two coordinates in meters

double find_distance_between_two_points(LatLon point1, LatLon point2) {

    //convert lat, lon due to the radian of earth surface
    double lagAvg = (point1.lat() + point2.lat()) / 2;

    double x1 = ((point1.lon() * cos(lagAvg * DEG_TO_RAD))) * DEG_TO_RAD;

    double y1 = (point1.lat()) * DEG_TO_RAD;

    double x2 = ((point2.lon() * cos(lagAvg * DEG_TO_RAD))) * DEG_TO_RAD;

    double y2 = (point2.lat()) * DEG_TO_RAD;

    double distance_between_two_points = (EARTH_RADIUS_IN_METERS) * (sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2)));

    return distance_between_two_points;

}



// Returns the length of the given street segment in meters

double find_street_segment_length(unsigned street_segment_id) {

    //corner case: input data out of range;
    if (street_segment_id > street_graph.number_of_segments) {

        return 0;

    }

    return (segment_length_vector[street_segment_id]);
}


// Returns the length of the specified street in meters

double find_street_length(unsigned street_id) {

    //corner case: input data out of range;
    if (street_id > street_graph.number_of_streets) {

        std::vector<unsigned> empty_vector;

        return 0;

    }

    return (street_length_vector[street_id]);
}

//Returns the travel time to drive a street segment in seconds ( time = distance /
//speed_limit )

double find_street_segment_travel_time(unsigned street_segment_id) {

    //corner case: input data out of range;
    if (street_segment_id > street_graph.number_of_segments) {

        std::vector<unsigned> empty_vector;

        return 0;

    }

    //convert the unit km/h -> m/s
    return (segment_length_vector[street_segment_id] / (segment_speed_limit[street_segment_id]) * 3.6);
}


// Returns the nearest point of interest to the given position

unsigned find_closest_point_of_interest(LatLon my_position) {

    //transfer input my_position to point type
    double x_my_position = my_position.lat();
    double y_my_position = my_position.lon();

    point target = point(x_my_position, y_my_position);

    std::vector<POI_position> possiable_POI;

    //if the system is small (has less than 10 POI)
    //only one potential point is enough
    if (street_graph.POI_rtree.size() < 10) {

        street_graph.POI_rtree.query(bgi::nearest(target, 1), std::back_inserter(possiable_POI));

        return (possiable_POI[0].second);

    }


    //since coordinates in tree are in cartesian
    //find 20 potential points & comparing then by distance
    //calculate the distance considering the radian of earth surface
    street_graph.POI_rtree.query(bgi::nearest(target, 20), std::back_inserter(possiable_POI));

    unsigned counter = 1;

    POIIndex closest_POI = possiable_POI[0].second;

    LatLon possiable_POI_position = getPointOfInterestPosition(possiable_POI[0].second);


    double shortest_distance = find_distance_between_two_points(my_position, possiable_POI_position);

    while (counter < 20) {

        possiable_POI_position = getPointOfInterestPosition(possiable_POI[counter].second);

        double compare_distance = find_distance_between_two_points(my_position, possiable_POI_position);

        if (compare_distance < shortest_distance) {

            shortest_distance = compare_distance;
            closest_POI = possiable_POI[counter].second;
        }

        counter++;

    }

    return (closest_POI);

}


// Returns the the nearest intersection to the given position

unsigned find_closest_intersection(LatLon my_position) {

    //transfer input my_position to point type
    double x_my_position = my_position.lat();
    double y_my_position = my_position.lon();

    point target = point(x_my_position, y_my_position);

    std::vector<intersection_position> possible_Intersection;


    //since there are large amount of intersections and are more intensive
    //find 30 potential points and use same method as POI to find the closest one

    street_graph.intersection_rtree.query(bgi::nearest(target, 30), std::back_inserter(possible_Intersection));

    unsigned counter = 1;

    IntersectionIndex closest_intersection;

    LatLon possiable_intersection_position = getIntersectionPosition(possible_Intersection[0].second);

    closest_intersection = possible_Intersection[0].second;

    double shortest_distance = find_distance_between_two_points(my_position, possiable_intersection_position);

    while (counter < 30) {

        possiable_intersection_position = getIntersectionPosition(possible_Intersection[counter].second);

        double compare_distance = find_distance_between_two_points(my_position, possiable_intersection_position);
        if (compare_distance < shortest_distance) {
            shortest_distance = compare_distance;
            closest_intersection = possible_Intersection[counter].second;
        }

        counter++;

    }
    return (closest_intersection);
}

