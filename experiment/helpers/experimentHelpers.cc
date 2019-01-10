#include <iostream>
#include <string.h>
#include <list>
#include <vector>
#include <chrono>
#include <fstream>
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

void setData(std::string& usersFile, std::vector<User>& users, std::vector<ItemToken>& tokenPosts, std::string& vectorTweetsFile, std::vector<Item>& vectorPosts, std::string& coinsFile, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, std::string& lexiconFile, std::unordered_map<std::string, double>& lexicon, errorCode& status){}
// Petropoulakis Panagiotis
