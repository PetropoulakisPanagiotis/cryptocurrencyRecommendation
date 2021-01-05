[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
## Cryptocurrency Recommendation based on Tweets
<p align="center">
<img src="recommender.png" width="720px" height="450px"> <br /> <br />
</p>

:zap:Note: this assignment is based on two previous projects. Refer to them at first: 
1. [nearest-neighbor-search](https://github.com/PetropoulakisPanagiotis/nearest-neighbor-search)
2. [clustering](https://github.com/PetropoulakisPanagiotis/clustering)

# Explanation 
We have a set of tweets(T) and N users. There are four files:
1. ./sets/tweets_dataset_small.csv that contains the tokenized(preprocessed) tweets
2. ./sets/twitter_dataset_small_vector.csv that contains the corresponding tfidf reprsentation of these tweets
3. ./sets/coins_queries.csv that contains K bitcoins
4. ./sets/vader_lexicon.csv that contains words/symbols with a corresponding sentiment score<br/>

:zap:Assumption: each tweet refers only to a specific bitcoin

At first, we calculate a sentiment vector Uj for every user. This vector has K dimensions and every cell corresponds to a specific bitcoin. Each cell contains a sentiment score([-1,1]) that represents the sentiment of a specific user for a specific bitcoin. The sentiment score is calculated as follows: <br />
<img align="center" src="http://latex.codecogs.com/svg.latex?S_i&space;=&space;\frac{totalscore}{\sqrt{totalscore^2&space;&plus;&space;15}}&space;&space;" title="http://latex.codecogs.com/svg.latex?S_i = \frac{totalscore}{\sqrt{totalscore^2 + 15}} " />, <br /><br />
where totalscore is the total sum of the corresponding score(vader lexicon) of every word/symbol for all the tweets that cointains Bitcoin i.

Cryptocurrency Recommendation(User Based k-Nearest Neighbors). Every user has written some posts and he is represented by a sentiment vector. These posts are represented with two ways: tokenized form and vector form(Tfid). <br /> 
There are 4 available methods: <br />
* Recommendation via k-nearest neighbors with lsh[1]
* Recommendation via k-nearest pseudo neighbors with lsh[2]
* Recommendation via k-nearest neighbors with clustering[3]
* Recommendation via k-nearest pseudo neighbors with clustering[4]
<br />
Note: There are also pseudo users. Pseudo users have been created with clustering. This clustering is performed in vector posts of users. After that every pseudo user owns the corresponding token posts of his cluster. Finaly these peusdo users have their own sentiment.

## [1] Method: Lhs - Users
For every user find with [lsh](https://github.com/PetropoulakisPanagiotis/neighbors-problem) method his k-nearest neighbors and recommend unknown coins.

## [2] Method Lsh - Pseudo Users
Same with method 1 but the k-nearest neighbors for every user are Pseudo Users from the initial clustering.

## [3] Method - Clustering - Users
First of all, clustering is performed. After that k-nearest neighbors for every user are the users that reside in the same cluster with the user's cluster.

## [4] Method - Clustering - Pseudo Users
Clustering is performed for pseudo users. In the next step for every user we find the nearest cluster and then the k-nearest neighbors of this user reside in the nearest cluster. 

## Installation
Clone this repository to your local machine: 
```
git clone https://github.com/PetropoulakisPanagiotis/cryptocurrencyRecommendation.git
```

## Replicate the results 
```
$ cd experiment
$ make
$ make run
```

## Author
Petropoulakis Panagiotis petropoulakispanagiotis@gmail.com
