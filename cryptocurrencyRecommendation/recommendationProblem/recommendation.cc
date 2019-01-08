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

    /* Fix pseudo users - cluster == user with it's posts(tokens) */
    /* Vector clustering -> clusters "converts" to pseudo users   */
    /* Each cluster has some vectorPosts -> pseudo user contains  */
    /* these posts but in tokenize form                           */
    for(i = 0; i < this->pseudoUsersSize; i++){

        /* Create current pseudo user */
        this->pseudoUsers.push_back(User((this->usersSize + i), clustersItems[i], *(this->allCoins), *(this->coins), *(this->lexicon), this->tokenPosts, status));
        if(status != SUCCESS){
            delete this->clusterVectorPosts;
            this->clusterVectorPosts = NULL;
            return;
        }
    } // End for - pseudo users

    /* No error occured */
    this->fittedA = 0;
    this->fittedB = 0;
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

}

// PetropoulakisPanagiotis
