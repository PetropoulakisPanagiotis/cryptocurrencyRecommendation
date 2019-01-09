#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../utils/utils.h"
#include "../user/user.h"
#include "../itemToken/itemToken.h"
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

    vector<User> users;
    vector<ItemToken> tokenPosts;
    char delimUsers = '\t';

    readUsersSet("../../experiment/sets/tweets_dataset_small.csv", delimUsers, users, tokenPosts, allCoins, coins, lexicon, status);
    printf("Users status: \n");
    printError(status);

    /* Check token posts */
    cout << "Check token posts 1\n";
    cout << "id: " << tokenPosts[0].getId(status) << "\n";
    cout << "token: " << tokenPosts[0].getToken(1, status) << "\n";

    cout << "Check token posts 5000\n";
    cout << "id: " << tokenPosts[4999].getId(status) << "\n";
    cout << "token: " << tokenPosts[4999].getToken(1, status) << "\n";

    /* Check users */
    vector<int>* idPosts;
    int i;

    cout << "User 1 id posts: \n";
    idPosts = users[0].getIdPosts(status);
    vector<int>& ref1 = *idPosts;

    for(i = 0; i < idPosts->size(); i++)
        cout << ref1[i] << "|";

    cout << "\n";

    cout << "User list id posts: \n";
    idPosts = users[users.size() - 1].getIdPosts(status);
    vector<int>& ref2 = *idPosts;

    for(i = 0; i < idPosts->size(); i++)
        cout << ref2[i] << "|";

    cout << "\n";
    return 0;
}
// Petropoulakyyis Panagiotis

