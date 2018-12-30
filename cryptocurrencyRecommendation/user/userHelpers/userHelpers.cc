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
/* Success: 0                      */
/* Failure: -1                      */
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

    status = SUCCESS;

    int i;

    /* Scan y id posts */
    for(i = 0; i < yIdPosts.size(); i++){
        if(yIdPosts[i] < 0){
            status = INVALID_ID_POSTS;
            return;
        }

        xIdPosts.push_back(yIdPosts[i]);
    } // End for
}

///////////////////////////////
/* Scaling and normalization */
///////////////////////////////

/* Scale sentiment: (-1, 1) */
void sentimentScaling(vector<double>& sentiment, double alpha=15){
    int i;

    if(alpha <= 0)
        return;

    /* Scale every individual sentiment */
    for(i = 0; i < sentiment.size(); i++){
        if(sentiment[i] == 0)
            continue;

        sentiment[i] = sentiment[i] / (sqrt((sentiment[i] * sentiment[i]) + alpha));
    } // End for - scaling
}

/* Normalize sentiment: sentiment[i] - average(sentiment) */
void sentimentNormalization(vector<double>& sentiment, vector<int>& unknownCoins){
    int i, totalActiveCoins = 0;
    double avgSentiment = 0;

    /* Find average sentiment */
    for(i = 0; i < sentiment.size(); i++){
        if(unknownCoins[i] != 0){
            avgSentiment += sentiment[i];
            totalActiveCoins++;
        }
    } // End for - Find average sentiment 

    /* Fix avg */
    if(totalActiveCoins != 0)
        avgSentiment /= totalActiveCoins;

    /* Normalize sentiment */
    for(i = 0; i < sentiment.size(); i++)
        sentiment[i] -= avgSentiment;
}

/* Find sentiment from current post and fix overall sentiment */
void fixSentiment(vector<double>& sentiment, vector<int>& unknownCoins, vector<unordered_set<string> >& allCoins, vector<string>& coins, unordered_map<string, double>& lexicon, int idPost, vector<ItemToken>& allPosts, errorCode& status){

    status = SUCCESS;

    /* Coins that current post reference to */
    vector<int> coinsOfPost;
    string currToken;
    int totalTokens, totalUniqueCoins; // Tokens in post and number of coins
    int i, j;
    unordered_set<string>::iterator coinsIter;
    double scorePost = 0;

    /* Set size */
    totalTokens = allPosts[idPost].getSizeTokens(status);
    if(status != SUCCESS)
        return;

    totalUniqueCoins = coins.size();

    /* Scan post */
    for(i = 0; i < totalTokens; i++){

        /* Get current token */
        currToken = allPosts[idPost].getToken(i, status);
        if(status != SUCCESS)
            return;

        /* Check if token is a coin */
        for(j = 0; j < totalUniqueCoins; j++){
            coinsIter = allCoins[j].find(currToken);

            /* Token is a coin */
            if(coinsIter != allCoins[j].end()){
                coinsOfPost.push_back(j);

                /* Coin is known */
                if(unknownCoins[j] == 0)
                    unknownCoins[j] = 1;
                break;
            }
        } // End for - Check token if it is a coin

        /* Find score for current token */
        scorePost += lexicon[currToken];
    } // End for - Scann tokens

    /* Add score in the corresponding coin(s) */
    for(i = 0; i < coinsOfPost.size(); i++)
        sentiment[coinsOfPost[i]] += scorePost;
}

/* Fix sentiment of the user */
void fixSentimentConstructor(vector<double>& sentiment, vector<int>& unknownCoins, vector<int>& idPosts, vector<unordered_set<string> >& allCoins, vector<string>& coins, unordered_map<string, double>& lexicon, vector<ItemToken>& allPosts, errorCode& status){
    int i;

    status = SUCCESS;

    for(i = 0; i < idPosts.size(); i++){

        /* Find sentiment from current post and fix overall sentiment */
        fixSentiment(sentiment, unknownCoins, allCoins, coins, lexicon, idPosts[i], allPosts, status);
        if(status != SUCCESS)
            return;
    } // For every post fix overall sentiment

    /* Scale and normalize sentiment */
    sentimentScaling(sentiment); 
    sentimentNormalization(sentiment, unknownCoins);
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
    for(i = 0; i < x.size(); i++){
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
    for(i = 0; i < x.size(); i++){
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
