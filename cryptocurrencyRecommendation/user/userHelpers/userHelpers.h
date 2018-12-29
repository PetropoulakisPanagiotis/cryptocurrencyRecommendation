#pragma once
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include "../../utils/utils.h"
#include "../../itemToken/itemToken.h"
#include "../user.h"

/////////////////////////
/* Constructor helpers */
/////////////////////////
void checkParamsConstructor(int id, std::vector<int>& idPosts, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, std::unordered_map<std::string, double>& lexicon, std::vector<ItemToken>& allPosts, errorCode& status);
void setIdPostsConstructor(std::vector<int>& xIdPosts, std::vector<int>& yIdPosts, errorCode& status);

///////////////////////////////
/* Scaling and normalization */
///////////////////////////////
void sentimentScaling(std::vector<double>& sentiment, double alpha); 
void sentimentNormalization(std::vector<double>& sentiment, std::vector<int>& unknownCoins); 

/* Fix overall sentiment */
void fixSentiment(std::vector<double>& sentiment, std::vector<int>& unknownCoins, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, std::unordered_map<std::string, double>& lexicon, int idPost, std::vector<ItemToken>& allPosts, errorCode& status);
void fixSentimentConstructor(std::vector<double>& sentiment, std::vector<int>& unknownCoins, std::vector<int>& idPosts, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, std::unordered_map<std::string, double>& lexicon, std::vector<ItemToken>& allPosts, errorCode& status);
// PetropoulakisPanagiotis
