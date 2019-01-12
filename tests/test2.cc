#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "../cryptocurrencyRecommendation/utils/utils.h"
#include "../cryptocurrencyRecommendation/itemToken/itemToken.h"

using namespace std;

TEST_CASE("Running itemToken test","test2.cc"){
    vector<string> x1;
    vector<string> x2;
    errorCode status;

    x1.push_back("hi");
    x1.push_back("nikos");

    x2.push_back("hallo");
    x2.push_back("papas");

    ItemToken tweet1(x1,status);
    REQUIRE(status == SUCCESS);

    ItemToken tweet2(x2,status);
    REQUIRE(status == SUCCESS);


    string token;

    token = tweet1.getToken(0, status);
    REQUIRE(status == SUCCESS);
    REQUIRE(token == "hi");

    token = tweet2.getToken(1, status);
    REQUIRE(status == SUCCESS);
    REQUIRE(token == "papas");
}
