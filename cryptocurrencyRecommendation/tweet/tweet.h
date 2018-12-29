#pragma once
#include <string>
#include <vector>
#include "../utils/utils.h"

/* Tweet class contains a tweet with tokens */
class Tweet{
    private:
        int id;
        std::vector<std::string> tokens;
        int tokensSize;
        static int count;

    public:
        Tweet(std::vector<std::string>& tokens, errorCode& status);

        /* Accessors */
        int getSizeTokens(errorCode& status);
        std::string getToken(int index, errorCode& status);
        std::vector<std::string>* getTokens(errorCode& status);
};
// PetropoulakisPanagiotis
