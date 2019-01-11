#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <random>
#include "../itemToken/itemToken.h"
#include "../utils/utils.h"
#include "userHelpers/userHelpers.h"
#include "user.h"
#include <unistd.h>

using namespace std;

///////////////////////////////////
/* Implementations of user class */
///////////////////////////////////

/* Constructor: set members, find overall sentiment, scale and normalize it */
User::User(int id, vector<int>& idPosts, vector<unordered_set<string> >& allCoins, vector<string>& coins, unordered_map<string, double>& lexicon, vector<ItemToken>& allPosts, errorCode& status){
    int i;

    status = SUCCESS;

    /* Check parameters */
    checkParamsConstructor(id, idPosts, allCoins, coins, lexicon, allPosts, status);
    if(status != SUCCESS){
        this->id = -1;
        return;
    }

    /////////////////
    /* Set members */
    /////////////////

    this->id = id;
    this->allCoins = &allCoins;
    this->coins = &coins;
    this->lexicon = &lexicon;
    this->allPosts = &allPosts;
    this->totalPosts = idPosts.size();

    setIdPostsConstructor(this->idPosts, idPosts, status);
    if(status != SUCCESS){
        this->id = -1;
        return;
    }

    /* Initialize sentiment and unknownCoins */
    vector<string>& coinsRef = *this->coins;

    for(i = 0; i < (int)coinsRef.size(); i++){
        this->unknownCoins.push_back(0);
        this->sentiment.push_back(0);
    } // End for

    int flag;

    flag = fixSentimentConstructor(this->sentiment, this->unknownCoins, this->idPosts, *this->allCoins, *this->coins, *this->lexicon, *this->allPosts, status);
    if(status != SUCCESS){
        this->id = -1;
        return;
    }

    if(flag == 0)
        this->invalid = 1; // Invalid user - no information
    else{
        this->invalid = 0;

        /* Find avg value of sentiment */
        int totalActiveCoins = 0;
        this->avgSentiment = 0;

        for(i = 0; i < (int)this->sentiment.size(); i++){
            if(this->unknownCoins[i] != 0){
                this->avgSentiment += this->sentiment[i];
                totalActiveCoins++;
            }
        } // End for - FInd avg sentiment

        if(totalActiveCoins != 0)
            this->avgSentiment /= (double)totalActiveCoins;

        /* Copy avg into unknown coins */
        for(i = 0; i < (int)this->sentiment.size(); i++){
            if(this->unknownCoins[i] == 0)
                this->sentiment[i] = this->avgSentiment;
        } // End for

    }
}

/* Recommend the best(p) coins in current user based on given neighbors */
void User::recommend(int p, vector<User*>& neighborUsers, vector<int>& newCoins, int metrice, errorCode& status){
    int i, totalUnknown = 0, j; // Total unknown coins
    double similaritySum = 0, normalizingFactor = 0, currVal, simpleSimilaritySum;
    vector<newCoinNode> coinsHeap; // Keep p best coins based on score

    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_USER;
        return;
    }

    /* Check parameters */
    if(p <= 0 || p > (int)this->coins->size()){
        status = INVALID_P;
        return;
    }

    if(metrice != 0 && metrice != 1){
        status = INVALID_METRICE;
        return;
    }

    /* Reset new coins */
    newCoins.clear();

    /* Check if user is invalid */
    if(this->invalid == 1){
        /* Set random engine */
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator(seed);
        uniform_real_distribution<float> uniformDist(0, this->coins->size() - 1); // [0, coins-1]

        /* Do not pick same coin twice */
        unordered_set<int> visited;
        unordered_set<int>::iterator iterVisited;
        int currPos;

        /* Exctract p random coins */
        for(i = 0; i < p; i++){

            currPos = uniformDist(generator);

            /* Check visited */
            iterVisited = visited.find(currPos);

            if(iterVisited == visited.end()){
                newCoins.push_back(currPos);
                visited.insert(currPos);
            }
            else
                i -= 1;
        } // End for
    }
    else{

        if(neighborUsers.size() == 0){
            status = INVALID_USER_NEIGHBORS;
            return;
        }

        /* For every unkown coin predict user's behavior */
        for(i = 0; i < this->unknownCoins.size(); i++){
            /* Known coin */
            if(this->unknownCoins[i] == 1)
                continue;
            else
                totalUnknown++;

            /* Reset  */
            similaritySum = 0;
            simpleSimilaritySum = 0;

            /* Scan users */
            for(j = 0; j < (int)neighborUsers.size(); j++){

                /* Discard invalid users */
                if(neighborUsers[j]->invalid == 1)
                    continue;

                currVal = similarityFunc(this->sentiment, neighborUsers[j]->sentiment, metrice, status);
                if(status != SUCCESS)
                    return;
                simpleSimilaritySum += currVal;
                currVal = currVal * (neighborUsers[j]->sentiment[i] - neighborUsers[j]->avgSentiment);
                similaritySum += currVal;
            } // End for scan users

            if(simpleSimilaritySum == 0){
                status = DIV_OVERFLOW;
                return;
            }

            /* Fix normalizing factor */
            normalizingFactor = 1 / similaritySum;
            currVal = this->avgSentiment + (normalizingFactor * similaritySum);

            /* Add new coin in heap */
            coinsHeap.push_back(newCoinNode(i, currVal));
        } // End for - unknown coins

        /* All coins are known */
        if(totalUnknown == 0)
            return;

        /* Create heap */
        make_heap(coinsHeap.begin(), coinsHeap.end(), newCoinNodeCompare());

        /* Fix p if unkown coins are less than p */
        if(p > totalUnknown)
            p = totalUnknown;

        /* Exctract best p coins */
        for(i = 0; i < p; i++)
            newCoins.push_back(coinsHeap.front().pos);
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

/* Get unknown value of given index */
int User::getUnknownCoin(int index, errorCode& status){
    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_USER;
        return -1;
    }

    if(index < 0 || index >= (int)this->unknownCoins.size()){
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

/* Get status of user */
int User::getStatus(errorCode& status){

    status = SUCCESS;

    if(this->id == -1){
        status = INVALID_USER;
        return -1;
    }

    return this->invalid;
}
// PetropoulakisPanagiotis
