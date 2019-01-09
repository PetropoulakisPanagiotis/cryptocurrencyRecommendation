#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include "../item/item.h"
#include "../utils/utils.h"
#include "../itemToken/itemToken.h"
#include "../user/user.h"
#include "fileHandler.h"

using namespace std;

/* Read given file and extract points */
/* WithId == 0, points have not id's  */
/* WithId == 1, points have id's      */
void readVectorPostsSet(string fileName, int withId, char delim, vector<Item>& points, errorCode& status){
    ifstream file; 
    string line, word; // Line is splitted in words
    int i, wordsSize, specialChar;
    double currComponent;
    
    /* Structures */
    set<string> ids; // Keep all ids - Check if all ids are unique
    set<string>::iterator iterSet; // Iterate through ids
    string::iterator iterStr; // Iteratre through word

    /* Note: words in file == points/id/metrices */
    status = SUCCESS;

    /* Check parameters */
    if(fileName.length() == 0 || (withId != 1 && withId != 0)){
        status = INVALID_PARAMETERS;
        return;
    }

    /* Clear points, types */
    points.clear();

    file.open(fileName);
    
    /* Check if file opened properly */
    if(!file){
        status = INVALID_DATA_SET;
        return;
    }
    
    /* Read lines in file */
    while(getline(file, line)){
        /* Discard empty lines */
        if(line.length() == 0)
            continue;

        /* For current points in line */
        vector<double> components;
        string id; 

        /* Split line */
        std::istringstream wordStream(line);
        vector<string> words; 

        /* Get points */
        while(getline(wordStream, word, delim))
            words.push_back(word);
       
        /* Receive id's - First word in line */
        if(withId == 1){
            iterSet = ids.find(words[0]); 

            /* Id exists - Invalid file */
            if(iterSet != ids.end()){
                status = INVALID_DATA_SET;
                return;
            }
            /* Add id in set */
            ids.insert(words[0]);

            /* Keep currend id */
            id = words[0];
        } // End if - id       

        string currWord; // Change form of word
        wordsSize = words.size();

        /* Read components */
        for(i = withId; i < wordsSize; i++){

            /* Handle word */
            currWord = words[i];

            specialChar = 0;

            /* Discard special characters */
            iterStr = currWord.begin();
            while(iterStr != currWord.end()){
                if((*iterStr < '0' || *iterStr > 'z') && *iterStr != '.' && *iterStr != '-'){
                    specialChar = 1;
                    break;
                }
                iterStr++;
            }

            /* Special characters found */
            if(specialChar == 1)
                continue;

            /* Handle negative numbers */
            if(currWord[0] == '-')
                currWord.erase(0,1);

            /* Handle floating points */
            currWord.erase(remove(currWord.begin(), currWord.end(), '.'), currWord.end());

            /* Check if component is digit */
            iterStr = currWord.begin();
            while(iterStr != currWord.end() && isdigit(*iterStr))
                iterStr++;

            /* Not digit */
            if(currWord.size() == 0 || iterStr != currWord.end()){
                status = INVALID_DATA_SET;
                return;
            }

            /* Convert string to double */
            try{
                currComponent = stod(words[i]);
            }
            catch(...){
                status = INVALID_POINTS;
                return;
            }

            /* Valid component - Save it */
            components.push_back(currComponent);
        } // End for
       
        /* Number of points is too big */
        if(points.size() == MAX_POINTS){
            status = INVALID_POINTS;
            return;
        }

        /* Create new item */
        if(withId == 1){
            points.push_back(Item(id,components,status));
            if(status != SUCCESS){
                return;
            }
        }
        else{
            points.push_back(Item(components,status));
            if(status != SUCCESS){
                return;
            }
        }

    } // End while - Read line

    /* Small number of points */
    if(points.size() < MIN_POINTS){
        status = INVALID_POINTS;
        return;
    }

    file.close();
}

