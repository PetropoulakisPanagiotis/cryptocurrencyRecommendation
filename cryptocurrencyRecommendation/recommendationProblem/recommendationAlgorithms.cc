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
    if(lshUsersModel == NULL){
        status = ALLOCATION_FAILED;
        return;
    }

    /* Fit model */
    lshUsersModel->fit(sentimentUsers, status);
    if(status != SUCCESS){
        delete lshUsersModel;
        return;
    }

    iterSentimentUsers = sentimentUsers.begin();

    /* For every user find it's neighbors and predict coins */
    for(i = 0; i < this->usersSize; i++){
        vector<User*> neighborUsers;
        vector<int> newCoins;
        list<int> neighborsIds;
        list<int>::iterator iterNeighborsIds;

        /* Get status of user */
        valid = this->users[i].getStatus(status);
        if(status != SUCCESS){
            delete lshUsersModel;
            return;
        }

        /* Invalid user */
        if(valid == 1){
            /* Recommend random coins */
            this->users[i].recommend(coinsReturned, neighborUsers, newCoins, 0, status);
            if(status != SUCCESS){
                delete lshUsersModel;
                return;
            }

            /* Fix vector of predicted */
            predictedLshUsers.push_back(newCoins);

            iterSentimentUsers++;
            continue;
        }
        /* Find neighbors */
        lshUsersModel->simpleNeighbors(*iterSentimentUsers, neighborsIds, this->p, status);
        if(status != SUCCESS){
            delete lshUsersModel;
            return;
        }

        /* Scan neighborsIds and fix neighbors users */
        for(iterNeighborsIds = neighborsIds.begin(); iterNeighborsIds != neighborsIds.end(); iterNeighborsIds++){
            neighborUsers.push_back(&(this->users[*iterNeighborsIds]));
        } // End for - scan neighborsIds

        /* Recommend coins */
        this->users[i].recommend(coinsReturned, neighborUsers, newCoins, 0, status);
        if(status != SUCCESS){
            delete lshUsersModel;
            return;
        }

        /* Fix vector of predicted */
        predictedLshUsers.push_back(newCoins);
        iterSentimentUsers++;
    } // End for - Predict coins for users

    /* Delete model */
    delete lshUsersModel;
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
    if(lshPseudoUsersModel == NULL){
        status = ALLOCATION_FAILED;
        return;
    }

    /* Fit model */
    lshPseudoUsersModel->fit(sentimentPseudoUsers, status);
    if(status != SUCCESS){
        delete lshPseudoUsersModel;
        return;
    }

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
        if(status != SUCCESS){
            delete lshPseudoUsersModel;
            return;
        }

        /* Invalid user */
        if(valid == 1){
            /* Recommend random coins */
            this->users[i].recommend(coinsReturned, neighborUsers, newCoins, 0, status);
            if(status != SUCCESS){
                delete lshPseudoUsersModel;
                return;
            } 

            /* Fix vector of predicted */
            predictedLshPseudoUsers.push_back(newCoins);
            continue;
        }

        sentimentUser = this->users[i].getSentiment(status);
        if(status != SUCCESS){
            delete lshPseudoUsersModel;
            return;
        } 

        /* Convert user to item */
        Item userItem(*sentimentUser, status);

        /* Find neighbors */
        lshPseudoUsersModel->simpleNeighbors(userItem, neighborsIds, this->p, status);
        if(status != SUCCESS){
            delete lshPseudoUsersModel;
            return;
        } 

        /* Scan neighborsIds and fix neighbors users */
        for(iterNeighborsIds = neighborsIds.begin(); iterNeighborsIds != neighborsIds.end(); iterNeighborsIds++){
            neighborUsers.push_back(&(this->users[*iterNeighborsIds]));
        } // End for - scan neighborsIds

        /* Recommend coins */
        this->users[i].recommend(coinsReturned, neighborUsers, newCoins, 0, status);
        if(status != SUCCESS){
            delete lshPseudoUsersModel;
            return;
        } 

        /* Fix vector of predicted */
        predictedLshPseudoUsers.push_back(newCoins);
    } // End for - Predict coins for users

    /* Delete model */
    delete lshPseudoUsersModel;
}

