#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../../cryptocurrencyRecommendation/item/item.h"
#include "../../cryptocurrencyRecommendation/itemToken/itemToken.h"
#include "../../cryptocurrencyRecommendation/user/user.h"
#include "../../cryptocurrencyRecommendation/recommendationProblem/recommendation.h"
#include "../../cryptocurrencyRecommendation/utils/utils.h"

/*  Read arguments */
int readArguments(int argc, char **argv, std::string& usersFile, std::string& vectorTweetsFile, std::string& coinsFile, std::string& lexiconFile, std::string& outputFile, int& validate);

/* Read files and set users, posts, coins and lexicon */
void setData(std::string& usersFile, std::vector<User>& users, std::vector<ItemToken>& tokenPosts, std::string& vectorTweetsFile, std::vector<Item>& vectorPosts, std::string& coinsFile, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, std::string& lexiconFile, std::unordered_map<std::string, double>& lexicon, errorCode& status);
// Petropoulakis Panagiotis
