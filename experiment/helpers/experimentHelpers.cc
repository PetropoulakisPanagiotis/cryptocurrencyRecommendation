#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../../cryptocurrencyRecommendation/item/item.h"
#include "../../cryptocurrencyRecommendation/itemToken/itemToken.h"
#include "../../cryptocurrencyRecommendation/user/user.h"
#include "../../cryptocurrencyRecommendation/recommendationProblem/recommendation.h"
#include "../../cryptocurrencyRecommendation/fileHandler/fileHandler.h"
#include "../../cryptocurrencyRecommendation/utils/utils.h"
#include "experimentHelpers.h"

using namespace std;

/* Read arguments from the user */
/* Arguments provided: 0        */
/* Invalid arguments: -1        */
int readArguments(int argc, char **argv, std::string& usersFile, std::string& vectorTweetsFile, std::string& coinsFile, std::string& lexiconFile, std::string& outputFile, int& validate){
    /* Invalid arguments */
    if(argc != 11 && argc != 12)
        return -1;

    if(strcmp(argv[1], "-d") || strcmp(argv[3], "-v") || strcmp(argv[5], "-c") || strcmp(argv[7], "-l") || strcmp(argv[9], "-o"))
        return -1;

    if(argc == 12 && strcmp(argv[11], "-validate"))
        return -1;

    /* Copy arguments */
    usersFile = argv[2];
    vectorTweetsFile = argv[4];
    coinsFile = argv[6];
    lexiconFile = argv[8];
    outputFile = argv[10];

    if(argc == 12)
        validate = 1;
    else
        validate = 0;

    return 0;
}

/* Read files and set users, posts, coins and lexicon */
void setData(string& usersFile, vector<User>& users, vector<ItemToken>& tokenPosts, string& vectorTweetsFile, vector<Item>& vectorPosts, string& coinsFile, vector<unordered_set<string> >& allCoins, vector<string>& coins, string& lexiconFile, unordered_map<string, double>& lexicon, errorCode& status){

    status = SUCCESS;

    /* Reset vectors etc */
    users.clear();
    tokenPosts.clear();
    vectorPosts.clear();
    allCoins.clear();
    coins.clear();
    lexicon.clear();

    ///////////////////////////////////
    /* Read files and set stractures */
    ///////////////////////////////////

    readVectorPostsSet(vectorTweetsFile, 1, ',', vectorPosts, status);
    if(status != SUCCESS)
        return;

    readCoinsSet(coinsFile, '\t', allCoins, coins, status);
    if(status != SUCCESS)
        return;

    readLexiconSet(lexiconFile, '\t', lexicon, status);
    if(status != SUCCESS)
        return;

    readUsersSet(usersFile, '\t', vectorPosts.size(), users, tokenPosts, allCoins, coins, lexicon, status);
    if(status != SUCCESS)
        return;
}

/* Write results in output file */
void writeResults(std::ofstream& outputStream, std::vector<std::vector<int> >& predictedLshUsers, std::vector<std::vector<int> >& predictedLshPseudoUsers, std::vector<std::vector<int> >& predictedClusteringUsers, std::vector<std::vector<int> >& predictedClusteringPseudoUsers, std::vector<std::string>& coins, int validate){

}
// Petropoulakis Panagiotis
