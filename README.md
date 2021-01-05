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
where totalscore is the total sum of the corresponding scores(vader lexicon) of every word/symbol of all the tweets that refer to Bitcoin i.

The Cruptocurrenct Recommendation is performed in 4 different ways:
1. Recommend to user k new Bitcoins using [nearest-neighbor-search](https://github.com/PetropoulakisPanagiotis/nearest-neighbor-search) techniques based on the Uj vectors
2. Perform clustering on users Uj vectors. Recommend to user k new Bitcoins using [nearest-neighbor-search](https://github.com/PetropoulakisPanagiotis/nearest-neighbor-search) techniques based on the Uj vectors that reside in the same cluster
3. Perdorm clustering on users using the TFIDF representation. Each cluster "forms" a pseudo user with a Uj sentiment vector. Recommend to user k new Bitcoins using [nearest-neighbor-search](https://github.com/PetropoulakisPanagiotis/nearest-neighbor-search) techniques based on the next Uj vectors of pseudo users
4. Perdorm clustering on users using the TFIDF representation. Each cluster "forms" a pseudo user with a Uj sentiment vector.

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
