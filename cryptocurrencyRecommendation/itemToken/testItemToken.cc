#include <iostream>
#include <string>
#include <vector>
#include "../utils/utils.h"
#include "itemToken.h"

using namespace std;

int main(void){
    vector<string> x1;
    vector<string> x2;
    errorCode status;

    x1.push_back("hi");
    x1.push_back("nikos");

    x2.push_back("hallo");
    x2.push_back("papas");

    ItemToken tweet1(x1,status);
    printError(status);

    ItemToken tweet2(x2,status);
    printError(status);

    cout << "tweet1: " << tweet1.getId(status) << "\n";
    cout << "tweet2: " << tweet2.getId(status) << "\n";

    cout << "tweet1: " << tweet1.getToken(0 ,status) << "\n";
    cout << "tweet1: " << tweet1.getToken(2 ,status) << "\n";
    printError(status);

    vector<string>* vecPtr;

    vecPtr = tweet1.getTokens(status);
    vector<string>& vecRef = *vecPtr;

    cout << vecRef[1] << "\n";

    return 0;
}
// PetropoulakisPanagiotis
