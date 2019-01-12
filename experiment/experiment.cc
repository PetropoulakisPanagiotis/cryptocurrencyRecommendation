#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
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
    chrono::steady_clock::time_point beginTimer, endTimer; // Timers
    errorCode status = SUCCESS; // Keep errors

    /* Read given arguments and set file names */
    argsStatus = readArguments(argc, argv, usersFile, vectorTweetsFile, coinsFile, lexiconFile, outputFile, validate);
    if(argsStatus == -1){
        cout << "Please give valid arguments. Try again later\n";
        return 0;
    }

    cout << "Welcome to cryptocurrency recommendation\n";
    cout << "----------------------------------------\n";

    cout << "Recommendation$: Reading input files\n";

    /* Set users, posts, coins and lexicon aka read files and set vectors etc */
    setData(usersFile, users, tokenPosts, vectorTweetsFile, vectorPosts, coinsFile, allCoins, coins, lexiconFile, lexicon, status);
    if(status != SUCCESS){
        printError(status);
        return 0;
    }


    /* Open output file */
    ofstream outputStream;

    outputStream.open(outputFile, ios::trunc);
    if(!outputStream){
        cout << "Can't open given output file\n";
        return 0;
    }

    /* Pick recommendation model */
    recommendation* recommendationModel;

    cout << "Recommendation$: Creating model\n";

    /* Create model */
    beginTimer = chrono::steady_clock::now();
    recommendationModel = new recommendation(tokenPosts, vectorPosts, users, allCoins, coins, lexicon, status);
    if(status != SUCCESS){
        printError(status);
        delete recommendationModel;
        return 0;
    }
    endTimer = chrono::steady_clock::now();

    cout << "Recommendation$: Model created[in: " << chrono::duration_cast<chrono::microseconds>(endTimer - beginTimer).count() / 1000000.0 << " sec]\n";

    /* Predict coins */
    if(validate == 0){
        /* Vectors to save predicted coins */
        vector<vector<int> > predictedLshUsers;
        vector<vector<int> > predictedLshPseudoUsers;
        vector<vector<int> > predictedClusteringUsers;
        vector<vector<int> > predictedClusteringPseudoUsers;
        int i, j;

        cout << "Recommendation$: Fitting lsh method\n";

        /* Fit model with lsh method */
        beginTimer = chrono::steady_clock::now();
        recommendationModel->fitLsh(predictedLshUsers, predictedLshPseudoUsers, status);
        if(status != SUCCESS){
            printError(status);
            delete recommendationModel;
            return 0;
        }
        endTimer = chrono::steady_clock::now();
        cout << "Recommendation$: Lsh method fitted[in: " << chrono::duration_cast<chrono::microseconds>(endTimer - beginTimer).count() / 1000000.0 << " sec]\n";

        cout << "Recommendation$: Fitting clustering method\n";

        /* Fit model with clustering method */
        beginTimer = chrono::steady_clock::now();
        recommendationModel->fitClustering(predictedClusteringUsers, predictedClusteringPseudoUsers, status);
        if(status != SUCCESS){
            printError(status);
            delete recommendationModel;
            return 0;
        }
        endTimer = chrono::steady_clock::now();
        cout << "Recommendation$: Clustering method fitted[in: " << chrono::duration_cast<chrono::microseconds>(endTimer - beginTimer).count() / 1000000.0 << " sec]\n";

        cout << "Recommendation$: Writing results in file\n";

        /* Write in file */
        beginTimer = chrono::steady_clock::now();

        outputStream << "Method: lsh - users:\n";
        for(i = 0; i < (int)predictedLshUsers.size(); i++){
            outputStream << "user: " << i + 1 << " [";
            for(j = 0; j < (int)predictedLshUsers[i].size(); j++){
                outputStream << coins[predictedLshUsers[i][j]];
                if(j != (int)predictedLshUsers[i].size() - 1)
                    outputStream << ", ";
                else
                    outputStream << "]\n";
            }
        }

        outputStream << "\nMethod: lsh - pseudo users:\n";
        for(i = 0; i < (int)predictedLshPseudoUsers.size(); i++){
            outputStream << "user: " << i + 1 << " [";
            for(j = 0; j < (int)predictedLshPseudoUsers[i].size(); j++){
                outputStream << coins[predictedLshPseudoUsers[i][j]];
                if(j != (int)predictedLshPseudoUsers[i].size() - 1)
                    outputStream << ", ";
                else
                    outputStream << "]\n";
            }
        }

        outputStream << "\nMethod: clustering - users:\n";
        for(i = 0; i < (int)predictedClusteringUsers.size(); i++){
            outputStream << "user: " << i + 1 << " [";
            for(j = 0; j < (int)predictedClusteringUsers[i].size(); j++){
                outputStream << coins[predictedClusteringUsers[i][j]];
                if(j != (int)predictedClusteringUsers[i].size() - 1)
                    outputStream << ", ";
                else
                    outputStream << "]\n";
            }
        }

        outputStream << "\nMethod: clustering - pseudo users:\n";
        for(i = 0; i < (int)predictedClusteringPseudoUsers.size(); i++){
            outputStream << "user: " << i + 1 << " [";
            for(j = 0; j < (int)predictedClusteringPseudoUsers[i].size(); j++){
                outputStream << coins[predictedClusteringPseudoUsers[i][j]];
                if(j != (int)predictedClusteringPseudoUsers[i].size() - 1)
                    outputStream << ", ";
                else
                    outputStream << "]\n";
            }
        }

        endTimer = chrono::steady_clock::now();
        cout << "Recommendation$: Results have been written[in: " << chrono::duration_cast<chrono::microseconds>(endTimer - beginTimer).count() / 1000000.0 << " sec]\n";

    }
    /* Validate method */
    else{


    }


    cout << "Recommendation$: Experiment is over. Have a good day!\n";

    outputStream.close();
    delete recommendationModel;

    return 0;
}
// Petropoulakis Panagiotis
