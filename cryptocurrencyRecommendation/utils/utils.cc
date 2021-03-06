#include <iostream>
#include <vector>
#include <random>
#include <bitset>
#include <chrono>
#include <limits>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include "utils.h"

using namespace std;

/* Get random float number */
/* Type 0: uniform float   */
/* Type 1: standard float  */
/* Type 2: uniform float   */
float getRandom(int type, int w){
    /* Set random engine */
    static unsigned seed = chrono::system_clock::now().time_since_epoch().count();    
    static default_random_engine generator(seed);
        
    /* Set uniform and standard distribution */
    static uniform_real_distribution<float> uniformDist1(0, w); // [0,W)
    static uniform_int_distribution<int> uniformDist2(MY_MIN_RANDOM, MY_MAX_RANDOM);
    static normal_distribution<float> normalDist(0, 1); 

    if(type == 0)
        return uniformDist1(generator);
    else if(type == 1)
        return normalDist(generator);
    else if(type == 2)
        return (float)uniformDist2(generator);
    else
        return -1;
}

/* Get mod of given number */
int myMod(int x, int y){
    return ((x % y) + y) % y;
}

/* Sum given numbers - check for overflow */
double mySumDouble(double x, double y, errorCode& status){

    status = SUM_OVERFLOW;

    if(y > 0 && x > numeric_limits<double>::max() - y)
        return 0;
   
    if(y < 0 && x < numeric_limits<double>::lowest() - y)
        return 0;
   
    status = SUCCESS;
    
    return x + y;
}

/* Subtract given numbers - check for overflow */
double mySubDouble(double x, double y, errorCode& status){

    status = SUB_OVERFLOW;

    if(y > 0 && x < numeric_limits<double>::lowest() + y)
        return 0;
   
    if(y < 0 && x > numeric_limits<double>::max() + y)
        return 0;  
    
    status = SUCCESS;
    
    return x - y;
}

/* Multiply given numbers - check for overflow */
double myMultDouble(double x, double y, errorCode& status){

    status = MULT_OVERFLOW;

    if(x > 0 && y > 0)
        if(x > numeric_limits<double>::max() / y)
            return 0;
    
    if(x > 0 && y <= 0)
        if(y < numeric_limits<double>::lowest() / x)
            return 0;
    
    if(x < 0 && y > 0)
        if(x < numeric_limits<double>::lowest() / y)
            return 0;

    if(x < 0 && y <= 0)
        if(x != 0 && y < numeric_limits<double>::max() / x)
            return 0;

    status = SUCCESS;
    
    return x * y;
}

/* Divide given numbers - check for overflow */
double myDivDouble(double x, double y, errorCode& status){

    status = DIV_OVERFLOW;

    if(y == 0 || (x == numeric_limits<double>::lowest() && y == -1))
        return 0;  
    
    status = SUCCESS;
    
    return x / y;
}

/* Sum given numbers - check for overflow */
int mySumInt(int x, int y, errorCode& status){

    status = SUM_OVERFLOW;

    if(y > 0 && x > numeric_limits<int>::max() - y)
        return 0;
   
    if(y < 0 && x < numeric_limits<int>::lowest() - y)
        return 0;
    
    status = SUCCESS;
    
    return x + y;
}

/* Subtract given numbers - check for overflow */
int mySubInt(int x, int y, errorCode& status){

    status = SUB_OVERFLOW;

    if(y > 0 && x < numeric_limits<int>::lowest() + y)
        return 0;
   
    if(y < 0 && x > numeric_limits<int>::max() + y)
        return 0;  
    
    status = SUCCESS;
    
    return x - y;
}

/* Multiply given numbers - check for overflow */
int myMultInt(int x, int y, errorCode& status){

    status = MULT_OVERFLOW;

    if(x > 0 && y > 0)
        if(x > numeric_limits<int>::max() / y)
            return 0;
    
    if(x > 0 && y <= 0)
        if(y < numeric_limits<int>::lowest() / x)
            return 0;
    
    if(x < 0 && y > 0)
        if(x < numeric_limits<int>::lowest() / y)
            return 0;

    if(x < 0 && y <= 0)
        if(x != 0 && y < numeric_limits<int>::max() / x)
            return 0;

    status = SUCCESS;
    
    return x * y;
}

/* Divide given numbers - check for overflow */
int myDivInt(int x, int y, errorCode& status){

    status = DIV_OVERFLOW;

    if(y == 0 || (x == numeric_limits<int>::lowest() && y == -1))
        return 0;  
    
    status = SUCCESS;
    
    return x / y;
}

