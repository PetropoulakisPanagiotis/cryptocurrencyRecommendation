#include <iostream>
#include <vector>
#include <string>
#include "../utils/utils.h"
#include "itemToken.h"

using namespace std;

////////////////////////////////////////
/* Implementation of item token class */
////////////////////////////////////////
int ItemToken::count = 0;

/* Create item by given tokens */
ItemToken::ItemToken(vector<string>& tokens, errorCode& status){

    status = SUCCESS;

    /* Check parameters */
    if(tokens.size() == 0){
        this->id = -1;
        status = INVALID_TOKENS;
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
int ItemToken::getId(errorCode& status){

    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_ITEM_TOKEN;
        return -1;
    }

    return this->id;
}

/* Get size of tweet */
int ItemToken::getSizeTokens(errorCode& status){

    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_ITEM_TOKEN;
        return -1;
    }

    return this->tokensSize;
}

/* Get token by given index */
string ItemToken::getToken(int index, errorCode& status){

    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_ITEM_TOKEN;
        return "";
    }

    if(index < 0 || index >= this->tokensSize){
        status = INVALID_INDEX;
        return "";
    }

    return this->tokens[index];
}

/* Get all tokens */
vector<string>* ItemToken::getTokens(errorCode& status){
    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_ITEM_TOKEN;
        return NULL;
    }

    return &(this->tokens);
}
// PetropoulakisPanagiotis
