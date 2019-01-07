#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../../utils/utils.h"
#include "../../itemToken/itemToken.h"
#include "../../item/item.h"
#include "../../user/user.h"
#include "recommendationHelpers.h"

using namespace std;

/* Check constructors parameters */
void checkParamsConstructor(vector<ItemToken>& tokenPosts, vector<Item>& vectorPosts, vector<User>& users, vector<unordered_set<string> >& allCoins, vector<string>& coins, unordered_map<string, double>& lexicon, int p, int k, errorCode& status){

    status = SUCCESS;

    if(tokenPosts.size() == 0 || tokenPosts.size() != vectorPosts.size() || tokenPosts.size() > MAX_POSTS){
        status = INVALID_POSTS;
        return;
    }

    if(vectorPosts.size() == 0){
        status = INVALID_POINTS;
        return;
    }

    if(users.size() == 0){
        status = INVALID_USER;
        return;
    }

    if(allCoins.size() == 0 || allCoins.size() != coins.size()){
        status = INVALID_ALL_COINS;
        return;
    }

    if(coins.size() == 0){
        status = INVALID_COINS;
        return;
    }

    if(lexicon.size() == 0){
        status = INVALID_LEXICON;
        return;
    }

    if(p < MIN_P){
        status = INVALID_P;
        return;
    }

    if(k < MIN_CLUSTERS || k > MAX_CLUSTERS){
        status = INVALID_CLUSTERS;
        return;
    }
}
// PetropoulakisPanagiotis
