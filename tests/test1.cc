#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../cryptocurrencyRecommendation/itemToken/itemToken.h"
#include "../cryptocurrencyRecommendation/utils/utils.h"
#include "../cryptocurrencyRecommendation/user/user.h"

using namespace std;

TEST_CASE("Running user test","test1.cc"){
    errorCode status;
    vector<string> coins{"bitcoin", "xp", "eth", "urio"};
    vector<unordered_set<string> > allCoins({
                                    {"bitcoin", "bitc"},
                                    {"xp","$xp"},
                                    {"eth", "$eth"},
                                    {"urio", "$urio"}});

    unordered_map<string, double> lexicon({
                                    {"good", 0.7},
                                    {"bad", -0.7},
                                    {"very", 0.8},
                                    {"cool", 0.55},
                                    {"worst", -1}});

    vector<ItemToken> allPosts;
    vector<string> tokens;

    /* Fix posts */
    tokens.push_back("good");
    tokens.push_back("very");
    tokens.push_back("eth");
    allPosts.push_back(ItemToken(tokens, status));

    tokens.clear();
    tokens.push_back("bad");
    tokens.push_back("xp");
    allPosts.push_back(ItemToken(tokens, status));

    tokens.clear();
    tokens.push_back("cool");
    tokens.push_back("$urio");
    allPosts.push_back(ItemToken(tokens, status));

    tokens.clear();
    tokens.push_back("very");
    tokens.push_back("bad");
    tokens.push_back("bitcoin");
    allPosts.push_back(ItemToken(tokens, status));

    /* Create users */
    vector<int> idPosts;

    idPosts.push_back(0);
    idPosts.push_back(1);

    int x = 0;

    User* user1;
    User* user2;

    user1 = new User(x, idPosts, allCoins, coins, lexicon, allPosts, status);
    REQUIRE(status == SUCCESS);

    idPosts.clear();
    idPosts.push_back(2);
    idPosts.push_back(3);

    user2 = new User(5, idPosts, allCoins, coins, lexicon, allPosts, status);
    REQUIRE(status == SUCCESS);


    /* Check ids */
    int id1, id2;

    id1 = user1->getId(status);
    REQUIRE(status == SUCCESS);
    REQUIRE(id1 == 0);

    id2 = user2->getId(status);
    REQUIRE(status == SUCCESS);
    REQUIRE(id2 == 5);


    delete user1;
    delete user2;
}


// Petropoulakis Panagiotis
