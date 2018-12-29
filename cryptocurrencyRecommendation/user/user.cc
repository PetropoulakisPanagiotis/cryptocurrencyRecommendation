#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../itemToken/itemToken.h"
#include "../utils/utils.h"
#include "userHelpers/userHelpers.h"
#include "user.h"

using namespace std;

///////////////////////////////////
/* Implementations of ucer class */
///////////////////////////////////

/* Constructor: set members, find overall sentiment and normalize it */
User::User(int id, std::vector<int>& idPosts, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, std::unordered_map<std::string, double>& lexicon, std::vector<ItemToken>& allPosts, errorCode& status){
    int i;

    status = SUCCESS;

    /* Check parameters */
    checkParamsConstructor(id, idPosts, allCoins, coins, lexicon, allPosts, status);
    if(status != SUCCESS){
        this->id = -1;
        return;
    }

    /* Set members */
    this->id = id;

    setIdPostsConstructor(this->idPosts, idPosts, status);
    if(status != SUCCESS){
        this->id = -1;
        return;
    }

    this->allCoins = &allCoins;
    this->coins = &coins;
    this->lexicon = &lexicon;
    this->allPosts = &allPosts;
    this->totalPosts = this->idPosts.size();

    /* Set unknown coins */
    vector<string>& coinsRef = &this->coins;
    for(i = 0; i < this->coinsRef.size(); i++)
        this->unknownCoins.push_back(0);

    /* Fix sentiment */
    fixSentiment(this->sentiment, this->unknownCoins, this->idPosts, this->allCoins, this->coins, this->lexicon, this->allPosts, status);
    if(status != SUCCESS){
        this->id = -1;
        return;
    }
}

// PetropoulakisPanagiotis