/* Find neighbor users with clustering and predict coins for users */
void recommendation::recommendationClusteringUsers(int coinsReturned, errorCode& status){
    vector<Item> sentimentUsers;
    string currId;
    int i, valid, j;
    unordered_map<int, int> mapId; // Map real id with id in clusters
    int numInsertion = 0;

    status = SUCCESS;

    /* Make clusters with the sentiment of the users */

    cluster* sentimentUsersClusters;

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

        /* Fix map */
        mapId.insert(make_pair(i, numInsertion));

        /* Fix sentiment list */
        sentimentUsers.push_back(Item(currId, *sentimentUser, status));
        if(status != SUCCESS)
            return;
        numInsertion++;
    } // End for

    /* Create clusters */
    sentimentUsersClusters = new cluster(status, sentimentUsers, this->coinsSize);
    if(sentimentUsersClusters == NULL){
        status = ALLOCATION_FAILED;
        return;
    }

    if(status != SUCCESS){
        delete sentimentUsersClusters;
        return;
    }

    /* Fit clusters */
    sentimentUsersClusters->fit(status);
    if(status != SUCCESS){
        delete sentimentUsersClusters;
        return;
    }

    /* FOr every user predict coins */
    for(i = 0; i < this->usersSize; i++){
        vector<User*> neighborUsers;
        vector<int> newCoins;
        vector<int> neighborsIds;

        /* Get status of user */
        valid = this->users[i].getStatus(status);
        if(status != SUCCESS){
            delete sentimentUsersClusters;
            return;
        }

        /* Invalid user */
        if(valid == 1){
            /* Recommend random coins */
            this->users[i].recommend(coinsReturned, neighborUsers, newCoins, 1, status);
            if(status != SUCCESS){
                delete sentimentUsersClusters;
                return;
            }

            /* Fix vector of predicted */
            predictedClusteringUsers.push_back(newCoins);
            continue;
        }

        /*  Find neighbors of user */
        sentimentUsersClusters->getNeighborsItem(mapId[i], neighborsIds, status);
        if(status != SUCCESS){
            delete sentimentUsersClusters;
            return;
        }

        /* Scan neighborsIds and fix neighbors users */
        for(j = 0; j < (int)neighborsIds.size(); j++){
            neighborUsers.push_back(&(this->users[neighborsIds[j]]));
        } // End for - scan neighborsIds

        /* Recommend coins */
        this->users[i].recommend(coinsReturned, neighborUsers, newCoins, 1, status);
        if(status != SUCCESS){
            delete sentimentUsersClusters;
            return;
        }

        /* Fix vector of predicted */
        predictedClusteringUsers.push_back(newCoins);
    } // End for

    /* Delete model */
    delete sentimentUsersClusters;
}

/* Find neighbor users with clustering and predict coins for users */
void recommendation::recommendationClusteringPseudoUsers(int coinsReturned, errorCode& status){
    vector<Item> sentimentPseudoUsers;
    string currId;
    int i, valid, j;
    unordered_map<int, int> mapId; // Map real id with id in clusters
    int numInsertion = 0;

    status = SUCCESS;

    /* Make clusters with the sentiment of the pseudo users */

    cluster* sentimentPseudoUsersClusters;

    /* Get sentiment of pseudo users */
    for(i = 0; i < this->pseudoUsersSize; i++){

        /* Discard invalid users */
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

        /* Fix map */
        mapId.insert(make_pair(numInsertion, i));

        /* Fix sentiment list */
        sentimentPseudoUsers.push_back(Item(currId, *sentimentPseudoUser, status));
        if(status != SUCCESS)
            return;
        numInsertion++;
    } // End for

    /* Create clusters */
    sentimentPseudoUsersClusters = new cluster(status, sentimentPseudoUsers, this->coinsSize);
    if(sentimentPseudoUsersClusters == NULL){
        status = ALLOCATION_FAILED;
        return;
    }

    if(status != SUCCESS){
        delete sentimentPseudoUsersClusters;
        return;
    }

    /* Fit clusters */
    sentimentPseudoUsersClusters->fit(status);
    if(status != SUCCESS){
        delete sentimentPseudoUsersClusters;
        return;
    }

    int minClusterPos; // Cluster of user
    vector<vector<int> > clustersItems; // Get indexes of items in clusters

    sentimentPseudoUsersClusters->getClustersItems(clustersItems, status);
    if(status != SUCCESS){
        delete sentimentPseudoUsersClusters;
        return;
    }

    /* For every user predict coins */
    for(i = 0; i < this->usersSize; i++){
        vector<User*> neighborUsers;
        vector<int> newCoins;
        vector<int> neighborsIds;

        /* Get status of user */
        valid = this->users[i].getStatus(status);
        if(status != SUCCESS){
            delete sentimentPseudoUsersClusters;
            return;
        }

        /* Invalid user */
        if(valid == 1){
            /* Recommend random coins */
            this->users[i].recommend(coinsReturned, neighborUsers, newCoins, 1, status);
            if(status != SUCCESS){
                delete sentimentPseudoUsersClusters;
                return;
            }

            /* Fix vector of predicted */
            predictedClusteringPseudoUsers.push_back(newCoins);
            continue;
        }

        /* Fix Item for user */
        vector<double>* sentimentUser;

        sentimentUser = this->users[i].getSentiment(status);
        if(status != SUCCESS){
            delete sentimentPseudoUsersClusters;
            return;
        }

        currId = to_string(i);
        Item query(*sentimentUser, status);

        /* Find min cluster */
        sentimentPseudoUsersClusters->getNearestCluster(query, minClusterPos, status);
        if(status != SUCCESS){
            delete sentimentPseudoUsersClusters;
            return;
        }

        int neighborPos;

        /*  Find neighbors of user */
        for(j = 0; j < (int)clustersItems[minClusterPos].size(); j++){
            neighborPos = clustersItems[minClusterPos][j];

            neighborUsers.push_back(&(this->pseudoUsers[mapId[neighborPos]]));
        } // End for - fix neighbors

        /* Recommend coins */
        this->users[i].recommend(coinsReturned, neighborUsers, newCoins, 1, status);
        if(status != SUCCESS){
            delete sentimentPseudoUsersClusters;
            return;
        }

        /* Fix vector of predicted */
        predictedClusteringUsers.push_back(newCoins);
    } // End for

    /* Delete model */
    delete sentimentPseudoUsersClusters;
}


// PetropoulakisPanagiotis
