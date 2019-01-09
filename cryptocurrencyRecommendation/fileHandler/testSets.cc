#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../utils/utils.h"
#include "fileHandler.h"

using namespace std;

int main(void){
    unordered_map<string, double> lexicon;
    char delimLexicon = '\t';
    errorCode status;

    readLexiconSet("../../experiment/sets/vader_lexicon.csv", delimLexicon, lexicon, status);
    printf("Lexicon status: \n");
    printError(status);

    cout << "$: score: "<< lexicon["$:"] << "\n";
    cout << ">:[ score: "<< lexicon[">:["] << "\n";

    vector<unordered_set<string> > allCoins;
    vector<string> coins;
    char delimCoins = '\t';

    readCoinsSet("../../experiment/sets/coins_queries.csv", delimLexicon, allCoins, coins, status);
    printf("Coins status: \n");
    printError(status);

    cout << "Coin1: " << coins[0] << "\n";
    cout << "Coin4: " << coins[3] << "\n";

    cout << "Find #nas: " << *(allCoins[20].find("#nas")) << "\n";

    return 0;
}
// Petropoulakyyis Panagiotis