/* Read users, set users and posts */
void readUsersSet(string fileName, char delim, vector<User>& users, vector<ItemToken>& tokenPosts, vector<unordered_set<string> >& allCoins, vector<string>& coins, unordered_map<string, double>& lexicon, errorCode& status){
    ifstream file;
    string line, word; // Line is splitted in words
    int i, wordsSize;
    int indexUser = 0;

    string currIdUser, prevIdUser="none", currIdPost;
    vector<vector<int> > idPostUsers;
    vector<int> idUsers;
    int intId, intIdPost;

    status = SUCCESS;

    /* Check parameters */
    if(fileName.length() == 0){
        status = INVALID_PARAMETERS;
        return;
    }

    /* Clear */
    users.clear();
    tokenPosts.clear();

    file.open(fileName);

    /* Check if file opened properly */
    if(!file){
        status = INVALID_USER;
        return;
    }

    /* Read lines in file */
    while(getline(file, line)){
        /* Discard empty lines */
        if(line.length() == 0)
            continue;

        /* Split line */
        std::istringstream wordStream(line);
        vector<string> words;

        /* Get tokens + score */
        while(getline(wordStream, word, delim))
            words.push_back(word);

        wordsSize = words.size();

        if(wordsSize < 3){
            status = INVALID_USER;
            return;
        }

        /* Read id */
        currIdUser = words[0];
        currIdPost = words[1];

        try{
            intId = stoi(currIdUser);
        }
        catch(...){
            status = INVALID_USER;
            return;
        }

        try{
            intIdPost = stoi(currIdPost);
        }
        catch(...){
            status = INVALID_USER;
            return;
        }

        vector<string> tokens;

        /* Read tokens */
        for(i = 2; i < wordsSize; i++){
            /* Fix tokens */
            tokens.push_back(words[i]);
        } // End for - read coins

        /* Fix token posts */
        tokenPosts.push_back(ItemToken(tokens, status));
        if(status != SUCCESS){
            status = INVALID_USER;
            return;
        }

        /*  First user */
        if(prevIdUser == "none"){
            idUsers.push_back(intId);
            idPostUsers.push_back(vector<int>());
            idPostUsers[indexUser].push_back(intIdPost);
        }
        else if(prevIdUser != currIdUser){
            indexUser++;
            idUsers.push_back(intId);
            idPostUsers.push_back(vector<int>());
            idPostUsers[indexUser].push_back(intIdPost);
        }
        else{
            idPostUsers[indexUser].push_back(intIdPost);
        }

        prevIdUser = currIdUser;
    } // End while - Read line

    /* Create users */
    for(i = 0; i < (int)idUsers.size(); i++){
        users.push_back(User(idUsers[i], idPostUsers[i], allCoins, coins, lexicon, tokenPosts, status));
        if(status != SUCCESS){
            status = INVALID_USER;
            return;
        }
    } // End for - create users

    file.close();
}

/* Read coins and return map and vector(with categories) */
void readCoinsSet(std::string fileName, char delim, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, errorCode& status){
    ifstream file;
    string line, word; // Line is splitted in words
    int i, wordsSize;
    int numLine = 0;

    status = SUCCESS;

    /* Check parameters */
    if(fileName.length() == 0){
        status = INVALID_PARAMETERS;
        return;
    }

    /* Clear coins */
    allCoins.clear();
    coins.clear();

    file.open(fileName);

    /* Check if file opened properly */
    if(!file){
        status = INVALID_COINS;
        return;
    }

    /* Read lines in file */
    while(getline(file, line)){
        /* Discard empty lines */
        if(line.length() == 0)
            continue;

        /* Split line */
        std::istringstream wordStream(line);
        vector<string> words;

        /* Get coins */
        while(getline(wordStream, word, delim))
            words.push_back(word);

        string currWordCoin; // Change form of word
        wordsSize = words.size();

        if(wordsSize < 1){
            status = INVALID_COINS;
            return;
        }

        allCoins.push_back(unordered_set<string>());

        /* Read coins */
        for(i = 0; i < wordsSize; i++){

            /* Read words */
            currWordCoin = words[i];

            /* Fix set */
            allCoins[numLine].insert(currWordCoin);
        } // End for - read coins

        /* Fix vector */
        if(wordsSize >= 5)
            coins.push_back(words[4]);
        else
            coins.push_back(words[0]);

        numLine++;
    } // End while - Read line

    file.close();
}

/* Read lexicon with scores. Return map */
void readLexiconSet(string fileName, char delim, unordered_map<string, double>& lexicon, errorCode& status){
    ifstream file;
    string line, word; // Line is splitted in words
    int wordsSize;

    status = SUCCESS;

    /* Check parameters */
    if(fileName.length() == 0){
        status = INVALID_PARAMETERS;
        return;
    }

    /* Clear lexicon */
    lexicon.clear();

    file.open(fileName);

    /* Check if file opened properly */
    if(!file){
        status = INVALID_LEXICON;
        return;
    }


    /* Read lines in file */
    while(getline(file, line)){
        /* Discard empty lines */
        if(line.length() == 0)
            continue;

        /* Split line */
        std::istringstream wordStream(line);
        vector<string> words;

        /* Get tokens + score */
        while(getline(wordStream, word, delim))
            words.push_back(word);


        string currWordKey, currWordScore; // Change form of word
        double currScore;
        wordsSize = words.size();

        if(wordsSize != 2){
            lexicon.clear();
            status = INVALID_LEXICON;
            return;
        }

        /* Read words */
        currWordKey = words[0];
        currWordScore = words[1];

        /* Convert string to double */
        try{
            currScore= stod(currWordScore);
        }
        catch(...){
            status = INVALID_LEXICON;
            lexicon.clear();
            return;
        }

        /* Fix map */
        lexicon.insert(make_pair(currWordKey, currScore));
    } // End while - Read line

    file.close();
}

// Petropoulakis Panagiotis
