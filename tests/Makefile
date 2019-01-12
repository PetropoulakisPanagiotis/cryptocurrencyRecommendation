# Petropoulakis Panagiotis
CC = g++
FLAGS = -g -Wall

test: test.o test1.o test2.o utils.o user.o userHelpers.o itemToken.o
	$(CC) -o test $(FLAGS) test.o test1.o test2.o utils.o user.o userHelpers.o itemToken.o -std=c++11

test.o: test.cc
	$(CC) -c $(FLAGS) test.cc -std=c++11

test1.o: test1.cc
	$(CC) -c $(FLAGS) test1.cc -std=c++11

test2.o: test2.cc
	$(CC) -c $(FLAGS) test2.cc -std=c++11

utils.o: ../cryptocurrencyRecommendation/utils/utils.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/utils/utils.cc -std=c++11

user.o: ../cryptocurrencyRecommendation/user/user.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/user/user.cc -std=c++11

userHelpers.o: ../cryptocurrencyRecommendation/user/userHelpers/userHelpers.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/user/userHelpers/userHelpers.cc -std=c++11

itemToken.o: ../cryptocurrencyRecommendation/itemToken/itemToken.cc
	$(CC) -c $(FLAGS) ../cryptocurrencyRecommendation/itemToken/itemToken.cc -std=c++11

.PHONY:
	clean
	runTests

clean:
	rm -rf test test.o test1.o test2.o utils.o user.o userHelpers.o itemToken.o

runTests:
	make && ./test



