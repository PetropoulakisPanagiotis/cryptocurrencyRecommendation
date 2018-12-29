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
