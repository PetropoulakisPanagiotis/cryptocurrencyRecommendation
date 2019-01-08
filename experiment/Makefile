# Petropoulakis Panagiotis
CC = g++
FLAGS = -g -Wall

exmperiment: experiment.o utils.o hashFunction.o item.o fileHandler.o lshEuclidean.o lshCosine.o hypercubeEuclidean.o hypercubeCosine.o cluster.o initAlgorithms.o assignAlgorithms.o updateAlgorithms.o clusterHelpers.o user.o userHelpers.o itemToken.o recommendation.o recommendationHelpers.o recommendationAlgorithms.o
	$(CC) -o experiment $(FLAGS) experiment.o utils.o hashFunction.o item.o fileHandler.o lshEuclidean.o lshCosine.o hypercubeEuclidean.o hypercubeCosine.o cluster.o initAlgorithms.o assignAlgorithms.o updateAlgorithms.o clusterHelpers.o user.o userHelpers.o itemToken.o recommendation.o recommendationHelpers.o recommendationAlgorithms.o -std=c++11

experiment.o: experiment.cc
	$(CC) -c  $(FLAGS) experiment.cc -std=c++11

utils.o: ../cryptocurrencyRecommendation/utils/utils.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/utils/utils.cc -std=c++11

hashFunction.o: ../cryptocurrencyRecommendation/neighborsProblem/hashFunction/hashFunction.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/neighborsProblem/hashFunction/hashFunction.cc -std=c++11

item.o: ../cryptocurrencyRecommendation/item/item.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/item/item.cc -std=c++11

fileHandler.o: ../cryptocurrencyRecommendation/fileHandler/fileHandler.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/fileHandler/fileHandler.cc -std=c++11

lshEuclidean.o: ../cryptocurrencyRecommendation/neighborsProblem/model/lsh/lshEuclidean.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/neighborsProblem/model/lsh/lshEuclidean.cc -std=c++11

lshCosine.o: ../cryptocurrencyRecommendation/neighborsProblem/model/lsh/lshCosine.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/neighborsProblem/model/lsh/lshCosine.cc -std=c++11

hypercubeEuclidean.o: ../cryptocurrencyRecommendation/neighborsProblem/model/hypercube/hypercubeEuclidean.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/neighborsProblem/model/hypercube/hypercubeEuclidean.cc -std=c++11

hypercubeCosine.o: ../cryptocurrencyRecommendation/neighborsProblem/model/hypercube/hypercubeCosine.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/neighborsProblem/model/hypercube/hypercubeCosine.cc -std=c++11

cluster.o: ../cryptocurrencyRecommendation/clusteringProblem/cluster.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/clusteringProblem/cluster.cc -std=c++11

initAlgorithms.o: ../cryptocurrencyRecommendation/clusteringProblem/algorithms/initAlgorithms.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/clusteringProblem/algorithms/initAlgorithms.cc -std=c++11

assignAlgorithms.o: ../cryptocurrencyRecommendation/clusteringProblem/algorithms/assignAlgorithms.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/clusteringProblem/algorithms/assignAlgorithms.cc -std=c++11

updateAlgorithms.o: ../cryptocurrencyRecommendation/clusteringProblem/algorithms/updateAlgorithms.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/clusteringProblem/algorithms/updateAlgorithms.cc -std=c++11

clusterHelpers.o: ../cryptocurrencyRecommendation/clusteringProblem/helpers/clusterHelpers.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/clusteringProblem/helpers/clusterHelpers.cc -std=c++11

user.o: ../cryptocurrencyRecommendation/user/user.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/user/user.cc -std=c++11

userHelpers.o: ../cryptocurrencyRecommendation/user/userHelpers/userHelpers.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/user/userHelpers/userHelpers.cc -std=c++11

itemToken.o: ../cryptocurrencyRecommendation/itemToken/itemToken.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/itemToken/itemToken.cc -std=c++11

recommendation.o: ../cryptocurrencyRecommendation/recommendationProblem/recommendation.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/recommendationProblem/recommendation.cc -std=c++11

recommendationHelpers.o: ../cryptocurrencyRecommendation/recommendationProblem/helpers/recommendationHelpers.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/recommendationProblem/helpers/recommendationHelpers.cc -std=c++11

recommendationAlgorithms.o: ../cryptocurrencyRecommendation/recommendationProblem/recommendationAlgorithms.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/recommendationProblem/recommendationAlgorithms.cc -std=c++11

.PHONY:
	clean
	check
	run

clean:
	rm -rf experiment experiment.o utils.o hashFunction.o item.o fileHandler.o lshEuclidean.o lshCosine.o hypercubeEuclidean.o hypercubeCosine.o cluster.o initAlgorithms.o assignAlgorithms.o updateAlgorithms.o clusterHelpers.o user.o userHelpers.o itemToken.o recommendation.o recommendationHelpers.o

check:
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --vgdb-error=1 ./experiment -i dataSets/twitter_dataset_small.csv -c cluster.conf -o outputFile.txt -d euclidean -complete

run:
	./experiment -i dataSets/twitter_dataset_small.csv -c cluster.conf -o outputFile.txt -d euclidean
