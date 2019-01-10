#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../cryptocurrencyRecommendation/item/item.h"
#include "../cryptocurrencyRecommendation/itemToken/itemToken.h"
#include "../cryptocurrencyRecommendation/user/user.h"
#include "../cryptocurrencyRecommendation/recommendationProblem/recommendation.h"
#include "../cryptocurrencyRecommendation/utils/utils.h"
#include "helpers/experimentHelpers.h"

using namespace std;

int main(int argc, char **argv){
    int argsStatus, validate;
    string usersFile, vectorTweetsFile, coinsFile, lexiconFile, outputFile; // Files
    vector<User> users;
    vector<Item> vectorPosts;
    vector<ItemToken> tokenPosts;
    vector<unordered_set<string> > allCoins;
    vector<string> coins; // Categories of coins
    unordered_map<string, double> lexicon; // Lexicon with tokens and score
    errorCode status = SUCCESS; // Keep errors

    /* Read given arguments and set file names */
    argsStatus = readArguments(argc, argv, usersFile, vectorTweetsFile, coinsFile, lexiconFile, outputFile, validate);
    if(argsStatus == -1){
        cout << "Please give valid arguments. Try again later\n";
        return 0;
    }

    /* Set users, posts, coins and lexicon aka read files and set vectors etc */
    setData(usersFile, users, tokenPosts, vectorTweetsFile, vectorPosts, coinsFile, allCoins, coins, lexiconFile, lexicon, status);
    if(status != SUCCESS){
        printError(status);
        return 0;
    }

    /* Pick recommendation model */
    recommendation* recommendationModel;

    /* Create model */
    recommendationModel = new recommendation(tokenPosts, vectorPosts, users, allCoins, coins, lexicon, status);
    if(status != SUCCESS){
        printError(status);
        delete recommendationModel;
        return 0;
    }
    return 0;
}
// Petropoulakis Panagiotis
