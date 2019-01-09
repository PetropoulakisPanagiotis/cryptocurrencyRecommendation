#include <iostream>
#include <string>
#include <unordered_map>
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

    printf("$: score: %lf \n", lexicon["$:"]);
    printf(">:[ score: %lf \n", lexicon[">:["]);

    return 0;
}
// Petropoulakyyis Panagiotis

