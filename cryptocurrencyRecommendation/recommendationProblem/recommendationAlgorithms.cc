#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include "../utils/utils.h"
#include "../itemToken/itemToken.h"
#include "../item/item.h"
#include "../user/user.h"
#include "../clusteringProblem/cluster.h"
#include "../neighborsProblem/model/model.h"
#include "../neighborsProblem/model/lsh/lsh.h"
#include "helpers/recommendationHelpers.h"
#include "recommendation.h"

using namespace std;

//////////////////////////////////////////////////
/* Implementations of recommendation algorithms */
//////////////////////////////////////////////////

/* Find neighbor users with lsh and predict coins */
void recommendation::recommendationLshUsers(int coinsReturned, errorCode& status){
    /* Every user is represented with it's sentiment */
    list<Item> sentimentUsers;
    list<Item>::iterator iterSentimentUsers;
    string currId;
    int i, valid;

    status = SUCCESS;

    /* Get sentiment of users */
    for(i = 0; i < this->usersSize; i++){

        /* Discard invalid users */
        valid = this->users[i].getStatus(status);
        if(status != SUCCESS)
            return;

        /* Invalid user */
        if(valid == 1)
            continue;

        /* Get sentiment */
        vector<double>* sentimentUser;

        sentimentUser = this->users[i].getSentiment(status);
        if(status != SUCCESS)
            return;

        currId = to_string(i);

        /* Fix sentiment list */
        sentimentUsers.push_back(Item(currId, *sentimentUser, status));
        if(status != SUCCESS)
            return;
    } // End for

    /* Pick model */
    model* lshUsersModel;

    /* Create model */
    lshUsersModel = new lshCosine();

    /* Fit model */
    lshUsersModel->fit(sentimentUsers, status);
    if(status != SUCCESS)
            return;

    iterSentimentUsers = sentimentUsers.begin();

    /* For every user find it's neighbors and predict coins */
    for(i = 0; i < this->usersSize; i++){
        vector<User*> neighborUsers;
        vector<int> newCoins;
        list<int> neighborsIds;
        list<int>::iterator iterNeighborsIds;

        /* Get status of user */
        valid = this->users[i].getStatus(status);
        if(status != SUCCESS)
            return;

        /* Invalid user */
        if(valid == 1){
            /* Recommend random coins */
            this->users[i].recommend(coinsReturned, neighborUsers, newCoins, status);
            if(status != SUCCESS)
                return;

            /* Fix vector of predicted */
            predictedLshUsers.push_back(newCoins);

            iterSentimentUsers++;
            continue;
        }
        /* Find neighbors */
        lshUsersModel->simpleNeighbors(*iterSentimentUsers, neighborsIds, this->p, status);
        if(status != SUCCESS)
            return;

        /* Scan neighborsIds and fix neighbors users */
        for(iterNeighborsIds = neighborsIds.begin(); iterNeighborsIds != neighborsIds.end(); iterNeighborsIds++){
            neighborUsers.push_back(&(this->users[*iterNeighborsIds]));
        } // End for - scan neighborsIds

        /* Recommend coins */
        this->users[i].recommend(coinsReturned, neighborUsers, newCoins, status);
        if(status != SUCCESS)
            return;

        /* Fix vector of predicted */
        predictedLshUsers.push_back(newCoins);
        iterSentimentUsers++;
    } // End for - Predict coins for users
}

/* Find neighbor pseudo users with lsh and predict coins for users */
void recommendation::recommendationLshPseudoUsers(int coinsReturned, errorCode& status){
    /* Every pseudo user is represented with it's sentiment */
    list<Item> sentimentPseudoUsers;
    list<Item>::iterator iterSentimentPseudoUsers;
    string currId;
    int i, valid;

    status = SUCCESS;

    /* Get sentiment of pseudo users */
    for(i = 0; i < this->pseudoUsersSize; i++){

        /* Discard invalid pseudo users */
        valid = this->pseudoUsers[i].getStatus(status);
        if(status != SUCCESS)
            return;

        /* Invalid pseudo user */
        if(valid == 1)
            continue;

        /* Get sentiment */
        vector<double>* sentimentPseudoUser;

        sentimentPseudoUser = this->pseudoUsers[i].getSentiment(status);
        if(status != SUCCESS)
            return;

        currId = to_string(i);

        /* Fix sentiment list */
        sentimentPseudoUsers.push_back(Item(currId, *sentimentPseudoUser, status));
        if(status != SUCCESS)
            return;
    } // End for

    /* Pick model */
    model* lshPseudoUsersModel;

    /* Create model */
    lshPseudoUsersModel = new lshCosine();

    /* Fit model */
    lshPseudoUsersModel->fit(sentimentPseudoUsers, status);
    if(status != SUCCESS)
            return;

    iterSentimentPseudoUsers = sentimentPseudoUsers.begin();

    /* For every user find it's neighbors and predict coins */
    for(i = 0; i < this->usersSize; i++){
        vector<User*> neighborUsers;
        vector<int> newCoins;
        list<int> neighborsIds;
        list<int>::iterator iterNeighborsIds;
        vector<double>* sentimentUser;

        /* Get status of user */
        valid = this->users[i].getStatus(status);
        if(status != SUCCESS)
            return;

        /* Invalid user */
        if(valid == 1){
            /* Recommend random coins */
            this->users[i].recommend(coinsReturned, neighborUsers, newCoins, status);
            if(status != SUCCESS)
                return;

            /* Fix vector of predicted */
            predictedLshPseudoUsers.push_back(newCoins);
            continue;
        }

        sentimentUser = this->users[i].getSentiment(status);
        if(status != SUCCESS)
            return;

        /* Convert user to item */
        Item userItem(*sentimentUser, status);

        /* Find neighbors */
        lshPseudoUsersModel->simpleNeighbors(userItem, neighborsIds, this->p, status);
        if(status != SUCCESS)
            return;

        /* Scan neighborsIds and fix neighbors users */
        for(iterNeighborsIds = neighborsIds.begin(); iterNeighborsIds != neighborsIds.end(); iterNeighborsIds++){
            neighborUsers.push_back(&(this->users[*iterNeighborsIds]));
        } // End for - scan neighborsIds

        /* Recommend coins */
        this->users[i].recommend(coinsReturned, neighborUsers, newCoins, status);
        if(status != SUCCESS)
            return;

        /* Fix vector of predicted */
        predictedLshPseudoUsers.push_back(newCoins);
    } // End for - Predict coins for users

}
void recommendation::recommendationClusteringUsers(int coinsReturned, errorCode& status){}
void recommendation::recommendationClusteringPseudoUsers(int coinsReturned, errorCode& status){}


// PetropoulakisPanagiotis
