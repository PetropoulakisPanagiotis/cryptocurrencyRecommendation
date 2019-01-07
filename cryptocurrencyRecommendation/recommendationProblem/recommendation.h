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
        int p; // Number of neighbors to be examined

    public:
        recommendation(std::vector<ItemToken>& tokenPosts, std::vector<Item>& vectorPosts, std::vector<User>& users, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, std::unordered_map<std::string, double>& lexicon, errorCode& status, int p=20,int k=120);
        ~recommendation();

        /* Recommend coins for users - lsh or clustering methods */
        void fitLsh(std::vector<std::vector<int> >& predictedLshUsers, std::vector<std::vector<int> >& predictedLshPseudoUsers, errorCode& status, int coinsUsersReturn=5, int coinsPseudoUsersReturn=2);
        void fitClustering(std::vector<std::vector<int> >& predictedClusteringUsers, std::vector<std::vector<int> >& predictedClusteringPseudoUsers, errorCode& status, int coinsUsersReturn=5, int coinsPseudoUsersReturn=2);
};

// PetropoulakisPanagiotis
