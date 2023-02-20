#!/bin/bash

g++ ./client/main_linux.cpp -o ./client/main_linux.out
g++ ./server/main_linux.cpp -o ./server/main_linux.out
./client/main_linux.out &
./server/main_linux.out &