#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <unistd.h>
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

/////////////////////////////////////////////
/* Implementations of recommendation class */
/////////////////////////////////////////////

/* Set members(users, posts, coins, lexicon), create clusters based on vectorPosts and set pseudo users  */
recommendation::recommendation(vector<ItemToken>& tokenPosts, vector<Item>& vectorPosts, vector<User>& users, vector<unordered_set<string> >& allCoins, vector<string>& coins, unordered_map<string, double>& lexicon, errorCode& status, int p,int k){
    int i;

    status = SUCCESS;

    this->fittedA = -1;
    this->fittedB = -1;

    /* Check given parameters */
    checkParamsConstructor(tokenPosts, vectorPosts, users, allCoins, coins, lexicon, p, k, status);
    if(status != SUCCESS)
        return;

    /* Set size of vectors */
    this->postsSize = tokenPosts.size();
    this->usersSize = users.size();
    this->coinsSize = coins.size();
    this->pseudoUsersSize = k;

    /* Set simple members */
    this->p = p;

    /* Set vectors */
    for(i = 0; i < this->postsSize; i++){
        this->tokenPosts.push_back(tokenPosts[i]);
        this->vectorPosts.push_back(vectorPosts[i]);
    } // End for - posts

    for(i = 0; i < this->usersSize; i++){
        this->users.push_back(users[i]);
    } // End for - users

    /* Set sets */
    this->coins = &coins;
    this->allCoins = &allCoins;
    this->lexicon = &lexicon;

    /* Create clusters */
    this->clusterVectorPosts = new cluster(status, this->vectorPosts, k);
    if(this->clusterVectorPosts == NULL){
        status = ALLOCATION_FAILED;
        return;
    }

    if(status != SUCCESS){
        delete this->clusterVectorPosts;
        this->clusterVectorPosts = NULL;
        return;
    }

    /* Fit clusters */
    this->clusterVectorPosts->fit(status);
    if(status != SUCCESS){
        delete this->clusterVectorPosts;
        this->clusterVectorPosts = NULL;
        return;
    }

    /* Get cluster items  */
    vector<vector<int> > clustersItems;
    this->clusterVectorPosts->getClustersItems(clustersItems, status);
    if(status != SUCCESS){
        delete this->clusterVectorPosts;
        this->clusterVectorPosts = NULL;
        return;
    }


    int activeClusters = 0;

    /* Fix pseudo users - cluster == user with it's posts(tokens) */
    /* Vector clustering -> clusters "converts" to pseudo users   */
    /* Each cluster has some vectorPosts -> pseudo user contains  */
    /* these posts but in tokenize form                           */
    for(i = 0; i < this->pseudoUsersSize; i++){

        if((int)clustersItems[i].size() == 0)
            continue;

        /* Create current pseudo user */
        this->pseudoUsers.push_back(User(i, clustersItems[i], *(this->allCoins), *(this->coins), *(this->lexicon), this->tokenPosts, status));
        if(status != SUCCESS){
            delete this->clusterVectorPosts;
            this->clusterVectorPosts = NULL;
            return;
        }

        activeClusters++;
    } // End for - pseudo users

    this->pseudoUsersSize = activeClusters;

    /* No error occured */
    this->fittedA = 0;
    this->fittedB = 0;
    this->validatedA1 = 0;
    this->validatedA2 = 0;
    this->validatedB1 = 0;
    this->validatedB2 = 0;
}

recommendation::~recommendation(){

    if(this->fittedA == -1 || this->fittedB == -1){
        return;
    }

    if(this->clusterVectorPosts != NULL)
        delete this->clusterVectorPosts;
}

