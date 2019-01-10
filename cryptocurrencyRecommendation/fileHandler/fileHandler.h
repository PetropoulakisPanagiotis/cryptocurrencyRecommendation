#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "../utils/utils.h"
#include "../item/item.h"
#include "../itemToken/itemToken.h"
#include "../user/user.h"

/////////////////////////////////////
/* Functions for reading data sets */
/////////////////////////////////////

/* Read given file, extract items and read possible metrices(euclidean, cosine, etc) */
void readVectorPostsSet(std::string fileName, int withId, char delim, std::vector<Item>& vectorPosts, errorCode& status);

/* Read users, set users and posts */
void readUsersSet(std::string fileName, char delim, int totalPosts, std::vector<User>& users, std::vector<ItemToken>& tokenPosts, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, std::unordered_map<std::string, double>& lexicon, errorCode& status);

/* Read coins and return map and vector(with categories) */
void readCoinsSet(std::string fileName, char delim, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, errorCode& status);

/* Read lexicon with scores. Return map */
void readLexiconSet(std::string fileName, char delim, std::unordered_map<std::string, double>& lexicon, errorCode& status);
// Petropoulakyyis Panagiotis
