#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../itemToken/itemToken.h"
#include "../utils/utils.h"

/* User class: each user has some texts and an overall sentiment */
/* Note: user have access to sets(coins,lexicon,tweets)          */
/* Make sure that sets are valid                                 */
class User{
    private:
        int id;
        std::vector<int> idPosts; // Posts of the user
        std::vector<double> sentiment; // Overall sentiment
        std::vector<int> unknownCoins; // 0 -> Unknown
        std::vector<std::unordered_set<std::string> >* allCoins;
        std::vector<std::string>* coins;
        std::unordered_map<std::string, double>* lexicon;
        std::vector<ItemToken>* allPosts; // All posts
        int totalPosts;

    public:
        User(int id, std::vector<int>& idPosts, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, std::unordered_map<std::string, double>& lexicon, std::vector<ItemToken>& allPosts, errorCode& status);

        /* Recommend the best(p) coins in current user based on given neighbors */
        void recommend(int p, std::vector<User>& neighborUsers, std::vector<int>& newCoins, errorCode& status);

        /* Accessors */
        int getId(errorCode& status);
        std::vector<int>* getIdPosts(errorCode& status);
        std::vector<double>* getSentiment(errorCode& status);
        std::vector<int>* getUnknownCoins(errorCode& status);
        int getUnknownCoin(int index, errorCode& status);
        int getSizeOfPosts(errorCode& status);
};
// PetropoulakisPanagiotis
