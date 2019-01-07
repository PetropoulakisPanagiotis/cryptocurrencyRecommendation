#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../../utils/utils.h"
#include "../../itemToken/itemToken.h"
#include "../../item/item.h"
#include "../../user/user.h"

/* Check constructors parameters */
void checkParamsConstructor(std::vector<ItemToken>& tokenPosts, std::vector<Item>& vectorPosts, std::vector<User>& users, std::vector<std::unordered_set<std::string> >& allCoins, std::vector<std::string>& coins, std::unordered_map<std::string, double>& lexicon, int p, int k, errorCode& status);
// PetropoulakisPanagiotis
