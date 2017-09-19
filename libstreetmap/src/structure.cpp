#include "OSMDatabaseAPI.h"
#include "StreetsDatabaseAPI.h"
#include "structure.h"
#include "m1.h"
#include "m2_helper.h"
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <math.h>
#include <utility>

extern streetGraph street_graph;

double streetGraph::find_length(LatLon point1, LatLon point2) {

    double lagAvg = (point1.lat() + point2.lat()) / 2;

    double x1 = ((point1.lon() * cos(lagAvg * DEG_TO_RAD))) * DEG_TO_RAD;

    double y1 = (point1.lat()) * DEG_TO_RAD;

    double x2 = ((point2.lon() * cos(lagAvg * DEG_TO_RAD))) * DEG_TO_RAD;

    double y2 = (point2.lat()) * DEG_TO_RAD;

    double distance_between_two_points = (EARTH_RADIUS_IN_METERS) * (sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2)));

    return distance_between_two_points;

}

void streetGraph::build() {
    //get constant data from API and store them
    number_of_streets = getNumberOfStreets();

    number_of_segments = getNumberOfStreetSegments();

    number_of_intersections = getNumberOfIntersections();

    number_of_interest_point = getNumberOfPointsOfInterest();

    number_of_feature = getNumberOfFeatures();

    number_of_nodes = getNumberOfNodes();

    numebr_of_ways = getNumberOfWays();

    number_of_relations = getNumberOfRelations();

    IntersectionIndex intersectionCount = number_of_intersections;

    POIIndex POICount = number_of_interest_point;

    //reserve space for vectors
    //in order to insert elements to a certain position
    all_street_intersections.resize(number_of_streets);

    street_street_segments.resize(number_of_streets);

    for (IntersectionIndex intersectionID = 0; intersectionID < intersectionCount; intersectionID++) {

        std::vector<StreetSegmentIndex> street_segments; //segments index around a intersection

        std::vector<std::string> street_segment_names; //corresponding street names of intersection street segments

        std::vector<StreetIndex> intersection_segment_streetID;

        unsigned count = getIntersectionStreetSegmentCount(intersectionID);

        //find all segments around an intersection
        //find its index, corresponding street index and names
        //insert data into different vectors

        for (unsigned i = 0; i < count; i++) {

            StreetSegmentIndex segment_index = getIntersectionStreetSegment(intersectionID, i);

            StreetIndex check_streetID = getStreetSegmentInfo(segment_index).streetID;

            std::string street_name = getStreetName(check_streetID);

            street_segments.push_back(segment_index);

            street_segment_names.push_back(street_name);

            intersection_segment_streetID.push_back(getStreetSegmentInfo(segment_index).streetID);
        }



        intersection_street_segments.push_back(street_segments);
        intersection_street_segment_names.push_back(street_segment_names);


        std::sort(intersection_segment_streetID.begin(), intersection_segment_streetID.end());

        intersection_segment_streetID.erase(std::unique(intersection_segment_streetID.begin(), intersection_segment_streetID.end()), intersection_segment_streetID.end());

        intersection_segment_streetID.resize(std::distance(intersection_segment_streetID.begin(), intersection_segment_streetID.end()));

        for (unsigned countSize = 0; countSize < intersection_segment_streetID.size(); countSize++) {

            unsigned street_index = intersection_segment_streetID[countSize];

            all_street_intersections[street_index].push_back(intersectionID);

        }


        //find all intersections connected with current intersection
        //check the segment information (one-way)

        std::vector<IntersectionIndex> adjacent_intersections_to;

        adjacent_intersections_to.clear();

        for (unsigned a = 0; a < street_segments.size(); a++) {

            StreetSegmentInfo street_segment_info;

            street_segment_info = getStreetSegmentInfo(street_segments[a]);

            //the street segment is one way only 
            if (street_segment_info.oneWay == true) {

                if (street_segment_info.from == intersectionID) {

                    adjacent_intersections_to.push_back(street_segment_info.to);

                }
            }//the segment is not one way only
            else {

                if (street_segment_info.from == intersectionID) {

                    adjacent_intersections_to.push_back(street_segment_info.to);
                } else {

                    adjacent_intersections_to.push_back(street_segment_info.from);
                }
            }
        }

        std::sort(adjacent_intersections_to.begin(), adjacent_intersections_to.end());

        adjacent_intersections_to.erase(std::unique(adjacent_intersections_to.begin(), adjacent_intersections_to.end()), adjacent_intersections_to.end());

        adjacent_intersections_to.resize(std::distance(adjacent_intersections_to.begin(), adjacent_intersections_to.end()));

        adjacent_intersections.insert(std::make_pair(intersectionID, adjacent_intersections_to));

    }


    //insert POI_rtree
    for (POIIndex countPOI = 0; countPOI < POICount; countPOI++) {

        LatLon position = getPointOfInterestPosition(countPOI);
        point point_position = point(position.lat(), position.lon());
        POI_rtree.insert(std::make_pair(point_position, countPOI));

        POI_names.push_back(getPointOfInterestName(countPOI));

    }

    //insert intersection_rtree
    for (IntersectionIndex countIntersection = 0; countIntersection < intersectionCount; countIntersection++) {

        LatLon position = getIntersectionPosition(countIntersection);
        point point_intersection = point(position.lat(), position.lon());
        intersection_rtree.insert(std::make_pair(point_intersection, countIntersection));

    }

    for (unsigned street_counter = 0; street_counter < number_of_streets; street_counter++) {

        street_name_vector.push_back(getStreetName(street_counter));

    }

    for (unsigned n = 0; n < number_of_segments; n++) {

        segment_street_name.push_back(getStreetName(getStreetSegmentInfo(n).streetID));
    }

    for (unsigned p = 0; p < number_of_intersections; p++) {

        all_intersection.push_back(p);

    }

}

