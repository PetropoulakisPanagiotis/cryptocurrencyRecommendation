#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../utils/utils.h"
#include "../itemToken/itemToken.h"
#include "../item/item.h"
#include "../user/user.h"
#include "../clusteringProblem/cluster.h"

class recommendation{
    private:
        std::vector<ItemToken> tokenPosts;
        std::vector<Item> vectorPosts;
        std::vector<User> users;
        std::vector<User> pseudoUsers; // Occured from clustering
        int postsSize, usersSize, pseudoUsersSize;
        std::vector<std::unordered_set<std::string> >* allCoins;
        std::vector<std::string>* coins; // Unique names of coins
        std::unordered_map<std::string, double>* lexicon; // Map: token -> score
        int coinsSize;
        cluster* clusterVectorPosts;
        std::vector<std::vector<int> > predictedLshUsers; // Predicted coins for user via lsh - A
        std::vector<std::vector<int> > predictedLshPseudoUsers; // Predicted coins based on pseudo users(lsh) - A
        std::vector<std::vector<int> > predictedClusteringUsers;// - B
        std::vector<std::vector<int> > predictedClusteringPseudoUsers;// - B
        int fittedA, fittedB; // Predicted vectors have been calculated
        int validatedA1, validatedA2, validatedB1, validatedB2; // Validation performed
        double scoreA1, scoreA2, scoreB1, scoreB2; // Validation score
        int p; // Number of neighbors to be examined

        /* Recommendation algorithms */
        void recommendationLshUsers(int coinsReturned, errorCode& status);
        void recommendationLshPseudoUsers(int coinsReturned, errorCode& status);
        void recommendationClusteringUsers(int coinsReturned, errorCode& status);
        void recommendationClusteringPseudoUsers(int coinsReturned, errorCode& status);

        /* Map user with known coin for validation */
        typedef struct userCoin{
            int userPos;
            int coinPos;

            userCoin(int userPos, int coinPos){
                this->userPos = userPos;
                this->coinPos = coinPos;
            }
        }userCoin;

    public:
        recommendation(std::vector<ItemToken>& tokenPosts, std::vector<Item>& vectorPosts, std::vector<User>& users, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, std::unordered_map<std::string, double>& lexicon, errorCode& status, int p=20,int k=120);
        ~recommendation();

        /* Recommend coins for users - lsh or clustering methods */
        void fitLsh(std::vector<std::vector<int> >& predictedLshUsers, std::vector<std::vector<int> >& predictedLshPseudoUsers, errorCode& status, int coinsUsersReturn=5, int coinsPseudoUsersReturn=2);
        void fitClustering(std::vector<std::vector<int> >& predictedClusteringUsers, std::vector<std::vector<int> >& predictedClusteringPseudoUsers, errorCode& status, int coinsUsersReturn=5, int coinsPseudoUsersReturn=2);

        /* Validate methods */
        void validationLshUser(double& score, errorCode& status);
        void validationLshPseudoUser(double& score, errorCode& status);
        void validationClusteringUser(double& score, errorCode& status);
        void validationClusteringPseudoUser(double& score, errorCode& status);
};

// PetropoulakisPanagiotis
