#!/bin/sh

g++ -o TestMyQueueVector TestMyQueueVector.cpp MyQueueVector.cpp rcpthread.cpp -lpthread  -Wall -g -fPIC -std=c++11