/* Recommend coins for users using lsh method */
/* Recommend coins based on neighbors users   */
/* and based on pseudo neighbors users        */
void recommendation::fitLsh(vector<vector<int> >& predictedLshUsers, vector<vector<int> >& predictedLshPseudoUsers, errorCode& status, int coinsUsersReturn, int coinsPseudoUsersReturn){

    status = SUCCESS;

    if(this->fittedA == -1 || this->fittedB == -1){
        status = INVALID_METHOD;
        return;
    }

    /* Already fitted return calculated predicted coins      */
    /* Note use another method if you whant to use different */
    /* default values(coinsUsersReturn etc.)                 */
    if(this->fittedA == 1){
        predictedLshUsers = this->predictedLshUsers;
        predictedLshPseudoUsers = this->predictedLshPseudoUsers;
        return;
    }

    ////////////////////////////////////////////////////
    /* Find neighbor users with lsh and predict coins */
    ////////////////////////////////////////////////////
    this->recommendationLshUsers(coinsUsersReturn, status);
    if(status != SUCCESS)
        return;

    ///////////////////////////////////////////////////////////
    /* Find neighbor pseudo users with lsh and predict coins */
    ///////////////////////////////////////////////////////////
    this->recommendationLshPseudoUsers(coinsPseudoUsersReturn, status);
    if(status != SUCCESS)
        return;

    /* Return results */
    predictedLshUsers = this->predictedLshUsers;
    predictedLshPseudoUsers = this->predictedLshPseudoUsers;

    /* Fitted method */
    this->fittedA = 1;
}

/* Recommend coins for users using clustering method */
/* Recommend coins based on neighbors users          */
/* and based on pseudo neighbors users               */
void recommendation::fitClustering(vector<vector<int> >& predictedClusteringUsers, vector<vector<int> >& predictedClusteringPseudoUsers, errorCode& status, int coinsUsersReturn, int coinsPseudoUsersReturn){

    status = SUCCESS;

    if(this->fittedA == -1 || this->fittedB == -1){
        status = INVALID_METHOD;
        return;
    }

    /* Already fitted return calculated predicted coins      */
    /* Note use another method if you whant to use different */
    /* default values(coinsUsersReturn etc.)                 */
    if(this->fittedB == 1){
        predictedClusteringUsers = this->predictedClusteringUsers;
        predictedClusteringPseudoUsers = this->predictedClusteringPseudoUsers;
        return;
    }

    ////////////////////////////////////////////////////
    /* Find neighbor users with lsh and predict coins */
    ////////////////////////////////////////////////////
    this->recommendationClusteringUsers(coinsUsersReturn, status);
    if(status != SUCCESS)
        return;

    ///////////////////////////////////////////////////////////
    /* Find neighbor pseudo users with lsh and predict coins */
    ///////////////////////////////////////////////////////////
    this->recommendationClusteringPseudoUsers(coinsPseudoUsersReturn, status);
    if(status != SUCCESS)
        return;

    /* Return results */
    predictedClusteringUsers = this->predictedClusteringUsers;
    predictedClusteringPseudoUsers = this->predictedClusteringPseudoUsers;

    /* Fitted method */
    this->fittedB = 1;
}

