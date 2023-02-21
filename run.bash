#!/bin/bash

g++ ./client/main_tcp_linux.cpp -o ./client/main_tcp_linux.out
g++ ./server/main_tcp_linux.cpp -o ./server/main_tcp_linux.out
./client/main_tcp_linux.out &
./server/main_tcp_linux.out &