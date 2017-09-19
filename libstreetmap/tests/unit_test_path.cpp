#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;

struct MapFixture {
    MapFixture() {
        rng = std::minstd_rand(4);
        rand_intersection = std::uniform_int_distribution<unsigned>(0, getNumberOfIntersections()-1);
        rand_street = std::uniform_int_distribution<unsigned>(1, getNumberOfStreets()-1);
        rand_segment = std::uniform_int_distribution<unsigned>(0, getNumberOfStreetSegments()-1);
        rand_poi = std::uniform_int_distribution<unsigned>(0, getNumberOfPointsOfInterest()-1);
        rand_lat = std::uniform_real_distribution<double>(43.48, 43.91998);
        rand_lon = std::uniform_real_distribution<double>(-79.78998, -79.00001);
        rand_turn_penalty = std::uniform_real_distribution<double>(0., 30.);
    }

    std::minstd_rand rng;
    std::uniform_int_distribution<unsigned> rand_intersection;
    std::uniform_int_distribution<unsigned> rand_street;
    std::uniform_int_distribution<unsigned> rand_segment;
    std::uniform_int_distribution<unsigned> rand_poi;
    std::uniform_real_distribution<double> rand_lat;
    std::uniform_real_distribution<double> rand_lon;
    std::uniform_real_distribution<double> rand_turn_penalty;
};

 TEST(find_path_between_intersections_legality) {
        std::vector<unsigned> path;
                        
        path = find_path_between_intersections(35806, 25937, 0);
        CHECK(path_is_legal(35806, 25937, path));
       
        
        /*path = find_path_between_intersections(20510, 76659, 0);
        CHECK(path_is_legal(20510, 76659, path));*/
 }

