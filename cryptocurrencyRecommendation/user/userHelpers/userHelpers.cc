#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include "../../utils/utils.h"
#include "../../itemToken/itemToken.h"

using namespace std;

/* Check parameters of constructor */
/* Success: 0                      */
/* Failure: -1                      */
void checkParamsConstructor(int id, vector<int>& idPosts, vector<unordered_set<string> >& allCoins, vector<string>& coins, unordered_map<string, double>& lexicon, vector<ItemToken>& allPosts, errorCode& status){

    status = SUCCESS;

    if(idPosts.size() <= 0 || idPosts.size() > MAX_ID_POSTS){
        status = INVALID_ID_POSTS;
        return;
    }
    else if(allCoins.size() <= 0){
        status = INVALID_ALL_COINS;
        return;
    }
    else if(coins.size() <= 0 || allCoins.size() != coins.size()){
        status = INVALID_COINS;
        return;
    }
    else if(lexicon.size() <= 0){
        status = INVALID_COINS;
        return ;
    }
    else if(allPosts.size() <= 0 || allPosts.size() > MAX_POSTS || allPosts.size() < idPosts.size()){
        status = INVALID_POSTS;
        return;
    }
    else if(id < 0){
        status = INVALID_USER_ID;
        return;
    }
}

/* Copy id posts */
void setIdPostsConstructor(vector<int>& xIdPosts, vector<int>& yIdPosts, errorCode& status){

    status = SUCCESS;

    int i;

    /* Scan y id posts */
    for(i = 0; i < yIdPosts.size(); i++){
        if(yIdPosts[i] < 0){
            status = INVALID_ID_POSTS;
            return;
        }

        xIdPosts.push_back(yIdPosts[i]);
    } // End for
}
// PetropoulakisPanagiotis
