#pragma once
#include <string>
#include <vector>
#include "../utils/utils.h"

/* Item token class contains tokens */
class ItemToken{
    private:
        int id;
        std::vector<std::string> tokens;
        int tokensSize;
        static int count;

    public:
        ItemToken(std::vector<std::string>& tokens, errorCode& status);

        /* Accessors */
        int getId(errorCode& status);
        int getSizeTokens(errorCode& status);
        std::string getToken(int index, errorCode& status);
        std::vector<std::string>* getTokens(errorCode& status);
};
// PetropoulakisPanagiotis
