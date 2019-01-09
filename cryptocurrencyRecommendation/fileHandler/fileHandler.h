#pragma once
#include <vector>
#include <list>
#include <unordered_map>
#include <string>
#include "../utils/utils.h"
#include "../item/item.h"

/* Functions for reading data set and query set */

/* Read given file, extract items and read possible metrices(euclidean, cosine, etc) */
void readVectorPostsSet(std::string fileName, int withId, char delim, std::list<Item>& points, errorCode& status); 

void readTokenPostsSet();

void readUsersSet();

void readCoinsSet();

/* Read lexicon with scores. Return map */
void readLexiconSet(std::string fileName, char delim, std::unordered_map<std::string, double>& lexicon, errorCode& status);
// Petropoulakyyis Panagiotis