/* Hamming distance of two integers */
int hammingDistance(int x, int y){
    return bitset<32>(x^y).count();
}

/* Print type of error */
void printError(errorCode& status){

    switch(status){
        case(SUCCESS):
            cout << "No error occured\n";
            break;

        case(INVALID_INDEX):
            cout << "Please give a valid index\n";
            break;
        
        case(INVALID_DIM):
            cout << "Please give a valid positive dimension up to: ";
            cout << MAX_DIM;
            cout << "\n";
            break;
        
        case(INVALID_HASH_FUNCTION):
            cout << "Hash function is broken. Has not been initialized properly\n";
            break;

        case(INVALID_COMPARE):
            cout << "Can't compare different types of hash functions\n";
            break;
    
        case(SUM_OVERFLOW):
            cout << "Can't add given numbers. Possible overflow occures\n";
            break;
        
        case(SUB_OVERFLOW):
            cout << "Can't subtract given numbers. Possible overflow occures\n";
            break;
        
        case(MULT_OVERFLOW):
            cout << "Can't multiply given numbers. Possible overflow occures\n";
            break;
        
        case(DIV_OVERFLOW):
            cout << "Can't divide given numbers. Possible overflow occures\n";
            break;

        case(ALLOCATION_FAILED):
            cout << "Allocation error occured. Check that you have enough memory or change your given parameters\n";
            break;
       
        case(INVALID_METHOD):
            cout << "Method is broken\n";
            break;

        case(INVALID_PARAMETERS):
            cout << "Invalid parameters given\n";
            break;

        case(INVALID_POINTS):
            cout << "Please give an apropriate number of items[" + to_string(MIN_POINTS) + "," + to_string(MAX_POINTS) +"]\n";
            break;
      
        case(METHOD_ALREADY_USED):
            cout << "Method is fitted\n";
            break;
        
        case(METHOD_UNFITTED):
            cout << "Method isn't fitted\n";
            break;
        
        case(INVALID_RADIUS):
            cout << "Please give an apropriate radius[" + to_string(MIN_RADIUS) + "," + to_string(MAX_RADIUS) +"]\n";
            break;
        
        case(INVALID_DATA_SET):
            cout << "Please give a valid data set\n";
            break;
        
        case(METHOD_NOT_IMPLEMENTED):
            cout << "Method not implemented for current class\n";
            break;

        case(INVALID_METRICE):
            cout << "Models does not support give metrice\n";
            break;

        case(INVALID_ITER):
            cout << "Please give an apropriate number of max iterations[" + to_string(MIN_ITER) + "," + to_string(MAX_ITER) +"]\n";
            break;

        case(INVALID_CLUSTERS):
            cout << "Please give an apropriate number of clusters[" + to_string(MIN_CLUSTERS) + "," + to_string(MAX_CLUSTERS) +"]\n";
            break;

        case(INVALID_ALGO):
            cout << "Please give valid algorithms\n";
            break;

        case(INVALID_TOL):
            cout << "Please give an apropriate tolerance[" + to_string(MIN_TOL) + "," + to_string(MAX_TOL) +"]\n";
            break;

        case(INVALID_CONF):
            cout << "Please give a valid config set\n";
            break;

        case(INVALID_COEF_RADIUS):
            cout << "Please give an apropriate radius coefficient[" + to_string(MIN_COEF_RADIUS) + "," + to_string(MAX_COEF_RADIUS) +"]\n";
            break;

        case(INVALID_ITEM_TOKEN):
            cout << "Item token has not been created properly\n";
            break;

        case(INVALID_TOKENS):
            cout << "Please give valid tokens\n";
            break;

        case(INVALID_ID_POSTS):
            cout << "Please give valid posts\n";
            break;

        case(INVALID_ALL_COINS):
            cout << "Please give valid coins\n";
            break;

        case(INVALID_LEXICON):
            cout << "Please give valid lexicon\n";
            break;

        case(INVALID_COINS):
            cout << "Please give valid categories of coins\n";
            break;

        case(INVALID_POSTS):
            cout << "Please give valid posts\n";
            break;

        case(INVALID_USER_ID):
            cout << "Please give valid id of user\n";
            break;

        case(INVALID_USER):
            cout << "Given user has not been created properly\n";
            break;

        case(INVALID_USER_NEIGHBORS):
            cout << "Please give valid user neighbors\n";
            break;

        case(INVALID_P):
            cout << "Please give an apropriate p (more than" + to_string(MIN_P) + ")\n";
            break;
    } // End switch
}

// Petropoulakis Panagiotis
