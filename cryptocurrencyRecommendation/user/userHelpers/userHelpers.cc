#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>
#include "../../utils/utils.h"
#include "../../itemToken/itemToken.h"
#include "../user.h"

using namespace std;

/* Check parameters of constructor */
void checkParamsConstructor(int id, vector<int>& idPosts, vector<unordered_set<string> >& allCoins, vector<string>& coins, unordered_map<string, double>& lexicon, vector<ItemToken>& allPosts, errorCode& status){

    status = SUCCESS;

    if(idPosts.size() <= 0 || idPosts.size() > MAX_ID_POSTS){
        status = INVALID_ID_POSTS;
        return;
    }
    else if(allCoins.size() <= 0){
        status = INVALID_ALL_COINS;
        return;
    }
    else if(coins.size() <= 0 || allCoins.size() != coins.size()){
        status = INVALID_COINS;
        return;
    }
    else if(lexicon.size() <= 0){
        status = INVALID_COINS;
        return ;
    }
    else if(allPosts.size() <= 0 || allPosts.size() > MAX_POSTS || allPosts.size() < idPosts.size()){
        status = INVALID_POSTS;
        return;
    }
    else if(id < 0){
        status = INVALID_USER_ID;
        return;
    }
}

/* Copy id posts */
void setIdPostsConstructor(vector<int>& xIdPosts, vector<int>& yIdPosts, errorCode& status){
    int i;

    status = SUCCESS;

    /* Scan y id posts */
    for(i = 0; i < (int)yIdPosts.size(); i++){
        if(yIdPosts[i] < 0){
            status = INVALID_ID_POSTS;
            return;
        }

        xIdPosts.push_back(yIdPosts[i]);
    } // End for
}

///////////////////
/* Normalization */
///////////////////

/* Scale sentiment: (-1, 1) */
int sentimentNormalization(vector<double>& sentiment, double alpha=15){
    int i;
    int flag = 0; // For invalid member 

    if(alpha <= 0)
        alpha = 15;

    /* Scale every individual sentiment */
    for(i = 0; i < (int)sentiment.size(); i++){
        if(sentiment[i] == 0)
            continue;

        sentiment[i] = sentiment[i] / (sqrt((sentiment[i] * sentiment[i]) + alpha));
        if(sentiment[i] != 0 && flag == 0)
            flag = 1;
    } // End for - normalization

    return flag;
}

/* Find sentiment from current post and fix the overall sentiment */
void fixSentiment(vector<double>& sentiment, vector<int>& unknownCoins, vector<unordered_set<string> >& allCoins, vector<string>& coins, unordered_map<string, double>& lexicon, int idPost, vector<ItemToken>& allPosts, errorCode& status){
    vector<int> coinsOfPost; // Coins that current post reference to
    string currToken;
    int totalTokens, totalUniqueCoins; // Tokens in post and number of coins
    int i, j, tokenIsCoin;
    unordered_set<string>::iterator coinsIter; // Scan coins set
    double scorePost = 0; // Score of current post

    status = SUCCESS;

    /* Set number of tokens of current post */
    totalTokens = allPosts[idPost].getSizeTokens(status);
    if(status != SUCCESS)
        return;

    if(totalTokens <= 0){
        status = INVALID_POSTS;
        return;
    }

    /* Get the number of unique coins */
    totalUniqueCoins = coins.size();

    /* Scan post - per token */
    for(i = 0; i < totalTokens; i++){

        /* Get current token */
        currToken = allPosts[idPost].getToken(i, status);
        if(status != SUCCESS)
            return;

        tokenIsCoin = 0;

        /* Check if token is a coin */
        for(j = 0; j < totalUniqueCoins; j++){

            /* Scan current set */
            coinsIter = allCoins[j].find(currToken);

            /* Token is a coin */
            if(coinsIter != allCoins[j].end()){
                coinsOfPost.push_back(j); // Post is reference in current coin

                /* Coin is known */
                if(unknownCoins[j] == 0)
                    unknownCoins[j] = 1;

                tokenIsCoin = 1;
                break;
            }
        } // End for - Check if token is a coin

        if(tokenIsCoin == 1)
            continue;

        /* Find score for current token */
        scorePost += lexicon[currToken];
    } // End for - Scann tokens

    /* Add score in the corresponding coin(s) */
    for(i = 0; i < (int)coinsOfPost.size(); i++)
        sentiment[coinsOfPost[i]] += scorePost;
}

/* Fix sentiment of the user */
int fixSentimentConstructor(vector<double>& sentiment, vector<int>& unknownCoins, vector<int>& idPosts, vector<unordered_set<string> >& allCoins, vector<string>& coins, unordered_map<string, double>& lexicon, vector<ItemToken>& allPosts, errorCode& status){
    int i;
    int flag;

    status = SUCCESS;

    /* Scan all posts of the user */
    for(i = 0; i < (int)idPosts.size(); i++){

        /* Find sentiment from current post and fix the overall sentiment */
        fixSentiment(sentiment, unknownCoins, allCoins, coins, lexicon, idPosts[i], allPosts, status);
        if(status != SUCCESS)
            return -1;
    } // For every post fix overall sentiment

    /* Normalize sentiment */
    flag = sentimentNormalization(sentiment);

    return flag;
}

/* Norm of vector */
double norm(vector<double>& x, errorCode& status){
    double norm = 0, tempMult;
    int i;

    status = SUCCESS;

    /* Check dimensions */
    if(x.size() == 0){
        status = INVALID_DIM;
        return -1;
    }

    /* Calculate norm */
    for(i = 0; i < (int)x.size(); i++){
        tempMult = x[i] * x[i];

        norm += tempMult;
    } // End for

    norm = sqrt(norm);

    return norm;
}

/* Calculate inner product of two vectors */
double innerProduct(vector<double>& x, vector<double>& y, errorCode& status){
    double product = 0, tempMult;
    int i;

    status = SUCCESS;

    /* Check dimensions */
    if(x.size() == 0 || y.size() == 0){
        status = INVALID_DIM;
        return -1;
    }

    if(x.size() != y.size()){
        status = INVALID_DIM;
        return -1;
    }

    /* Calculate product */
    for(i = 0; i < (int)x.size(); i++){
        tempMult = x[i] * y[i];

        product += tempMult;
    } // End for

    return product;
}

/* Similarity function of two vectors */
double similarityFunc(std::vector<double>& x, std::vector<double>& y, errorCode& status){
    double dist = 0, mult;
    double normX, normY;

    status = SUCCESS;

    /* Check dimensions */
    if(x.size() == 0 || y.size() == 0){
        status = INVALID_DIM;
        return -1;
    }

    if(x.size() != y.size()){
        status = INVALID_DIM;
        return -1;
    }

    dist = innerProduct(x, y, status);
    if(status != SUCCESS)
        return -1;

    normX = norm(x, status);
    if(status != SUCCESS)
        return -1;

    normY = norm(y, status);
    if(status != SUCCESS)
        return -1;

    mult = normX * normY;

    if(mult == 0){
        status = DIV_OVERFLOW;
        return -1;
    }

    dist = dist /  mult;

    return dist;
}

// PetropoulakisPanagiotis