/*SUITE(inter_inter_path_func_public) {
    TEST(find_path_between_intersections_legality) {
        std::vector<unsigned> path;
        
        path = find_path_between_intersections(20510, 76659, 0);
        CHECK(path_is_legal(20510, 76659, path));
        
        path = find_path_between_intersections(25454, 6484, 15);
        CHECK(path_is_legal(25454, 6484, path));
        
        path = find_path_between_intersections(6521, 64448, 0);
        CHECK(path_is_legal(6521, 64448, path));
        
        path = find_path_between_intersections(31204, 83543, 15);
        CHECK(path_is_legal(31204, 83543, path));
        
        path = find_path_between_intersections(35806, 25937, 0);
        CHECK(path_is_legal(35806, 25937, path));
        
        path = find_path_between_intersections(38992, 26296, 0);
        CHECK(path_is_legal(38992, 26296, path));
        
        path = find_path_between_intersections(40025, 5100, 0);
        CHECK(path_is_legal(40025, 5100, path));
        
        path = find_path_between_intersections(40237, 40770, 15);
        CHECK(path_is_legal(40237, 40770, path));
        
        path = find_path_between_intersections(44133, 61663, 0);
        CHECK(path_is_legal(44133, 61663, path));
        
        path = find_path_between_intersections(47674, 79363, 15);
        CHECK(path_is_legal(47674, 79363, path));
        
        path = find_path_between_intersections(50814, 34461, 15);
        CHECK(path_is_legal(50814, 34461, path));
        
        path = find_path_between_intersections(55141, 106719, 0);
        CHECK(path_is_legal(55141, 106719, path));
        
        path = find_path_between_intersections(65097, 66621, 0);
        CHECK(path_is_legal(65097, 66621, path));
        
        path = find_path_between_intersections(65984, 92110, 0);
        CHECK(path_is_legal(65984, 92110, path));
        
        path = find_path_between_intersections(68296, 19170, 0);
        CHECK(path_is_legal(68296, 19170, path));
        
        path = find_path_between_intersections(69186, 105509, 15);
        CHECK(path_is_legal(69186, 105509, path));
        
        path = find_path_between_intersections(71722, 47022, 0);
        CHECK(path_is_legal(71722, 47022, path));
        
        path = find_path_between_intersections(81098, 71692, 0);
        CHECK(path_is_legal(81098, 71692, path));
        
        path = find_path_between_intersections(82359, 19278, 15);
        CHECK(path_is_legal(82359, 19278, path));
        
        path = find_path_between_intersections(9, 37026, 0);
        CHECK(path_is_legal(9, 37026, path));
        
        path = find_path_between_intersections(63326, 17323, 15);
        CHECK(path_is_legal(63326, 17323, path));
        
        path = find_path_between_intersections(43347, 41335, 15);
        CHECK(path_is_legal(43347, 41335, path));
        
        path = find_path_between_intersections(82230, 70534, 15);
        CHECK(path_is_legal(82230, 70534, path));
        
        path = find_path_between_intersections(94906, 82598, 0);
        CHECK(path_is_legal(94906, 82598, path));
        
        path = find_path_between_intersections(97367, 13222, 15);
        CHECK(path_is_legal(97367, 13222, path));
        
        path = find_path_between_intersections(98501, 94321, 0);
        CHECK(path_is_legal(98501, 94321, path));
        
        path = find_path_between_intersections(36562, 78964, 15);
        CHECK(path_is_legal(36562, 78964, path));
        
        path = find_path_between_intersections(104205, 27925, 15);
        CHECK(path_is_legal(104205, 27925, path));
        
        path = find_path_between_intersections(5619, 106035, 0);
        CHECK(path_is_legal(5619, 106035, path));
        
        path = find_path_between_intersections(105542, 9948, 15);
        CHECK(path_is_legal(105542, 9948, path));
        
    } //find_path_between_intersections_legality

    TEST(find_path_between_intersections_optimality) {
        std::vector<unsigned> path;
        path = find_path_between_intersections(20510, 76659, 0);
        CHECK(path_is_legal(20510, 76659, path));
        CHECK(compute_path_travel_time(path, 0) <= 1349.260787042346);
        
        path = find_path_between_intersections(25454, 6484, 15);
        CHECK(path_is_legal(25454, 6484, path));
        CHECK(compute_path_travel_time(path, 15) <= 671.4928163085616);
        
        path = find_path_between_intersections(6521, 64448, 0);
        CHECK(path_is_legal(6521, 64448, path));
        CHECK(compute_path_travel_time(path, 0) <= 733.4867700911947);
        
        path = find_path_between_intersections(31204, 83543, 15);
        CHECK(path_is_legal(31204, 83543, path));
        CHECK(compute_path_travel_time(path, 15) <= 2530.201870638255);
        
        path = find_path_between_intersections(35806, 25937, 0);
        CHECK(path_is_legal(35806, 25937, path));
        CHECK(compute_path_travel_time(path, 0) <= 2456.324680341101);
        
        path = find_path_between_intersections(38992, 26296, 0);
        CHECK(path_is_legal(38992, 26296, path));
        CHECK(compute_path_travel_time(path, 0) <= 831.794412213809);
        
        path = find_path_between_intersections(40025, 5100, 0);
        CHECK(path_is_legal(40025, 5100, path));
        CHECK(compute_path_travel_time(path, 0) <= 1674.970375792561);
        
        path = find_path_between_intersections(40237, 40770, 15);
        CHECK(path_is_legal(40237, 40770, path));
        CHECK(compute_path_travel_time(path, 15) <= 211.7101302022182);
        
        path = find_path_between_intersections(44133, 61663, 0);
        CHECK(path_is_legal(44133, 61663, path));
        CHECK(compute_path_travel_time(path, 0) <= 1813.572057019503);
        
        path = find_path_between_intersections(47674, 79363, 15);
        CHECK(path_is_legal(47674, 79363, path));
        CHECK(compute_path_travel_time(path, 15) <= 1004.14011971932);
        
        path = find_path_between_intersections(50814, 34461, 15);
        CHECK(path_is_legal(50814, 34461, path));
        CHECK(compute_path_travel_time(path, 15) <= 2020.170734639228);
        
        path = find_path_between_intersections(55141, 106719, 0);
        CHECK(path_is_legal(55141, 106719, path));
        CHECK(compute_path_travel_time(path, 0) <= 1818.959928943403);
        
        path = find_path_between_intersections(65097, 66621, 0);
        CHECK(path_is_legal(65097, 66621, path));
        CHECK(compute_path_travel_time(path, 0) <= 566.2457329058725);
        
        path = find_path_between_intersections(65984, 92110, 0);
        CHECK(path_is_legal(65984, 92110, path));
        CHECK(compute_path_travel_time(path, 0) <= 563.2849733751518);
        
        path = find_path_between_intersections(68296, 19170, 0);
        CHECK(path_is_legal(68296, 19170, path));
        CHECK(compute_path_travel_time(path, 0) <= 1314.984229914835);
        
        path = find_path_between_intersections(69186, 105509, 15);
        CHECK(path_is_legal(69186, 105509, path));
        CHECK(compute_path_travel_time(path, 15) <= 2227.683933366302);
        
        path = find_path_between_intersections(71722, 47022, 0);
        CHECK(path_is_legal(71722, 47022, path));
        CHECK(compute_path_travel_time(path, 0) <= 2109.715675148193);
        
        path = find_path_between_intersections(81098, 71692, 0);
        CHECK(path_is_legal(81098, 71692, path));
        CHECK(compute_path_travel_time(path, 0) <= 589.0802038101533);
        
        path = find_path_between_intersections(82359, 19278, 15);
        CHECK(path_is_legal(82359, 19278, path));
        CHECK(compute_path_travel_time(path, 15) <= 1177.726682558728);
        
        path = find_path_between_intersections(9, 37026, 0);
        CHECK(path_is_legal(9, 37026, path));
        CHECK(compute_path_travel_time(path, 0) <= 1788.7078797374);
        
        path = find_path_between_intersections(63326, 17323, 15);
        CHECK(path_is_legal(63326, 17323, path));
        CHECK(compute_path_travel_time(path, 15) <= 1813.269876448359);
        
        path = find_path_between_intersections(43347, 41335, 15);
        CHECK(path_is_legal(43347, 41335, path));
        CHECK(compute_path_travel_time(path, 15) <= 953.1213593447345);
        
        path = find_path_between_intersections(82230, 70534, 15);
        CHECK(path_is_legal(82230, 70534, path));
        CHECK(compute_path_travel_time(path, 15) <= 1216.308623932535);
        
        path = find_path_between_intersections(94906, 82598, 0);
        CHECK(path_is_legal(94906, 82598, path));
        CHECK(compute_path_travel_time(path, 0) <= 1725.37523894484);
        
        path = find_path_between_intersections(97367, 13222, 15);
        CHECK(path_is_legal(97367, 13222, path));
        CHECK(compute_path_travel_time(path, 15) <= 2165.172949643942);
        
        path = find_path_between_intersections(98501, 94321, 0);
        CHECK(path_is_legal(98501, 94321, path));
        CHECK(compute_path_travel_time(path, 0) <= 15.72894735513906);
        
        path = find_path_between_intersections(36562, 78964, 15);
        CHECK(path_is_legal(36562, 78964, path));
        CHECK(compute_path_travel_time(path, 15) <= 2642.390528354219);
        
        path = find_path_between_intersections(104205, 27925, 15);
        CHECK(path_is_legal(104205, 27925, path));
        CHECK(compute_path_travel_time(path, 15) <= 391.9550187281172);
        
        path = find_path_between_intersections(5619, 106035, 0);
        CHECK(path_is_legal(5619, 106035, path));
        CHECK(compute_path_travel_time(path, 0) <= 1709.351224976835);
        
        path = find_path_between_intersections(105542, 9948, 15);
        CHECK(path_is_legal(105542, 9948, path));
        CHECK(compute_path_travel_time(path, 15) <= 796.2531812350959);
        
    } //find_path_between_intersections_optimality

} //inter_inter_path_func_public*/

