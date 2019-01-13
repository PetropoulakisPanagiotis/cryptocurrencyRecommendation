#pragma once
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include "../../utils/utils.h"
#include "../../itemToken/itemToken.h"
#include "../user.h"

/* Stracture for recommendation  */
/* Use it for heap and extract p */
/* highest nodes                 */
typedef struct newCoinNode{
    int pos;
    double val;

    newCoinNode(int pos, double val){
        this->pos = pos;
        this->val = val;
    }
}newCoinNode;

/* Create a compare class based on val of coin node */
struct newCoinNodeCompare{
    bool operator()(const newCoinNode& x, const newCoinNode& y) const{
        return x.val < y.val;
    }
};

///////////////////////////////
/* Constructor basic helpers */
///////////////////////////////
void checkParamsConstructor(int id, std::vector<int>& idPosts, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, std::unordered_map<std::string, double>& lexicon, std::vector<ItemToken>& allPosts, errorCode& status);
void setIdPostsConstructor(std::vector<int>& xIdPosts, std::vector<int>& yIdPosts, errorCode& status);

///////////////////////////////
/* Scaling and normalization */
///////////////////////////////
int sentimentNormalization(std::vector<double>& sentiment, std::vector<int>& unknownCoins);

///////////////////////
/* Sentiment helpers */
///////////////////////
void fixSentiment(std::vector<double>& sentiment, std::vector<int>& unknownCoins, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, std::unordered_map<std::string, double>& lexicon, int idPost, std::vector<ItemToken>& allPosts, errorCode& status);
int fixSentimentConstructor(std::vector<double>& sentiment, std::vector<int>& unknownCoins, std::vector<int>& idPosts, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, std::unordered_map<std::string, double>& lexicon, std::vector<ItemToken>& allPosts, errorCode& status);

//////////////////////////////////////////////////////////////////
/* Norm, inner product and similarity function - for users, etc */
//////////////////////////////////////////////////////////////////
double norm(std::vector<double>& x, errorCode& status);
double innerProduct(std::vector<double>& x, std::vector<double>& y, errorCode& status);
double similarityFunc(std::vector<double>& x, std::vector<double>& y, int metrice, errorCode& status);
// PetropoulakisPanagiotis
