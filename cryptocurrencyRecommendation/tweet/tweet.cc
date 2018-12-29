#include <iostream>
#include <vector>
#include <string>
#include "../utils/utils.h"
#include "tweet.h"

using namespace std;

///////////////////////////////////
/* Implementation of tweet class */
///////////////////////////////////
int Tweet::count = 0;

/* Create tweet by given tokens */
Tweet::Tweet(vector<string>& tokens, errorCode& status){

    status = SUCCESS;

    /* Check parameters */
    if(tokens.size() == 0){
        this->id = -1;
        status = INVALID_TWEET;
    }
    else{

        /* Set size */
        this->tokensSize = tokens.size();
        this->tokens.reserve(this->tokensSize);

        this->id = count;
        count++;

        int i;

        /* Set tokens */
        for(i = 0; i < this->tokensSize; i++)
            this->tokens.push_back(tokens[i]);
    }
}

///////////////
/* Accessors */
///////////////

/* Get id of current tweet */
int Tweet::getId(errorCode& status){

    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_TWEET;
        return -1;
    }

    return this->id;
}

/* Get size of tweet */
int Tweet::getSizeTokens(errorCode& status){

    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_TWEET;
        return -1;
    }

    return this->tokensSize;
}

/* Get token by given index */
string Tweet::getToken(int index, errorCode& status){

    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_TWEET;
        return "";
    }

    if(index < 0 || index >= this->tokensSize){
        status = INVALID_INDEX;
        return "";
    }

    return this->tokens[index];
}

/* Get all tokens */
vector<string>* Tweet::getTokens(errorCode& status){
    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_TWEET;
        return NULL;
    }

    return &(this->tokens);
}
// PetropoulakisPanagiotis
