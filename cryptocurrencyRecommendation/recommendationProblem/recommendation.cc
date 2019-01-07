#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../utils/utils.h"
#include "../itemToken/itemToken.h"
#include "../item/item.h"
#include "../user/user.h"
#include "../clusteringProblem/cluster.h"
#include "recommendation.h"

using namespace std;

/////////////////////////////////////////////
/* Implementations of recommendation class */
/////////////////////////////////////////////

/* Set members(users, posts, coins, lexicon), create clusters based on vectorPosts and set pseudo users  */
recommendation::recommendation(vector<ItemToken>& tokenPosts, vector<Item>& vectorPosts, vector<User>& users, vector<unordered_set<string> >& allCoins, vector<string>& coins, unordered_map<string, double>& lexicon, errorCode& status, int p,int k){

    status = SUCCESS;

    /* Check given parameters */
    checkParamsConstructor(tokenPosts, vectorPosts, users, allCoins, coins, lexicon, p, k, status);
    if(status != SUCCESS){
        this->fittedA = -1;
        this->fittedB = -1;
    }


    /* Init fitted */
    this->fittedA = 0;
    this->fittedB = 0;
}



// PetropoulakisPanagiotis
