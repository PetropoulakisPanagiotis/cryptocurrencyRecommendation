#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
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

    /* Initialize sentimen and unknownCoins */
    vector<string>& coinsRef = *this->coins;
    for(i = 0; i < coinsRef.size(); i++){
        this->unknownCoins.push_back(0);
        this->sentiment.push_back(0);
    } // End for

    /* Fix sentiment */
    fixSentimentConstructor(this->sentiment, this->unknownCoins, this->idPosts, this->allCoins, this->coins, this->lexicon, this->allPosts, status);
    if(status != SUCCESS){
        this->id = -1;
        return;
    }
}


/* Recommend the best(p) coins in current user based on given neighbors */
void User::recommend(int p, vector<User>& neighborUsers, vector<int>& newCoins, errorCode& status){
    int i, totalUnknown = 0;
    double similaritySum = 0;
    double normalizingFactor = 0, currVal;
    vector<newCoinNode> coinsHeap; // Keep p best coins

    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_USER;
        return;
    }

    /* Check parameters */
    if(p <= 0 || p > neighborUsers.size()){
        status = INVALID_P;
        return;
    }

    if(neighborUsers.size() == 0){
        status = INVALID_USER_NEIGHBORS;
        return;
    }

    /* Reset new coins */
    newCoins.clear();

    /* Find similarity sum only once with neighbors */
    for(i = 0; i < neighborUsers.size(); i++){
        similaritySum += similarityFunc(this->sentiment, neighborUsers[i].sentiment, status);
        if(status != SUCCESS)
            return; 
    } // End for

    /* Fix normalizing factor */
    if(similaritySum == 0){
        status = DIV_OVERFLOW;
        return;
    }

    normalizingFactor = 1 / similaritySum;

    /* For every unkown coin predict user's behavior */
    for(i = 0; this->unknownCoins.size(); i++){

        /* Known coin */
        if(this->unknownCoins[i] == 1)
            continue;
        else
            totalUnknown++;

        currVal = normalizingFactor * similaritySum * this->sentiment[i];

        coinsHeap.push_back(newCoinNode(i, currVal));
    } // End for - unknown coins

    /* Create heap */
    make_heap(coinsHeap.begin(), coinsHeap.end(), newCoinNodeCompare());

    /* Fix p if unkown coins are less than p */
    if(p > totalUnknown)
        p = totalUnknown;

    /* Exctract best p coins */
    for(i = 0; i < p; i++)
        newCoins.push_back(coinsHeap.front().pos);
}

////////////////
/*  Accessors */
////////////////

/* Get user id */
int User::getId(errorCode& status){

    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_USER;
        return -1;
    }

    return this->id;
}

/* Get all ids of posts */
vector<int>* User::getIdPosts(errorCode& status){

    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_USER;
        return NULL;
    }

    return &this->idPosts;
}

/* Get sentiment of the user */
vector<double>* User::getSentiment(errorCode& status){

    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_USER;
        return NULL;
    }

    return &this->sentiment;
}

/* Get indexes of unknown coins of the user */
vector<int>* User::getUnknownCoins(errorCode& status){

    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_USER;
        return NULL;
    }

    return &this->unknownCoins;
}

/* Get unknown value of given index */
int User::getUnknownCoin(int index, errorCode& status){
    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_USER;
        return -1;
    }

    if(index < 0 || index >= this->unknownCoins.size()){
        status = INVALID_INDEX;
        return -1;
    }

    return this->unknownCoins[index];
}
/* Get number of post */
int User::getSizeOfPosts(errorCode& status){

    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_USER;
        return -1;
    }

    return this->totalPosts;
}

// PetropoulakisPanagiotis
