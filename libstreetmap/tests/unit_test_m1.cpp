#include <algorithm>
#include <set>
#include <unittest++/UnitTest++.h>
#include "unit_test_util.h"
#include "m1.h"
#include "StreetsDatabaseAPI.h"

//Unit test 1: go through the street indexes and get name
//use the name to do unit test

/*TEST(find_street_id_from_name1) {

    for (StreetIndex actual_index = 0; actual_index < getNumberOfStreets(); actual_index++) {

        std::string street_name = getStreetName(actual_index);
        std::vector<StreetIndex> expect;
        std::vector<StreetIndex> actual;

        for (StreetIndex expect_index = 0; expect_index < getNumberOfStreets(); expect_index++) {

            if (getStreetName(expect_index) == street_name)
                expect.push_back(expect_index);
        }

        actual = find_street_ids_from_name(street_name);

        CHECK_ARRAY_EQUAL(expect, actual, expect.size());
    }
}


//Unit test 2: generate a random number
//use the random number as street id -> get street name
//try other random number for 1000 times
//use the name to do unit test

TEST(find_street_id_from_name2) {

    for (unsigned count = 0; count < 1000; count++) {

        unsigned rand_street_id = rand() % (getNumberOfStreets());

        std::string rand_street_name = getStreetName(rand_street_id);

        std::vector<StreetIndex> expect;
        std::vector<StreetIndex> actual;

        for (StreetIndex expect_index = 0; expect_index < getNumberOfStreets(); expect_index++) {

            if (getStreetName(expect_index) == rand_street_name)
                expect.push_back(expect_index);
        }

        actual = find_street_ids_from_name(rand_street_name);

        CHECK_ARRAY_EQUAL(expect, actual, expect.size());
    }
}

//Unit test 3: generate a random number
//use the random number as street id -> get street name
//use the name to do unit test and call it for 1,000,000 time
//check the time constrain

TEST(find_street_id_from_name3) {

    unsigned rand_street_id = rand() % (getNumberOfStreets());

    std::string rand_street_name = getStreetName(rand_street_id);

    std::vector<StreetIndex> expect;
    std::vector<StreetIndex> actual;

    for (StreetIndex expect_index = 0; expect_index < getNumberOfStreets(); expect_index++) {

        if (getStreetName(expect_index) == rand_street_name)
            expect.push_back(expect_index);
    }

    UNITTEST_TIME_CONSTRAINT(250);

    for (unsigned count = 0; count < 1000000; count++) {

        actual = find_street_ids_from_name(rand_street_name);
    }

    CHECK_ARRAY_EQUAL(expect, actual, expect.size());

}

//Unit test 4: corner case 1
//artificially create a wired street name -> Peggiehahahah Road
//use the name to do unit test
//expect an empty vector

TEST(find_street_id_from_name4) {

    std::string test_street_name = "Peggiehahaha Road";

    std::vector<StreetIndex> expect;
    std::vector<StreetIndex> actual;

    for (StreetIndex expect_index = 0; expect_index < getNumberOfStreets(); expect_index++) {

        if (getStreetName(expect_index) == test_street_name)
            expect.push_back(expect_index);
    }

    actual = find_street_ids_from_name(test_street_name);

    CHECK_ARRAY_EQUAL(expect, actual, expect.size());
}*/


/*
 * The following unit test case is commanded since this corner case is 
 * impossible to happen (input data is string type) 
 * (number out of range does not need to be considered)
 */

//Unit test 5: corner case 2
//artificially create a street id out of range
//use the street id to do unit test
//expect an empty vector

/*TEST(find_street_id_from_name5) {

    StreetIndex test_street_id = 99e99;

    std::string test_street_name = getStreetName(test_street_id);

    std::vector<StreetIndex> expect;
    std::vector<StreetIndex> actual;

    for (StreetIndex expect_index = 0; expect_index < getNumberOfStreets(); expect_index++) {

        if (getStreetName(expect_index) == test_street_name)
            expect.push_back(expect_index);
    }

    actual = find_street_ids_from_name(test_street_name);

    CHECK_ARRAY_EQUAL(expect, actual, expect.size());
}*/