/* Validate lsh user method */
void recommendation::validationLshUser(double& score, errorCode& status){
    int i, j;
    vector<vector<int> > knownCoinsUsers;
    vector<vector<userCoin> > buckets; // map: user - coin
    int sizeBuckets = 10;
    int round = 0, posBucket;
    double currScore;
    int totalPairs = 0;

    status = SUCCESS;

    if(this->fittedA == -1 || this->fittedB == -1){
        status = INVALID_METHOD;
        return;
    }

    if(this->validatedA1 == 1){
        score = this->scoreA1;
        return;
    }

    /* Set known coins */
    for(i = 0; i < this->usersSize; i++){
        knownCoinsUsers.push_back(vector<int>());

        vector<int> *unknownCoins = this->users[i].getUnknownCoins(status);
        if(status != SUCCESS)
            return;

        vector<int>& ref = *unknownCoins;
        for(j = 0; j < (int)ref.size(); j++){
            if(ref[j] == 1){
                knownCoinsUsers[i].push_back(j);
            }
        }
    }

    /* Set buckets */
    for(i = 0; i < sizeBuckets; i++)
        buckets.push_back(vector<userCoin>());

    /* Split (users,coins) in buckets */
    for(i = 0; i < (int)knownCoinsUsers.size(); i++){
        for(j = 0; j < (int)knownCoinsUsers[i].size(); j++){
            buckets[round].push_back(userCoin(i,j));
            round++;
            if(round == 10)
                round = 0;
        }
    }

    /* Reset score */
    this->scoreA1 = 0;
    /* Perform 10-fold validation */
    for(posBucket = 0; posBucket < sizeBuckets; posBucket++){
        vector<User> tmpUsers;

        /* Copy users */
        for(i = 0; i < this->usersSize; i++){
            tmpUsers.push_back(this->users[i]);
        } // End for - users

        /* Remove coins from current bucket */
        int posUser;

        for(posUser = 0; posUser < (int)buckets[posBucket].size(); posUser++){
            tmpUsers[buckets[posBucket][posUser].userPos].removeCoin(buckets[posBucket][posUser].coinPos, status);
            if(status != SUCCESS)
                return;
        } // End for - remove coins

        //////////////////////////////
        /* Perform lsh users method */
        //////////////////////////////
        //
        /* Every user is represented with it's sentiment */
        list<Item> sentimentUsers;
        vector<Item> sentimentVectorUsers;
        list<Item>::iterator iterSentimentUsers;
        string currId;
        int valid;

        /* Get sentiment of users */
        for(i = 0; i < this->usersSize; i++){

            /* Discard invalid users */
            valid = tmpUsers[i].getStatus(status);
            if(status != SUCCESS)
                return;

            /* Invalid user */
            if(valid == 1){
                sentimentVectorUsers.push_back(Item());
                continue;
            }

            /* Get sentiment */
            vector<double>* sentimentUser;

            sentimentUser = tmpUsers[i].getSentiment(status);
            if(status != SUCCESS)
                return;

            currId = to_string(i);

            /* Fix sentiment list */
            sentimentUsers.push_back(Item(currId, *sentimentUser, status));
            if(status != SUCCESS)
                return;

            sentimentVectorUsers.push_back(Item(currId, *sentimentUser, status));
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

        int posUserCoin;

        /* For every test user find it's neighbors and predict Unknown coin */
        for(posUserCoin = 0; posUserCoin < (int)buckets[posBucket].size(); posUserCoin++){
            vector<User*> neighborUsers;
            vector<int> newCoins;
            list<int> neighborsIds;
            list<int>::iterator iterNeighborsIds;
            vector<double>* sentimentUser;
            totalPairs++;

            /* Get status of user */
            valid = tmpUsers[buckets[posBucket][posUserCoin].userPos].getStatus(status);
            if(status != SUCCESS){
                delete lshUsersModel;
                return;
            }

            /* Invalid user */
            if(valid == 1){
                /* Recommend random coins */
                tmpUsers[buckets[posBucket][posUserCoin].userPos].recommend(buckets[posBucket][posUserCoin].coinPos, neighborUsers, currScore, 0, status);
                if(status != SUCCESS){
                    delete lshUsersModel;
                    return;
                }

                /* Fix curr score */
                currScore = this->users[buckets[posBucket][posUserCoin].userPos].getSentimentIndex(buckets[posBucket][posUserCoin].coinPos, status) - currScore;
                if(status != SUCCESS){
                    delete lshUsersModel;
                    return;
                }
                this->scoreA1 += currScore;

                continue;
            }

            sentimentUser = tmpUsers[buckets[posBucket][posUserCoin].userPos].getSentiment(status);
            if(status != SUCCESS){
                delete lshUsersModel;
                return;
            }

            /* Convert user to item */
            Item userItem(*sentimentUser, status);

            /* Find neighbors */
            lshUsersModel->simpleNeighbors(userItem, neighborsIds, this->p, status);
            if(status != SUCCESS){
                delete lshUsersModel;
                return;
            }

            /* Scan neighborsIds and fix neighbors users */
            for(iterNeighborsIds = neighborsIds.begin(); iterNeighborsIds != neighborsIds.end(); iterNeighborsIds++){
                neighborUsers.push_back(&(this->users[*iterNeighborsIds]));
            } // End for - scan neighborsIds

            /* Recommend coin */
            tmpUsers[buckets[posBucket][posUserCoin].userPos].recommend(buckets[posBucket][posUserCoin].coinPos, neighborUsers, currScore, 0, status);
            if(status != SUCCESS){
                delete lshUsersModel;
                return;
            }

            /* Fix curr score */
            currScore = this->users[buckets[posBucket][posUserCoin].userPos].getSentimentIndex(buckets[posBucket][posUserCoin].coinPos, status) - currScore;
            if(status != SUCCESS){
                delete lshUsersModel;
                return;
            }

            this->scoreA1 += currScore;

        } // End for - Predict coins for users

        /* Delete model */
        delete lshUsersModel;
    } // End for - map

    if(totalPairs != 0)
        this->scoreA1 = this->scoreA1 / totalPairs;
    else
        status = DIV_OVERFLOW;

    score = this->scoreA1;
    this->validatedA1 = 1;
}

/* Validate lsh pseudo user method */
void recommendation::validationLshPseudoUser(double& score, errorCode& status){
    int i, j;
    vector<vector<int> > knownCoinsUsers;
    vector<vector<userCoin> > buckets; // map: user - coin
    int sizeBuckets = 10;
    int round = 0, posBucket;
    double currScore;
    int totalPairs = 0;

    status = SUCCESS;

    if(this->fittedA == -1 || this->fittedB == -1){
        status = INVALID_METHOD;
        return;
    }

    if(this->validatedA2 == 1){
        score = this->scoreA2;
        return;
    }

    /* Set known coins */
    for(i = 0; i < this->usersSize; i++){
        knownCoinsUsers.push_back(vector<int>());

        vector<int> *unknownCoins = this->users[i].getUnknownCoins(status);
        if(status != SUCCESS)
            return;

        vector<int>& ref = *unknownCoins;
        for(j = 0; j < (int)ref.size(); j++){
            if(ref[j] == 1){
                knownCoinsUsers[i].push_back(j);
            }
        }
    }

    /* Set buckets */
    for(i = 0; i < sizeBuckets; i++)
        buckets.push_back(vector<userCoin>());

    /* Split (users,coins) in buckets */
    for(i = 0; i < (int)knownCoinsUsers.size(); i++){
        for(j = 0; j < (int)knownCoinsUsers[i].size(); j++){
            buckets[round].push_back(userCoin(i,j));
            round++;
            if(round == 10)
                round = 0;
        }
    }

    /* Reset score */
    this->scoreA2 = 0;
    /* Perform 10-fold validation */
    for(posBucket = 0; posBucket < sizeBuckets; posBucket++){
        vector<User> tmpUsers;

        /* Copy users */
        for(i = 0; i < this->usersSize; i++){
            tmpUsers.push_back(this->users[i]);
        } // End for - users

        /* Remove coins from current bucket */
        int posUser;

        for(posUser = 0; posUser < (int)buckets[posBucket].size(); posUser++){
            tmpUsers[buckets[posBucket][posUser].userPos].removeCoin(buckets[posBucket][posUser].coinPos, status);
            if(status != SUCCESS)
                return;
        } // End for - remove coins

        //////////////////////////////
        /* Perform lsh users method */
        //////////////////////////////
        //
        /* Every user is represented with it's sentiment */
        list<Item> sentimentPseudoUsers;
        list<Item>::iterator iterSentimentUsers;
        string currId;
        int valid;

        /* Get sentiment of users */
        for(i = 0; i < this->pseudoUsersSize; i++){

            /* Discard invalid users */
            valid = this->pseudoUsers[i].getStatus(status);
            if(status != SUCCESS)
                return;

            /* Invalid user */
            if(valid == 1){
                continue;
            }

            /* Get sentiment */
            vector<double>* sentimentPseudoUser;

            sentimentPseudoUser = tmpUsers[i].getSentiment(status);
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
        int posUserCoin;
        /* For every test user find it's neighbors and predict Unknown coin */
        for(posUserCoin = 0; posUserCoin < (int)buckets[posBucket].size(); posUserCoin++){
            vector<User*> neighborUsers;
            vector<int> newCoins;
            list<int> neighborsIds;
            list<int>::iterator iterNeighborsIds;
            vector<double>* sentimentUser;
            totalPairs++;

            /* Get status of user */
            valid = tmpUsers[buckets[posBucket][posUserCoin].userPos].getStatus(status);
            if(status != SUCCESS){
                delete lshPseudoUsersModel;
                return;
            }

            /* Invalid user */
            if(valid == 1){
                /* Recommend random coins */
                tmpUsers[buckets[posBucket][posUserCoin].userPos].recommend(buckets[posBucket][posUserCoin].coinPos, neighborUsers, currScore, 0, status);
                if(status != SUCCESS){
                    delete lshPseudoUsersModel;
                    return;
                }

                /* Fix curr score */
                currScore = this->users[buckets[posBucket][posUserCoin].userPos].getSentimentIndex(buckets[posBucket][posUserCoin].coinPos, status) - currScore;
                if(status != SUCCESS){
                    delete lshPseudoUsersModel;
                    return;
                }
                this->scoreA2 += currScore;

                continue;
            }

            sentimentUser = tmpUsers[buckets[posBucket][posUserCoin].userPos].getSentiment(status);
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
            tmpUsers[buckets[posBucket][posUserCoin].userPos].recommend(buckets[posBucket][posUserCoin].coinPos, neighborUsers, currScore, 0, status);
            if(status != SUCCESS){
                delete lshPseudoUsersModel;
                return;
            }

            /* Fix curr score */
            currScore = this->users[buckets[posBucket][posUserCoin].userPos].getSentimentIndex(buckets[posBucket][posUserCoin].coinPos, status) - currScore;
            if(status != SUCCESS){
                delete lshPseudoUsersModel;
                return;
            }

            this->scoreA2 += currScore;

        } // End for - Predict coins for users

        /* Delete model */
        delete lshPseudoUsersModel;
    } // End for - map

    if(totalPairs != 0)
        this->scoreA2 = this->scoreA2 / totalPairs;
    else
        status = DIV_OVERFLOW;
    score = this->scoreA2;
    this->validatedA2 = 1;
}

/* Validate clustering user method */
void recommendation::validationClusteringUser(double& score, errorCode& status){
    int i, j;
    vector<vector<int> > knownCoinsUsers;
    vector<vector<userCoin> > buckets; // map: user - coin
    int sizeBuckets = 10;
    int round = 0, posBucket;
    double currScore;
    int totalPairs = 0;

    status = SUCCESS;

    if(this->fittedA == -1 || this->fittedB == -1){
        status = INVALID_METHOD;
        return;
    }

    if(this->validatedB1 == 1){
        score = this->scoreB1;
        return;
    }

    /* Set known coins */
    for(i = 0; i < this->usersSize; i++){
        knownCoinsUsers.push_back(vector<int>());

        vector<int> *unknownCoins = this->users[i].getUnknownCoins(status);
        if(status != SUCCESS)
            return;

        vector<int>& ref = *unknownCoins;
        for(j = 0; j < (int)ref.size(); j++){
            if(ref[j] == 1){
                knownCoinsUsers[i].push_back(j);
            }
        }
    }

    /* Set buckets */
    for(i = 0; i < sizeBuckets; i++)
        buckets.push_back(vector<userCoin>());

    /* Split (users,coins) in buckets */
    for(i = 0; i < (int)knownCoinsUsers.size(); i++){
        for(j = 0; j < (int)knownCoinsUsers[i].size(); j++){
            buckets[round].push_back(userCoin(i,j));
            round++;
            if(round == 10)
                round = 0;
        }
    }

    /* Reset score */
    this->scoreB1 = 0;
    /* Perform 10-fold validation */
    for(posBucket = 0; posBucket < sizeBuckets; posBucket++){
        vector<User> tmpUsers;

        /* Copy users */
        for(i = 0; i < this->usersSize; i++){
            tmpUsers.push_back(this->users[i]);
        } // End for - users

        /* Remove coins from current bucket */
        int posUser;

        for(posUser = 0; posUser < (int)buckets[posBucket].size(); posUser++){
            tmpUsers[buckets[posBucket][posUser].userPos].removeCoin(buckets[posBucket][posUser].coinPos, status);
            if(status != SUCCESS)
                return;
        } // End for - remove coins

        //////////////////////////////
        /* Perform lsh users method */
        //////////////////////////////
        //
        /* Every user is represented with it's sentiment */
        vector<Item> sentimentUsers;
        string currId;
        int valid;
        unordered_map<int, int> mapId; // Map real id with id in clusters
        int numInsertion = 0;


        /* Make clusters with the sentiment of the users */
        cluster* sentimentUsersClusters;

        /* Get sentiment of users */
        for(i = 0; i < this->usersSize; i++){

            /* Discard invalid users */
            valid = tmpUsers[i].getStatus(status);
            if(status != SUCCESS)
                return;

            /* Invalid user */
            if(valid == 1){
                continue;
            }

            /* Get sentiment */
            vector<double>* sentimentUser;

            sentimentUser = tmpUsers[i].getSentiment(status);
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

        int posUserCoin;
        /* For every test user find it's neighbors and predict Unknown coin */
        for(posUserCoin = 0; posUserCoin < (int)buckets[posBucket].size(); posUserCoin++){
            vector<User*> neighborUsers;
            vector<int> newCoins;
            vector<int> neighborsIds;
            totalPairs++;

            /* Get status of user */
            valid = tmpUsers[buckets[posBucket][posUserCoin].userPos].getStatus(status);
            if(status != SUCCESS){
                delete sentimentUsersClusters;
                return;
            }

            /* Invalid user */
            if(valid == 1){
                /* Recommend random coins */
                tmpUsers[buckets[posBucket][posUserCoin].userPos].recommend(buckets[posBucket][posUserCoin].coinPos, neighborUsers, currScore, 1, status);
                if(status != SUCCESS){
                    delete sentimentUsersClusters;
                    return;
                }

                /* Fix curr score */
                currScore = this->users[buckets[posBucket][posUserCoin].userPos].getSentimentIndex(buckets[posBucket][posUserCoin].coinPos, status) - currScore;
                if(status != SUCCESS){
                    delete sentimentUsersClusters;
                    return;
                }
                this->scoreB1 += currScore;

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
            tmpUsers[buckets[posBucket][posUserCoin].userPos].recommend(buckets[posBucket][posUserCoin].coinPos, neighborUsers, currScore, 1, status);
            if(status != SUCCESS){
                delete sentimentUsersClusters;
                return;
            }

            /* Fix curr score */
            currScore = this->users[buckets[posBucket][posUserCoin].userPos].getSentimentIndex(buckets[posBucket][posUserCoin].coinPos, status) - currScore;
            if(status != SUCCESS){
                delete sentimentUsersClusters;
                return;
            }

            this->scoreB1 += currScore;

        } // End for - Predict coins for users

        /* Delete model */
        delete sentimentUsersClusters;
    } // End for - map

    if(totalPairs != 0)
        this->scoreB1 = this->scoreB1 / totalPairs;
    else
        status = DIV_OVERFLOW;

    score = this->scoreB1;
    this->validatedB1 = 1;
}

/* Validate clustering user method */
void recommendation::validationClusteringPseudoUser(double& score, errorCode& status){
    int i, j;
    vector<vector<int> > knownCoinsUsers;
    vector<vector<userCoin> > buckets; // map: user - coin
    int sizeBuckets = 10;
    int round = 0, posBucket;
    double currScore;
    int totalPairs = 0;

    status = SUCCESS;

    if(this->fittedA == -1 || this->fittedB == -1){
        status = INVALID_METHOD;
        return;
    }

    if(this->validatedB2 == 1){
        score = this->scoreB2;
        return;
    }

    /* Set known coins */
    for(i = 0; i < this->usersSize; i++){
        knownCoinsUsers.push_back(vector<int>());

        vector<int> *unknownCoins = this->users[i].getUnknownCoins(status);
        if(status != SUCCESS)
            return;

        vector<int>& ref = *unknownCoins;
        for(j = 0; j < (int)ref.size(); j++){
            if(ref[j] == 1){
                knownCoinsUsers[i].push_back(j);
            }
        }
    }

    /* Set buckets */
    for(i = 0; i < sizeBuckets; i++)
        buckets.push_back(vector<userCoin>());

    /* Split (users,coins) in buckets */
    for(i = 0; i < (int)knownCoinsUsers.size(); i++){
        for(j = 0; j < (int)knownCoinsUsers[i].size(); j++){
            buckets[round].push_back(userCoin(i,j));
            round++;
            if(round == 10)
                round = 0;
        }
    }

    /* Reset score */
    this->scoreB2 = 0;
    /* Perform 10-fold validation */
    for(posBucket = 0; posBucket < sizeBuckets; posBucket++){
        vector<User> tmpUsers;

        /* Copy users */
        for(i = 0; i < this->usersSize; i++){
            tmpUsers.push_back(this->users[i]);
        } // End for - users

        /* Remove coins from current bucket */
        int posUser;

        for(posUser = 0; posUser < (int)buckets[posBucket].size(); posUser++){
            tmpUsers[buckets[posBucket][posUser].userPos].removeCoin(buckets[posBucket][posUser].coinPos, status);
            if(status != SUCCESS)
                return;
        } // End for - remove coins

        //////////////////////////////
        /* Perform lsh users method */
        //////////////////////////////
        //
        /* Every user is represented with it's sentiment */
        vector<Item> sentimentPseudoUsers;
        string currId;
        int valid;
        unordered_map<int, int> mapId; // Map real id with id in clusters
        int numInsertion = 0;


        /* Make clusters with the sentiment of the users */
        cluster* sentimentPseudoUsersClusters;

        /* Get sentiment of users */
        for(i = 0; i < this->pseudoUsersSize; i++){

            /* Discard invalid users */
            valid = this->pseudoUsers[i].getStatus(status);
            if(status != SUCCESS)
                return;

            /* Invalid user */
            if(valid == 1){
                continue;
            }

            /* Get sentiment */
            vector<double>* sentimentPseudoUser;

            sentimentPseudoUser = this->pseudoUsers[i].getSentiment(status);
            if(status != SUCCESS)
                return;

            currId = to_string(i);

            /* Fix map */
            mapId.insert(make_pair(i, numInsertion));

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

        int posUserCoin;
        /* For every test user find it's neighbors and predict Unknown coin */
        for(posUserCoin = 0; posUserCoin < (int)buckets[posBucket].size(); posUserCoin++){
            vector<User*> neighborUsers;
            vector<int> newCoins;
            vector<int> neighborsIds;
            totalPairs++;

            /* Get status of user */
            valid = tmpUsers[buckets[posBucket][posUserCoin].userPos].getStatus(status);
            if(status != SUCCESS){
                delete sentimentPseudoUsersClusters;
                return;
            }

            /* Invalid user */
            if(valid == 1){
                /* Recommend random coins */
                tmpUsers[buckets[posBucket][posUserCoin].userPos].recommend(buckets[posBucket][posUserCoin].coinPos, neighborUsers, currScore, 1, status);
                if(status != SUCCESS){
                    delete sentimentPseudoUsersClusters;
                    return;
                }

                /* Fix curr score */
                currScore = this->users[buckets[posBucket][posUserCoin].userPos].getSentimentIndex(buckets[posBucket][posUserCoin].coinPos, status) - currScore;
                if(status != SUCCESS){
                    delete sentimentPseudoUsersClusters;
                    return;
                }
                this->scoreB2 += currScore;

                continue;
            }

            /* Fix Item for user */
            vector<double>* sentimentUser;
            sentimentUser = tmpUsers[buckets[posBucket][posUserCoin].userPos].getSentiment(status);
            if(status != SUCCESS){
                delete sentimentPseudoUsersClusters;
                return;
            }

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

            /* Recommend coin */
            tmpUsers[buckets[posBucket][posUserCoin].userPos].recommend(buckets[posBucket][posUserCoin].coinPos, neighborUsers, currScore, 1, status);
            if(status != SUCCESS){
                delete sentimentPseudoUsersClusters;
                return;
            }

            /* Fix curr score */
            currScore = this->users[buckets[posBucket][posUserCoin].userPos].getSentimentIndex(buckets[posBucket][posUserCoin].coinPos, status) - currScore;
            if(status != SUCCESS){
                delete sentimentPseudoUsersClusters;
                return;
            }

            this->scoreB2 += currScore;

        } // End for - Predict coins for users

        /* Delete model */
        delete sentimentPseudoUsersClusters;
    } // End for - map

    if(totalPairs != 0)
        this->scoreB2 = this->scoreB2 / totalPairs;
    else
        status = DIV_OVERFLOW;

    score = this->scoreB2;
    this->validatedB2 = 1;
}




// PetropoulakisPanagiotis
