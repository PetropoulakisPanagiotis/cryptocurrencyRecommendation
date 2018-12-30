#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../itemToken/itemToken.h"
#include "../utils/utils.h"
#include "user.h"

using namespace std;

int main(void){
    errorCode status;
    vector<string> coins{"bitcoin", "xp", "eth", "urio"};
    vector<unordered_set<string> > allCoins({
                                    {"bitcoin", "bitc"},
                                    {"xp","$xp"},
                                    {"eth", "$eth"},
                                    {"urio", "$urio"}});

    unordered_map<string, double> lexicon({
                                    {"good", 0.7},
                                    {"bad", -0.7},
                                    {"very", 0.8},
                                    {"cool", 0.55},
                                    {"worst", -1}});

    vector<ItemToken> allPosts;
    vector<string> tokens;

    /* Fix posts */
    tokens.push_back("good");
    tokens.push_back("very");
    tokens.push_back("eth");
    allPosts.push_back(ItemToken(tokens, status));

    tokens.clear();
    tokens.push_back("bad");
    tokens.push_back("xp");
    allPosts.push_back(ItemToken(tokens, status));

    tokens.clear();
    tokens.push_back("cool");
    tokens.push_back("$urio");
    allPosts.push_back(ItemToken(tokens, status));

    tokens.clear();
    tokens.push_back("very");
    tokens.push_back("bad");
    tokens.push_back("bitcoin");
    allPosts.push_back(ItemToken(tokens, status));

    /* Create users */
    vector<int> idPosts;
    vector<double>* sent;

    idPosts.push_back(0);
    idPosts.push_back(1);

    int x = 0;

    User* user1;
    User* user2;

    user1 = new User(x, idPosts, allCoins, coins, lexicon, allPosts, status);

    idPosts.clear();
    idPosts.push_back(2);
    idPosts.push_back(3);

    user2 = new User(5, idPosts, allCoins, coins, lexicon, allPosts, status);

    cout << "user1-id: " << user1->getId(status) << "\n";
    cout << "user2-id: " << user2->getId(status) << "\n";

    cout << "user1-total posts: " << user1->getSizeOfPosts(status) << "\n";
    cout << "user2-total posts: " << user2->getSizeOfPosts(status) << "\n";

    /* Print unknown coins */
    vector<int>* unknownCoins1;
    vector<int>* unknownCoins2;
    int i;

    unknownCoins1 = user1->getUnknownCoins(status);
    unknownCoins2 = user2->getUnknownCoins(status);

    for(i = 0; i < unknownCoins1->size(); i++){
        cout << "user1-UC: " << unknownCoins1->at(i) << "\n";
    }

    cout << "\n";

    for(i = 0; i < unknownCoins2->size(); i++){
        cout << "user2-UC: " << unknownCoins2->at(i) << "\n";
    }

    cout << "\n\n";

    /* Print sentiment */
    vector<double>* s1;
    vector<double>* s2;

    s1 = user1->getSentiment(status);
    s2 = user2->getSentiment(status);

    for(i = 0; i < s1->size(); i++){
        cout << "user1-S: " << s1->at(i) << "\n";
    }

    cout << "\n";

    for(i = 0; i < s2->size(); i++){
        cout << "user2-S: " << s2->at(i) << "\n";
    }

    delete user1;
    delete user2;

    return 0;
}
// PetropoulakisPanagiotis
